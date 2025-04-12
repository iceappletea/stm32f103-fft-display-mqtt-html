#ifndef __COMMON_H__
#define __COMMON_H__

#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "malloc.h"
#include "string.h"
#include "usart3.h"

void atk_8266_at_response(uint8_t mode);
uint8_t* atk_8266_check_cmd(uint8_t *str);
uint8_t atk_8266_send_cmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime);
uint8_t atk_8266_send_data(uint8_t *data, uint8_t *ack, uint16_t waittime);
uint8_t atk_8266_quit_trans(void);
uint8_t atk_8266_apsta_check(void);
uint8_t atk_8266_consta_check(void);
void atk_8266_test(void);
void atk_8266_get_wanip(uint8_t* ipbuf);
uint8_t atk_8266_wifista_test(void);

extern const uint8_t* wifiap_ssid;
extern const uint8_t* wifiap_password;
extern const uint8_t* wifista_ssid;
extern const uint8_t* wifista_password;
extern const uint8_t* portnum;

#endif
