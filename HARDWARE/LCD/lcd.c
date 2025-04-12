#include "lcd.h"
#include "LCDfont.h"
#include "delay.h"
#include "bmp.h"

uint16_t BACK_COLOR;

void LCD_Writ_Bus(uint8_t dat) 
{	
    uint8_t i;
    for (i = 0; i < 8; i++) 
    {			  
        LCD_SCLK_Clr();
        if (dat & 0x80) 
        {
            LCD_SDIN_Set();
        }
        else 
        {
            LCD_SDIN_Clr();
        }
        LCD_SCLK_Set();
        dat <<= 1;   
    }
}

void LCD_WR_DATA8(uint8_t dat)
{
    LCD_DC_Set();
    LCD_Writ_Bus(dat);
}

void LCD_WR_DATA(uint16_t dat)
{
    LCD_DC_Set();
    LCD_Writ_Bus(dat >> 8);
    LCD_Writ_Bus(dat);
}

void LCD_WR_REG(uint8_t dat)
{
    LCD_DC_Clr();
    LCD_Writ_Bus(dat);
}

void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (USE_HORIZONTAL == 0)
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);
    }
    else if (USE_HORIZONTAL == 1)
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1 + 80);
        LCD_WR_DATA(y2 + 80);
        LCD_WR_REG(0x2c);
    }
    else if (USE_HORIZONTAL == 2)
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);
    }
    else
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1 + 80);
        LCD_WR_DATA(x2 + 80);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);
    }
}

void Lcd_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_15;	 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_15);

    LCD_CS_Clr();
    LCD_CS1_Clr();
    
    LCD_RST_Clr();
    delay_ms(100);
    LCD_RST_Set();
    delay_ms(100);
    LCD_BLK_Set();

    LCD_WR_REG(0x36);
    
    if (USE_HORIZONTAL == 0)
    {
        LCD_WR_DATA8(0x00);
    }
    else if (USE_HORIZONTAL == 1)
    {
        LCD_WR_DATA8(0xC0);
    }
    else if (USE_HORIZONTAL == 2)
    {
        LCD_WR_DATA8(0x70);
    }
    else
    {
        LCD_WR_DATA8(0xA0);
    }

    LCD_WR_REG(0x3A); 
    LCD_WR_DATA8(0x05);

    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33); 

    LCD_WR_REG(0xB7); 
    LCD_WR_DATA8(0x35);  

    LCD_WR_REG(0xBB);
    LCD_WR_DATA8(0x19);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x2C);

    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x12);   

    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x20);  

    LCD_WR_REG(0xC6); 
    LCD_WR_DATA8(0x0F);    

    LCD_WR_REG(0xD0); 
    LCD_WR_DATA8(0xA4);
    LCD_WR_DATA8(0xA1);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x54);
    LCD_WR_DATA8(0x4C);
    LCD_WR_DATA8(0x18);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x44);
    LCD_WR_DATA8(0x51);
    LCD_WR_DATA8(0x2F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0x21); 

    LCD_WR_REG(0x11); 

    LCD_WR_REG(0x29);
}

void LCD_Clear(uint16_t Color)
{
    uint16_t i, j;  	
    LCD_Address_Set(0, 0, LCD_W - 1, LCD_H - 1);
    for (i = 0; i < LCD_W; i++) 
    {
        for (j = 0; j < LCD_H; j++) 
        {
            LCD_WR_DATA(Color);
        }
    }
}

void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t index, uint8_t size, uint16_t color)	
{  
    uint8_t i, j;
    uint8_t *temp, size1;

    if (size == 16) 
    {
        temp = Hzk16;
    }
    if (size == 32) 
    {
        temp = Hzk32;
    }

    LCD_Address_Set(x, y, x + size - 1, y + size - 1);
    size1 = size * size / 8;
    temp += index * size1;

    for (j = 0; j < size1; j++) 
    {
        for (i = 0; i < 8; i++) 
        {
            if ((*temp & (1 << i)) != 0) 
            {
                LCD_WR_DATA(color);
            }
            else 
            {
                LCD_WR_DATA(BACK_COLOR);
            }
        }
        temp++;
    }
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Address_Set(x, y, x, y);
    LCD_WR_DATA(color);
}

