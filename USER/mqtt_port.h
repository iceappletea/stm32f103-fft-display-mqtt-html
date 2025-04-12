#ifndef __MQTT_PORT_H
#define __MQTT_PORT_H

// 將對外開放的函式聲明
int esp8266_connect_ap(const char* ssid, const char* pwd);
int esp8266_tcp_connect(const char* host, int port);

int mqtt_connect(const char* clientID, int keepAlive);
int mqtt_publish(const char* topic, const char* msg, int qos);

#endif
