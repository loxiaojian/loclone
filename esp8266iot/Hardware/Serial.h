#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;
extern uint8_t Serial_OKFlag;
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void WifiInit();

#define sub_topic "/sys/iu51wddAmdO/eps8266/thing/service/property/set"
#define PUB_TOPIC "/sys/iu51wddAmdO/eps8266/thing/event/property/post"
#define JSON_FORMAT      "{\\\"params\\\":{\\\"led\\\":%d\\,\\\"Current\\\":%d\\}\\,\\\"version\\\":\\\"1.0.0\\\"}"


#endif
