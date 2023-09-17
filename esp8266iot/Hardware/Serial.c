#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include <DELAY.h>
#include "serial.h"
#include "OLED.h"

#define serbuff 1024
char Serial_RxPacket[serbuff];				//"@MSG\r\n"
uint8_t Serial_RxFlag = 0;


void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

//wifi
/*********************************************************************************************/
char* command[] = {"AT+CWMODE=1\r\n",
	"AT+CWJAP=\"lo\",\"66666666\"\r\n",
"AT+MQTTUSERCFG=0,1,\"123456|securemode=2\\,signmethod=hmacsha1\\,timestamp=1693640945952|\",\"eps8266&iu51wddAmdO\",\"56CF879E39EDC3B5513684524B3D43D5BFAA299A\",0,0,\"\"\r\n",
"AT+MQTTCONN=0,\"iu51wddAmdO.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,0\r\n",
"AT+MQTTSUB=0,\"/sys/iu51wddAmdO/eps8266/thing/service/property/set\",0\r\n"
};

 uint8_t Wifiintflag = 0;


#define msgbuffer 256

unsigned char msg[msgbuffer];

void WifiInit(){

	Delay_ms(1000);
	for(unsigned char i = 0 ; i<5 ; i++){
		
		while(Serial_RxFlag == 0){
		printf("%s",command[i]);
		Delay_ms(1000);		
	}	
		Serial_RxFlag = 0;	
  }
	OLED_ShowString(2, 1, "WIFI OK");
	Wifiintflag = 1;
  
}


void Publish(unsigned char ledstate,unsigned char currert){


	sprintf((char *)msg,"AT+MQTTPUB=0,\""PUB_TOPIC"\",\""JSON_FORMAT"\",0,0\r\n",ledstate,currert);
	printf("%s",msg);
	
}

void Subscribe(unsigned char ledstate,unsigned char currert){


}

 uint8_t Rxflag = 0;
 int pRxPacket = 0;

void wifinitconditon(unsigned char RxData){
	
				if (RxData == 'O')
			{				
//				pRxPacket = 0;	
				Rxflag = 1;
			}
					
			if (RxData == 'K' && (Rxflag == 1))
			{				
				Serial_RxFlag = 1;		
				Rxflag = 0;
			}
			
		 if ((RxData != 'k') && (RxData != 'O')){
			
				Rxflag = 0 ;
			}		
				

}


void USART1_IRQHandler(void)
{


	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
	if(Wifiintflag == 0)
		wifinitconditon(RxData);
	else
	{		
		Serial_RxPacket[pRxPacket] = RxData;
		pRxPacket ++; 
		if(pRxPacket >= serbuff ) pRxPacket = 0;
	}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}	
	
   
}