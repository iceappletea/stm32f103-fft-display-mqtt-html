#include "common.h"

uint8_t atk_8266_wifista_test(void)
{
	uint8_t ipbuf[16];
	uint8_t *p;
	uint8_t res = 0;

	p = mymalloc(SRAMIN, 100);

	atk_8266_send_cmd("AT+CWMODE=1", "OK", 50);
	atk_8266_send_cmd("AT+RST", "OK", 50);
	delay_ms(3000);

	sprintf((char *)p, "AT+CWJAP=\"%s\",\"%s\"", wifista_ssid, wifista_password);
	if (atk_8266_send_cmd(p, "WIFI GOT IP", 1000) != 0)
	{
		printf("Failed to connect to WiFi\r\n");
		myfree(SRAMIN, p);
		return 1;
	}
	printf("WiFi connected successfully\r\n");

	atk_8266_get_wanip(ipbuf);
	printf("STA IP address: %s\r\n", ipbuf);

	atk_8266_send_cmd("AT+CIPMUX=0", "OK", 50);
	sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"192.168.1.123\",%s", portnum);
	if (atk_8266_send_cmd(p, "OK", 200) != 0)
	{
		printf("Failed to connect to TCP server\r\n");
		myfree(SRAMIN, p);
		return 2;
	}
	printf("TCP connection established\r\n");

	sprintf((char *)p, "AT+CIPSEND=25");
	if (atk_8266_send_cmd(p, ">", 100) == 0)
	{
		atk_8266_send_data((uint8_t *)"WIFI STA TEST MESSAGE\r\n", "SEND OK", 100);
		printf("Test message sent\r\n");
	}

	myfree(SRAMIN, p);
	return res;
}
