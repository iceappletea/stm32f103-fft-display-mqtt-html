#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "malloc.h"
#include "usart3.h"
#include "common.h"
#include "MQTTPacket.h"
#include "mqtt_port.h"
#include "AD.h"
#include "lcd.h"
#include "Encoder.h"

#define arr_value 1632
#define FFT_TOPIC "fft/data"

static const char* WIFI_SSID = "abc";
static const char* WIFI_PWD = "0931309933";
static const char* MQTT_BROKER_HOST = "192.168.217.166";
static const int MQTT_BROKER_PORT = 1883;

float sample_rate = 0.0f;
uint32_t lBufOutArray[NPT];
uint8_t CS_Flag;
int16_t psc_value = 11;

extern uint32_t fft_max_amp;
extern uint32_t fft_peak_freq;

float SampleRate(uint16_t arr, uint16_t psc);
void twistEncoder(void);
void DrawLCD(void);

int main(void)
{
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(115200);
    LED_Init();
    usart3_init(115200);
    my_mem_init(SRAMIN);

    printf("==== [MQTT Publish Demo Start] ====\r\n");
    delay_ms(500);

    while (1)
    {
        // [1] WiFi Connect Retry
        while (esp8266_connect_ap(WIFI_SSID, WIFI_PWD) != 0)
        {
            printf("[Retry] WiFi connect failed, retrying...\r\n");
            delay_ms(2000);
        }
        printf("[OK] WiFi connected\r\n");

        // [2] TCP Connect Retry
        while (esp8266_tcp_connect(MQTT_BROKER_HOST, MQTT_BROKER_PORT) != 0)
        {
            printf("[Retry] TCP connect failed, retrying...\r\n");
            delay_ms(2000);
        }
        printf("[OK] TCP connected\r\n");

        atk_8266_send_cmd((u8 *)"AT+CIPSTO=7200", (u8 *)"OK", 50);

        // [3] MQTT CONNECT Retry
        while (mqtt_connect("STM32Client", 60) != 0)
        {
            printf("[Retry] MQTT CONNECT failed, retrying...\r\n");
            delay_ms(2000);
        }
        printf("[OK] MQTT CONNECT success\r\n");

        // [4] Initialize LCD + ADC + FFT
        printf("[Info] Init LCD + ADC + FFT\r\n");
        Lcd_Init();
        LCD_Clear(WHITE);
        BACK_COLOR = WHITE;

        Encoder_Init();
        AD_Init();
        TIM3_init(arr_value, psc_value);
        sample_rate = SampleRate(arr_value, psc_value);

        // [5] Main Loop: Sample, FFT, Publish
        while (1)
        {
            twistEncoder();

            ADCDMA_Start();
            ADCData();
            cr4_fft_256_stm32(lBufOutArray, AD_Data, NPT);
            AnalyzeFFT(lBufOutArray, NPT, sample_rate);

            DrawLCD();

            char buffer[64];
            sprintf(buffer, "%lu,%lu,%d,%d", fft_peak_freq, fft_max_amp, psc_value, (int)sample_rate);

            mqtt_publish(FFT_TOPIC, buffer, 0);
            printf("Publish FFT: %s\r\n", buffer);

            LED0 = !LED0;
            delay_ms(1000);
        }
        delay_ms(1000);
    }
}

// [îlÂÊ“QËã]
float SampleRate(uint16_t arr, uint16_t psc)
{
    return 72000000.0f / ((psc + 1) * (arr + 1));
}

// [ÐýÞD¾Ž´aÆ÷¿ØÖÆ PSC]
void twistEncoder(void)
{
    int16_t EncoderCount = Encoder_Get();

    if (EncoderCount != 0)
    {
        int16_t new_psc_value = psc_value + EncoderCount;
        if (new_psc_value < 0) new_psc_value = 16;
        if (new_psc_value > 16) new_psc_value = 0;

        psc_value = new_psc_value;
        TIM3_init(arr_value, psc_value);
        sample_rate = SampleRate(arr_value, psc_value);
    }
}

// [ÇÐ“Qï@Ê¾ Waveform / FFT]
void DrawLCD(void)
{
    CS_Flag = !CS_Flag;

    if (CS_Flag)
    {
        LCD_CS_Set();
        LCD_CS1_Clr();
        ST7789_DrawWaveform(AD_Data, 240, psc_value, sample_rate);
    }
    else
    {
        LCD_CS_Clr();
        LCD_CS1_Set();
        ST7789_DrawFFT(lBufOutArray, 240, sample_rate);
    }
}
