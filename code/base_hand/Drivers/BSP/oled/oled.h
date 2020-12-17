#include "stm32f4xx_hal.h"
#include "I2C2.h"

#define OLED_COLUMN_NUMBER 128
#define OLED_LINE_NUMBER 64
#define OLED_COLUMN_OFFSET 2
#define OLED_PAGE_NUMBER OLED_LINE_NUMBER/8

void WriteCmd(unsigned char Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void oled_show_disturbe_state(void);
void quan_of_elec(void);
void oled_show_location(uint16_t azimuth);
//void angle(uint8_t angle);
void show_tempeture(void);
void show_distence(unsigned char * distence);
void alarm_release(void);
void elec_twinkle(uint8_t s);
void oled_show_status(uint8_t status);
