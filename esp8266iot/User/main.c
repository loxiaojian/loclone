#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "KEY.h"
#include <stdio.h>

int main(void)
{
	OLED_Init();
	LED_Init();
	Serial_Init();	
	Key_Init();
	OLED_ShowString(1, 1, "WIFI CONNECT");
	WifiInit();
	unsigned char msg[256];
	unsigned char num = 0;
	while (1)
	{
	
		Delay_ms(1000);
		sprintf((char *)msg,"AT+MQTTPUB=0,\""PUB_TOPIC"\",\""JSON_FORMAT"\",0,0\r\n",num,2);
		printf("%s",msg);
		OLED_ShowNum(3,0,num++,8);
		if(num>=255) num = 0;
	}
}
