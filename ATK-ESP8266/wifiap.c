#include "common.h"

uint8_t atk_8266_wifiap_test(void)
{
    uint8_t ipbuf[16];
    uint8_t *p;
    uint8_t res = 0;

    p = mymalloc(SRAMIN, 100);

    atk_8266_send_cmd("AT+CWMODE=2", "OK", 50);

    sprintf((char *)p, "AT+CWSAP=\"%s\",\"%s\",1,4", wifiap_ssid, wifiap_password);
    atk_8266_send_cmd(p, "OK", 1000);

    atk_8266_send_cmd("AT+CIPMUX=1", "OK", 50);
    sprintf((char *)p, "AT+CIPSERVER=1,%s", (uint8_t *)portnum);
    atk_8266_send_cmd(p, "OK", 50);
    atk_8266_send_cmd("AT+CIPSTO=1200", "OK", 50);

    atk_8266_get_wanip(ipbuf);
    printf("AP mode started successfully£¬IP=%s Port=%s\r\n", ipbuf, portnum);

    sprintf((char *)p, "AT+CIPSEND=0,17");
    if (atk_8266_send_cmd(p, ">", 100) == 0)
    {
        atk_8266_send_data((uint8_t *)"WIFI AP TEST DATA", "SEND OK", 100);
    }

    myfree(SRAMIN, p);
    return res;
}
