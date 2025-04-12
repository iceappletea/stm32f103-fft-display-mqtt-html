#ifndef __LCD_H
#define __LCD_H

#include "sys.h"

#define USE_HORIZONTAL 0

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 240

#else
#define LCD_W 240
#define LCD_H 240
#endif

#define LED_ON GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define LED_OFF GPIO_SetBits(GPIOA,GPIO_Pin_15)

#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define LCD_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define LCD_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define LCD_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define LCD_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define LCD_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)

#define LCD_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define LCD_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define LCD_CS_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define LCD_CS_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define LCD_CS1_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define LCD_CS1_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define LCD_BLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define LCD_BLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LCD_CMD 0
#define LCD_DATA 1

extern uint16_t BACK_COLOR;
extern uint8_t CS_Flag;

void LCD_Writ_Bus(uint8_t dat);
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void Lcd_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t index,uint8_t size,uint16_t color);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_DrawPoint_big(uint16_t x,uint16_t y,uint16_t colory);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode,uint16_t color);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t color);
uint32_t mypow(uint8_t m,uint8_t n);
void LCD_ShowNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t color);
void LCD_ShowNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t color);
void LCD_ShowPicture(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458
#define LIGHTGREEN 0X841F
#define LGRAY 0XC618
#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12
					  		 
#endif
