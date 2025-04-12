#include "common.h"

const uint8_t * portnum = "8086";

const uint8_t * wifista_ssid       = "abc";
const uint8_t * wifista_encryption = "wpawpa2_aes";
const uint8_t * wifista_password   = "0931309933";

const uint8_t * wifiap_ssid        = "ATK-ESP8266";
const uint8_t * wifiap_encryption  = "wpawpa2_aes";
const uint8_t * wifiap_password    = "12345678";

void atk_8266_at_response(uint8_t mode)
{
    if ((USART3_RX_STA & 0x8000) != 0)
    {
        USART3_RX_BUF[USART3_RX_STA & 0x7FFF] = 0;
        printf("%s", USART3_RX_BUF);

        if (mode != 0)
        {
            USART3_RX_STA = 0;
        }
    }
}

uint8_t * atk_8266_check_cmd(uint8_t * str)
{
    char * strx = 0;

    if ((USART3_RX_STA & 0x8000) != 0)
    {
        USART3_RX_BUF[USART3_RX_STA & 0x7FFF] = 0;
        strx = strstr((const char *)USART3_RX_BUF, (const char *)str);
    }

    return (uint8_t *)strx;
}

uint8_t atk_8266_send_cmd(uint8_t * cmd, uint8_t * ack, uint16_t waittime)
{
    uint8_t res = 0;

    USART3_RX_STA = 0;

    u3_printf("%s\r\n", cmd);

    if ((ack != 0) && (waittime != 0))
    {
        while (--waittime)
        {
            delay_ms(10);

            if ((USART3_RX_STA & 0x8000) != 0)
            {
                if (atk_8266_check_cmd(ack) != 0)
                {
                    break;
                }

                USART3_RX_STA = 0;
            }
        }

        if (waittime == 0)
        {
            res = 1;
        }
    }

    return res;
}

uint8_t atk_8266_send_data(uint8_t * data, uint8_t * ack, uint16_t waittime)
{
    uint8_t res = 0;

    USART3_RX_STA = 0;

    u3_printf("%s", data);

    if ((ack != 0) && (waittime != 0))
    {
        while (--waittime)
        {
            delay_ms(10);

            if ((USART3_RX_STA & 0x8000) != 0)
            {
                if (atk_8266_check_cmd(ack) != 0)
                {
                    break;
                }

                USART3_RX_STA = 0;
            }
        }

        if (waittime == 0)
        {
            res = 1;
        }
    }

    return res;
}

uint8_t atk_8266_quit_trans(void)
{
    while ((USART3->SR & 0x40) == 0);
    USART3->DR = '+';
    delay_ms(15);

    while ((USART3->SR & 0x40) == 0);
    USART3->DR = '+';
    delay_ms(15);

    while ((USART3->SR & 0x40) == 0);
    USART3->DR = '+';
    delay_ms(500);

    return atk_8266_send_cmd("AT", "OK", 20);
}

void atk_8266_get_wanip(uint8_t *ipbuf)
{
    atk_8266_send_cmd("AT+CIFSR", "OK", 100);

    char *p;
    p = strstr((const char *)USART3_RX_BUF, "STAIP,\"");

    if (p)
    {
        p = p + 7;

        uint8_t i;
        i = 0;

        while (*p != '"' && i < 15)
        {
            ipbuf[i] = *p;
            i = i + 1;
            p = p + 1;
        }

        ipbuf[i] = 0;
    }
    else
    {
        strcpy((char *)ipbuf, "0.0.0.0");
    }
}

void atk_8266_test(void)
{
    while (atk_8266_send_cmd("AT", "OK", 20))
    {
        atk_8266_quit_trans();
        atk_8266_send_cmd("AT+CIPMODE=0", "OK", 200);
        delay_ms(800);
    }

    atk_8266_send_cmd("ATE0", "OK", 20);
    atk_8266_send_cmd("AT+CWMODE=1", "OK", 50);
    atk_8266_wifista_test();

    while (1)
    {
        delay_ms(500);
        LED0 = !LED0;
    }
}
