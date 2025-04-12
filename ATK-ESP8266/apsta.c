#include "common.h"

uint8_t atk_8266_apsta_test(void)
{
    uint8_t * p;
    uint8_t res = 0;

    p = mymalloc(SRAMIN, 100);

    atk_8266_send_cmd("AT+CWMODE=3", "OK", 50);

    sprintf((char *)p, "AT+CWSAP=\"%s\",\"%s\",1,4", wifiap_ssid, wifiap_password);
    atk_8266_send_cmd(p, "OK", 1000);

    sprintf((char *)p, "AT+CWJAP=\"%s\",\"%s\"", wifista_ssid, wifista_password);
    atk_8266_send_cmd(p, "WIFI GOT IP", 1000);

    while (1)
    {
        if (atk_8266_send_cmd("AT+CIFSR", "STAIP", 20) == 0)
        {
            break;
        }
    }

    while (1)
    {
        if (atk_8266_send_cmd("AT+CIFSR", "APIP", 20) == 0)
        {
            break;
        }
    }

    atk_8266_send_cmd("AT+CIPMUX=1", "OK", 50);
    delay_ms(500);

    sprintf((char *)p, "AT+CIPSERVER=1,%s", (uint8_t *)portnum);
    atk_8266_send_cmd(p, "OK", 50);
    delay_ms(500);

    atk_8266_send_cmd("AT+CIPSTO=1200", "OK", 50);

    sprintf((char *)p, "AT+CIPSEND=0,16");
    atk_8266_send_cmd(p, ">", 100);
    atk_8266_send_data((uint8_t *)"APSTA test data", "SEND OK", 100);

    myfree(SRAMIN, p);

    return res;
}
