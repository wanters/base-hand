#include "stm32f4xx_hal.h"
#include "I2C2.h"

#define OLED_COLUMN_NUMBER 128
#define OLED_LINE_NUMBER 64
#define OLED_COLUMN_OFFSET 2
#define OLED_PAGE_NUMBER OLED_LINE_NUMBER/8

#define CLEAR_TARGET_INFO	-1

typedef struct
{
	float lat;
	float lon;
	int status;
}GPS_INFO_T;

enum
{
	TYPE_ELEC,
	TYPE_DISTURB,
	TYPE_TARGET_INFO,
	TYPE_OPERATION,
	TYPE_DISTANCE,
	TYPE_TEMPERATURE,
	TYPE_GPS,
};

enum
{
	FIRST_SCREEN,
	SECOND_SCREEN,
};

extern uint32_t battery_time;
extern uint32_t switch_screen_time;

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

void oled_show_disturb_state(void);
void oled_show_tempeture(void);
void oled_show_distance(unsigned char * distence);

/*
* 电池相关
*/
void oled_show_battery_status(void);
void oled_twinkle_battery(uint8_t status);

/*
* 第一屏内容
*/
void oled_show_operation_status(uint8_t status);
void oled_show_target_info(int16_t azimuth);
void oled_clear_target_info(void);

/*
* 第二屏内容
*/
void oled_show_gps_info(GPS_INFO_T *gps_info);
void oled_clear_gps_info(void);

void oled_display_control(uint8_t which, uint8_t type, void *op);
void oled_display_style(void);
void oled_display_style_test(uint8_t screen);