void LCD_DrawPoint_big(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Fill(x - 1, y - 1, x + 1, y + 1, color);
}

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{          
    uint16_t i, j; 
    LCD_Address_Set(xsta, ysta, xend, yend);
    for (i = ysta; i <= yend; i++) 
    {													   	 	
        for (j = xsta; j <= xend; j++) 
        {
            LCD_WR_DATA(color);
        }
    } 					  	    
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t; 
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if (delta_x > 0) 
    {
        incx = 1;
    }
    else if (delta_x == 0) 
    {
        incx = 0;
    }
    else 
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0) 
    {
        incy = 1;
    }
    else if (delta_y == 0) 
    {
        incy = 0;
    }
    else 
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y) 
    {
        distance = delta_x;
    }
    else 
    {
        distance = delta_y;
    }

    for (t = 0; t < distance + 1; t++) 
    {
        LCD_DrawPoint(uRow, uCol, color);
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance) 
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance) 
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a, b;
    a = 0; 
    b = r;	  
    while (a <= b) 
    {
        LCD_DrawPoint(x0 - b, y0 - a, color);
        LCD_DrawPoint(x0 + b, y0 - a, color);
        LCD_DrawPoint(x0 - a, y0 + b, color);
        LCD_DrawPoint(x0 - a, y0 - b, color);
        LCD_DrawPoint(x0 + b, y0 + a, color);
        LCD_DrawPoint(x0 + a, y0 - b, color);
        LCD_DrawPoint(x0 + a, y0 + b, color);
        LCD_DrawPoint(x0 - b, y0 + a, color);
        a++;
        if ((a * a + b * b) > (r * r)) 
        {
            b--;
        }
    }
}

void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t mode, uint16_t color)
{
    uint8_t temp;
    uint8_t pos, t;
    uint16_t x0 = x;     

    if (x > LCD_W - 16 || y > LCD_H - 16) 
    {
        return;
    }

    num = num - ' ';
    LCD_Address_Set(x, y, x + 8 - 1, y + 16 - 1);

    if (!mode) 
    {
        for (pos = 0; pos < 16; pos++) 
        { 
            temp = asc2_1608[(uint16_t)num * 16 + pos];
            for (t = 0; t < 8; t++) 
            {                 
                if (temp & 0x01) 
                {
                    LCD_WR_DATA(color);
                }
                else 
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
                temp >>= 1; 
                x++;
            }
            x = x0;
            y++;
        }	
    }
    else
    {
        for (pos = 0; pos < 16; pos++) 
        {
            temp = asc2_1608[(uint16_t)num * 16 + pos];
            for (t = 0; t < 8; t++) 
            {                 
                if (temp & 0x01) 
                {
                    LCD_DrawPoint(x + t, y + pos, color);
                }
                temp >>= 1; 
            }
        }
    }   	   	 	  
}

void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t color)
{         
    while (*p != '\0') 
    {       
        if (x > LCD_W - 16) 
        {
            x = 0;
            y += 16;
        }
        if (y > LCD_H - 16) 
        {
            y = x = 0;
            LCD_Clear(RED);
        }
        LCD_ShowChar(x, y, *p, 0, color);
        x += 8;
        p++;
    }  
}

uint32_t mypow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;	 
    while (n--) 
    {
        result *= m;    
    }
    return result;
}

void LCD_ShowNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t color)
{         	
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++) 
    {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) 
        {
            if (temp == 0) 
            {
                LCD_ShowChar(x + 8 * t, y, ' ', 0, color);
                continue;
            }
            else 
            {
                enshow = 1; 
            }
        }
        LCD_ShowChar(x + 8 * t, y, temp + 48, 0, color); 
    }
} 

void LCD_ShowNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t color)
{         	
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint16_t num1 = num * 100;

    for (t = 0; t < len; t++) 
    {
        temp = (num1 / mypow(10, len - t - 1)) % 10;
        if (t == (len - 2)) 
        {
            LCD_ShowChar(x + 8 * (len - 2), y, '.', 0, color);
            t++;
            len += 1;
        }
        LCD_ShowChar(x + 8 * t, y, temp + 48, 0, color);
    }
}

void LCD_ShowPicture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    int i;
    LCD_Address_Set(x1, y1, x2, y2);
    for (i = 0; i < 1600; i++) 
    { 	
        LCD_WR_DATA8(image[i * 2 + 1]);	 
        LCD_WR_DATA8(image[i * 2]);			
    }			
}
