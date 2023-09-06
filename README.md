MQTT
/sys/ProductKey/DeviceName/thing/event/property/post 消息发布主题

/sys/ProductKey/DeviceName/thing/service/property/set 消息订阅主题

{"params":{"temp":1,"humi":4},"version":"1.0.0"} 发送json数据


ESP8266
发送AT指令","需要"\,"
AT+CWMODE=1

AT+CWJAP="lo","66666666"

AT+MQTTUSERCFG=0,1,"123456|securemode=2\,signmethod=hmacsha1\,timestamp=1693640945952|","eps8266&iu51wddAmdO","56CF879E39EDC3B5513684524B3D43D5BFAA299A",0,0,""

AT+MQTTCONN=0,"iu51wddAmdO.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883,0

AT+MQTTSUB=0,"/sys/iu51wddAmdO/eps8266/thing/service/property/set",0

AT+MQTTPUB=0,"/sys/iu51wddAmdO/eps8266/thing/event/property/post","{\"params\":{\"led\":60}\,\"version\":\"1.0.0\"}",0,0