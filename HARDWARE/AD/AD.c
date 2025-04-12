#include "AD.h"
#include "usart.h"
#include "lcd.h"
#include "delay.h"

uint32_t ADValue[NPT];
uint16_t AD_Data[NPT * 2];

uint32_t fft_max_amp = 0;
uint32_t fft_peak_freq = 0;

void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 1, ADC_SampleTime_239Cycles5);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_FastInterl;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;  
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_Init(ADC2, &ADC_InitStructure);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADValue;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = NPT;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    ADC_ExternalTrigConvCmd(ADC2, ENABLE);

    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    ADC_Cmd(ADC2, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

    ADC_ResetCalibration(ADC2);
    while (ADC_GetResetCalibrationStatus(ADC2) == SET);
    ADC_StartCalibration(ADC2);
    while (ADC_GetCalibrationStatus(ADC2) == SET);
}

void TIM3_init(uint16_t arr,uint16_t psc)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_InternalClockConfig(TIM3);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    TIM_Cmd(TIM3, ENABLE);
}

void ADCDMA_Start(void)
{
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, NPT);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void ADCData(void)
{
    int i = 0;
    uint32_t value = 0;
    uint16_t adc1_value = 0;
    uint16_t adc2_value = 0;

    for (i = 0; i < NPT; i++)
    {
        value = ADValue[i];
        adc1_value = (uint16_t)(value >> 16);
        adc2_value = (uint16_t)(value & 0x0FFF);

        AD_Data[2 * i] = adc1_value;
        AD_Data[2 * i + 1] = adc2_value;
    }
}

void ST7789_DrawWaveform(uint16_t * data, uint16_t length, int16_t psc_value, float sample_rate)
{
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t prev_x = 0;
    uint16_t prev_y = 0;
    uint16_t displayed_points = 240;

    int16_t offset = 800;
    uint16_t range = 2400;

    uint8_t str[30];
    int int_rate = (int)sample_rate;

    LCD_Clear(WHITE);

    sprintf((char *)str, "psc=%d", psc_value);
    LCD_ShowString(5, 5, str, GRAYBLUE);

    sprintf((char *)str, "Max Sampling range: %dHz", int_rate);
    LCD_ShowString(5, 25, str, GRAYBLUE);

    prev_x = 0;
    prev_y = 240 - ((data[0] - offset) * 240 / range) + 60;

    for (x = 1; x < displayed_points; x++)
    {
        y = 240 - ((data[x] - offset) * 240 / range) + 60;

        if (y >= 240)
        {
            y = 240;
        }
        else if (y <= 0)
        {
            y = 0;
        }

        LCD_DrawLine(prev_x, prev_y, x, y, BLACK);

        prev_x = x;
        prev_y = y;
    }
}

void ST7789_DrawFFT(uint32_t * fftData, uint16_t length, float sample_rate)
{
    int i = 0;
    int k = 0;
    int max_value = 0;
    int magY[64];
    int16_t real = 0;
    int16_t imag = 0;
    int magnitude[64];
    int peak_freq = 0;

    LCD_Clear(WHITE);

    for (i = 1; i < 64; i++)
    {
        real = (int16_t)((fftData[i] << 16) >> 16);
        imag = (int16_t)((fftData[i] >> 16));

        magnitude[i] = ((int)sqrt((float)(real * real + imag * imag)) >> 3) + 1;

        if (magnitude[i] > max_value)
        {
            max_value = magnitude[i];
            k = i;
        }

        magY[i] = 240 - (magnitude[i] * 4);

        if (magY[i] > 240)
        {
            magY[i] = 240;
        }

        if (magY[i] < 0)
        {
            magY[i] = 0;
        }
    }

    peak_freq = (int)(k * (sample_rate / (float)NPT));

    LCD_ShowString(15, 5, "peak_freq = ", GRAYBLUE);
    LCD_ShowNum(105, 5, peak_freq, 5, BRRED);
    LCD_ShowString(165, 5, "Hz", GRAYBLUE);

    LCD_ShowString(15, 25, "max_amp = ", GRAYBLUE);
    LCD_ShowNum(85, 25, max_value, 5, BRRED);

    for (i = 1; i < 64; i++)
    {
        LCD_DrawLine(i * 4, 240, i * 4, magY[i], BLACK);
        LCD_DrawLine(i * 4 + 2, 240, i * 4 + 2, magY[i], BLACK);
    }
}

void AnalyzeFFT(uint32_t * fftData, uint16_t length, float sample_rate)
{
    int max_index = 0;
    int max_magnitude = 0;
    int i;

    for (i = 1; i < 64; i++)
    {
        int16_t real;
        int16_t imag;
        int magnitude;

        real = (int16_t)((fftData[i] << 16) >> 16);
        imag = (int16_t)(fftData[i] >> 16);
        magnitude = (real * real + imag * imag) >> 3;

        if (magnitude > max_magnitude)
        {
            max_magnitude = magnitude;
            max_index = i;
        }
    }

    fft_max_amp = max_magnitude;
    fft_peak_freq = (int)(max_index * (sample_rate / (float)NPT));
}
