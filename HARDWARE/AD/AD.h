#ifndef __AD_H
#define __AD_H

#include "sys.h"

#define NPT 256

extern uint32_t ADValue[NPT];
extern uint16_t AD_Data[NPT * 2];
extern uint32_t fft_max_amp;
extern uint32_t fft_peak_freq;

void AD_Init(void);
void TIM3_init(uint16_t arr,uint16_t psc);
void ADCDMA_Start(void);
uint16_t AD_Getvalue(void);
void ADCData(void);
void ST7789_DrawWaveform(uint16_t* data, uint16_t length, int16_t psc_value, float sample_rate);
void ST7789_DrawFFT(uint32_t* fftData, uint16_t length, float sample_rate);
void AnalyzeFFT(uint32_t* fftData, uint16_t length, float sample_rate);

#endif
