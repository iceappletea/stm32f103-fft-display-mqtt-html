#include "mqtt_port.h"
#include "MQTTPacket.h"
#include "usart3.h"
#include "common.h"
#include <string.h>
#include <stdio.h>

// 等待時間
#define AT_RESP_TIMEOUT  50

//----------------------------------------------------------------------------
// 1) 連接 WiFi
int esp8266_connect_ap(const char *ssid, const char *password)
{
    char cmd[128];
    atk_8266_quit_trans();
    atk_8266_send_cmd((u8*)"AT+CWQAP", (u8*)"OK", 50);
    delay_ms(500);

    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
    printf("[Send] %s\r\n", cmd);
    USART3_RX_STA = 0;
    u3_printf("%s\r\n", cmd);

    for (int i = 0; i < 400; i++)
    {
        delay_ms(50);
        if (USART3_RX_STA & 0x8000)
		{
			printf("[Recv] %s\r\n", USART3_RX_BUF);  // 保留這行很重要！

			if (strstr((char*)USART3_RX_BUF, "GOT IP") || 
				strstr((char*)USART3_RX_BUF, "WIFI GOT IP"))
			{
				printf("[OK] GOT IP!\r\n");
				return 0;
			}
			USART3_RX_STA = 0;
		}

    }
    printf("[ERR] Timeout, no GOT IP\r\n");
    return -2;
}

//----------------------------------------------------------------------------
// 2) 建立 TCP 連線
int esp8266_tcp_connect(const char* host, int port)
{
    char cmd[128];
    sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%d", host, port);
    USART3_RX_STA = 0;
    u3_printf("%s\r\n", cmd);

    for (int i = 0; i < 100; i++)
    {
        delay_ms(50);
        if (USART3_RX_STA & 0x8000)
        {
            printf("[Recv] %s\r\n", USART3_RX_BUF);
            if (strstr((char*)USART3_RX_BUF, "CONNECT") || strstr((char*)USART3_RX_BUF, "OK"))
            {
                delay_ms(2000); // 等連線穩定

                // ?? 新增這段確認 link 是否還有效
                atk_8266_send_cmd((u8*)"AT+CIPSTATUS", (u8*)"+CIPSTATUS:", 50);
                if (strstr((char*)USART3_RX_BUF, "+CIPSTATUS:0"))
                {
                    printf("[OK] Link confirmed alive.\r\n");
                    return 0;
                }
                else
                {
                    printf("[ERR] Link was closed after connect.\r\n");
                    return -2;
                }
            }
            USART3_RX_STA = 0;
        }
    }
    printf("[ERR] TCP connect fail\r\n");
    return -1;
}

//----------------------------------------------------------------------------
// 發送 MQTT CONNECT 封包
int mqtt_connect(const char *client_id, int keepalive)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    unsigned char buf[200];
    int len;

    data.clientID.cstring = (char *)client_id;
    data.keepAliveInterval = keepalive;
    data.cleansession = 1;
    data.MQTTVersion = 4;

    len = MQTTSerialize_connect(buf, sizeof(buf), &data);
    if (len <= 0)
    {
        printf("[ERR] MQTT Serialize fail!\r\n");
        return -1;
    }
    printf("[Info] MQTT CONNECT length = %d\r\n", len);

    // 確保 CIPMODE=0
    atk_8266_send_cmd((u8*)"AT+CIPMODE=0", (u8*)"OK", 20);

    // AT+CIPSEND=length
    char cmd[32];
    sprintf(cmd, "AT+CIPSEND=%d", len);
    int send_ready = 0;
	for (int retry = 0; retry < 3; retry++)
	{
		USART3_RX_STA = 0;
		u3_printf("%s\r\n", cmd);  // 發送 AT+CIPSEND

		for (int i = 0; i < 100; i++)  // 最長等待 5 秒
		{
			delay_ms(50);
			if (USART3_RX_STA & 0x8000)
			{
				printf("[Recv][CIPSEND] %s\r\n", USART3_RX_BUF);  // 顯示 ESP8266 原始回應
				if (strstr((char*)USART3_RX_BUF, ">"))
				{
					printf("[OK] CIPSEND ready.\r\n");
					send_ready = 1;
					break;
				}
				break;
			}
		}

		if (send_ready) break;

		printf("[WARN] No > received, retrying...\r\n");
		delay_ms(500);  // 等 ESP8266 不忙了
	}

	if (!send_ready)
	{
		printf("[ERR] AT+CIPSEND failed after retries!\r\n");
		return -2;
	}


    // 發送封包
    printf("[Info] Sending CONNECT packet...\r\n");
    for (int i = 0; i < len; i++)
    {
        while ((USART3->SR & 0X40) == 0);
        USART3->DR = buf[i];
    }

    // 等待 CONNACK
    for (int i = 0; i < 200; i++)
    {
        delay_ms(50);
        if (USART3_RX_STA & 0x8000)
        {
            printf("[Recv] %s\r\n", USART3_RX_BUF);
            if (strstr((char*)USART3_RX_BUF, "\x20")) // 0x20 是 CONNACK
            {
                printf("[OK] CONNACK Received!\r\n");
                return 0;
            }
            USART3_RX_STA = 0;
        }
    }
    printf("[ERR] CONNACK Timeout!\r\n");
    return -3;
}

//----------------------------------------------------------------------------
// 發送 Publish 封包
int mqtt_publish(const char* topic, const char* msg, int qos)
{
    unsigned char buf[256];
    MQTTString topicStr = MQTTString_initializer;
    topicStr.cstring = (char*)topic;

    int len = MQTTSerialize_publish(buf, sizeof(buf), 0, qos, 0, 0, topicStr, (unsigned char*)msg, strlen(msg));
    if (len <= 0)
    {
        printf("[ERR] MQTT Serialize publish fail\r\n");
        return -1;
    }

    char cipsend[32];
    sprintf(cipsend, "AT+CIPSEND=%d", len);
    if (atk_8266_send_cmd((u8*)cipsend, (u8*)">", 50) != 0)
    {
        printf("[ERR] AT+CIPSEND failed (publish)\r\n");
        return -2;
    }

    printf("[Info] Publish sending: topic=%s, msg=%s\r\n", topic, msg);
    for (int i = 0; i < len; i++)
    {
        while ((USART3->SR & 0X40) == 0);
        USART3->DR = buf[i];
    }
    return 0;
}
