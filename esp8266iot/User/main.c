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
	
	unsigned char num = 0;
	while (1)
	{
		Publish(num++,20);
		Delay_ms(1000);
		OLED_ShowNum(3,0,num,8);
		if(num>=255) num = 0;
	}
}
