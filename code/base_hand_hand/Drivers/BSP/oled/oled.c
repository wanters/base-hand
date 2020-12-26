
/************************************************************************************
* 
* Description:128*64点阵的OLED显示屏驱动文件，仅适用于惠特自动化(heltec.taobao.com)的SD1306驱动IIC通信方式显示屏
*
* Others: none;
*
* Function List:
* 1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
* 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
* 4. void WriteDat(unsigned char I2C_Data) -- 写数据
* 5. void OLED_Init(void) -- OLED屏初始化
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
* 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
* 8. void OLED_CLS(void) -- 清屏
* 9. void OLED_ON(void) -- 唤醒
* 10. void OLED_OFF(void) -- 睡眠
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
*
* History: none;
*
*************************************************************************************/

#include "I2C2.h"
#include "delay.h"
#include "oledfont.h"
#include "adc.h"
#include "oled.h"
#include "tmp116.h"
#include "stdio.h"
#include "string.h"
#include "communication.h"
#include "key_switch.h"

#define BATTERY_TIME	1000
#define SWITCH_SCREEN_TIME	3000

enum
{
	ELEC_0,
	ELEC_25,
	ELEC_50,
	ELEC_75,
	ELEC_100
};

char buff[50];
uint8_t twinkle_flag = 0;
uint32_t battery_low = 0;
uint32_t battery_time = 0;
uint8_t cur_which = 0;
uint8_t battery_low_beep = 0;
uint32_t switch_screen_time = 0;
uint32_t switch_screen_flag = 0;

extern MONITOR_IV monitor_iv[];

void WriteCmd(unsigned char Command)//写命令
{
	I2C2_start();
	I2C2_SendByte(0x00);
	I2C2_SendByte(Command);
	I2C2_stop();
}
void WriteDat(unsigned char Data)//写数据
{
	I2C2_start();
	I2C2_SendByte(0x40);
	I2C2_SendByte(Data);
	I2C2_stop();
}

void OLED_Init(void)
{
	delay_ms(100); //这里的延时很重要
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<9;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<130;n++)
		{
			WriteDat(fill_Data);
		}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

/*
* 干扰模块状态
*/
void oled_show_disturb_state(void)
{
	if(monitor_iv[A1_V].result_v < 5)
	{
		OLED_ShowStr(0,0,(unsigned char*)&"A1:OFF",2);
	}
	if(monitor_iv[A1_V].result_v > 20)
	{
		OLED_ShowStr(0,0,(unsigned char*)&"A1: ON",2);
	}
	
	if(monitor_iv[A2_V].result_v < 5)
	{
		OLED_ShowStr(0,2,(unsigned char*)&"A2:OFF",2);
	}
	if(monitor_iv[A2_V].result_v > 20)
	{
		OLED_ShowStr(0,2,(unsigned char*)&"A2: ON",2);
	}
	
	if(monitor_iv[A3_V].result_v < 5)
	{
		OLED_ShowStr(0,4,(unsigned char*)&"A3:OFF",2);
	}
	if(monitor_iv[A3_V].result_v > 20)
	{
		OLED_ShowStr(0,4,(unsigned char*)&"A3: ON",2);
	}
	
	if(monitor_iv[A4_V].result_v < 5)
	{
		OLED_ShowStr(0,6,(unsigned char*)&"A4:OFF",2);
	}
	if(monitor_iv[A4_V].result_v > 20)
	{
		OLED_ShowStr(0,6,(unsigned char*)&"A4: ON",2);
	}
}

int oled_get_battery_status(void)
{
	float elec;
	/*
	* 100%电量
	*/
	if((monitor_iv[POWER_V].result_v+0.35f)>=29.06f)
	{
		return ELEC_100;
	}
	
	/*
	* 0~100%
	*/
	if((monitor_iv[POWER_V].result_v+0.35f)<29.06f || \
	   (monitor_iv[POWER_V].result_v+0.35f)>22.2f)
	{
		elec = 100.0f*(monitor_iv[POWER_V].result_v+0.35f-22.2f)/7.0f;
		
		if(elec > 75)
		{
			return ELEC_75;
		}
		else if(elec > 50)
		{
			return ELEC_50;
		}
		else if(elec > 25)
		{
			return ELEC_25;
		}
		else
		{
			return ELEC_0;
		}
	}
	else
	{
		return ELEC_0;
	}	
}

void oled_show_battery_status(void)
{
	int elec_percent;
	
	if(battery_time >= BATTERY_TIME)
	{
		/* 清空计时时间 */
		battery_time = 0;
		
		/* 清空电量状态	*/
		battery_low = 0;
		
		/* 查看电量 */
		elec_percent = oled_get_battery_status();
		
		//elec_percent = ELEC_0;
		switch(elec_percent)
		{
			case ELEC_100:
				OLED_DrawBMP(90,0,124,3,bmp_elec100);
			break;
			case ELEC_75:
				OLED_DrawBMP(90,0,124,3,bmp_elec75);
			break;		
			case ELEC_50:
				OLED_DrawBMP(90,0,124,3,bmp_elec50);
			break;
			case ELEC_25:
				OLED_DrawBMP(90,0,124,3,bmp_elec25);
			break;			
			default:
				battery_low = 1;
			break;		
		}
		
		/*
		* 当电量低于25%时，置位beep标志
		*/
		if(elec_percent == ELEC_0)
		{
			if(battery_low_beep == 0)
			{
				battery_low_beep = 1;
			}
		}
		else
		{
			/*
			* 确认已经响完一声后，再清除beep标志
			*/
			if(battery_low_beep == 3)
			{
				battery_low_beep = 0;
			}
		}
		
		/*
		* 蜂鸣器鸣叫
		*/
		if(battery_low_beep == 1)
		{
			beep_on();
			battery_low_beep = 2;
		}
		else if(battery_low_beep == 2)
		{
			beep_off();
			battery_low_beep = 3;
		}

		if(battery_low != 0)
		{
			if(twinkle_flag == 0)
			{
				oled_twinkle_battery(0);
				twinkle_flag = 1;
			}
			else
			{
				twinkle_flag = 0;
				oled_twinkle_battery(1);
			}
		}
	}
}

/*
* 电池闪烁
*/
void oled_twinkle_battery(uint8_t status)
{
	if(status == 0)
	{
		OLED_DrawBMP(90,0,124,3,bmp_elec0);
	}
	if(status == 1)
	{
		OLED_DrawBMP(90,0,124,3,bmp_elec_empty);
	}
}

/*
* 显示目标信息
*/
void oled_show_target_info(int16_t azimuth)
{
	if((azimuth<=360) && (azimuth>=0))
	{
		if(azimuth<=10 || azimuth>=350)
		{
			OLED_ShowCN(40,3,3);
			OLED_ShowCN(57,3,4);
		}
		if(azimuth>10 & azimuth<80)
		{
			OLED_ShowCN(40,3,0);
			OLED_ShowCN(57,3,3);
		}
		if(azimuth>=80 && azimuth<=100)
		{
			OLED_ShowCN(40,3,0);
			OLED_ShowCN(57,3,4);
		}
		if(azimuth>100 && azimuth<170)
		{
			OLED_ShowCN(40,3,0);
			OLED_ShowCN(57,3,1);
		}
		if(azimuth>=170 && azimuth<=190)
		{
			OLED_ShowCN(40,3,1);
			OLED_ShowCN(57,3,4);
		}
		if(azimuth>190 && azimuth<260)
		{
			OLED_ShowCN(40,3,2);
			OLED_ShowCN(57,3,1);
		}
		if(azimuth>=260 && azimuth<280)
		{
			OLED_ShowCN(40,3,2);
			OLED_ShowCN(57,3,4);
		}
		if(azimuth>280 && azimuth<350)
		{
			OLED_ShowCN(40,3,2);
			OLED_ShowCN(57,3,3);
		}
		
		sprintf(buff, "%3d", azimuth);
		OLED_ShowCN(83,3,4);
		OLED_ShowCN(100,3,4);
		OLED_ShowStr(83, 3, (unsigned char*)&buff, 2);
	}
	else
	{
		oled_clear_target_info();
	}
}

/*
* 清空目标信息
*/
void oled_clear_target_info(void)
{
	OLED_ShowCN(40, 3, 4);
	OLED_ShowCN(57, 3, 4);
	OLED_ShowCN(74, 3, 4);
	OLED_ShowCN(91, 3, 4);
	OLED_ShowCN(0,  0, 4);
	OLED_ShowCN(15, 0, 4);
	OLED_ShowCN(30, 0, 4);
	OLED_ShowCN(42, 0, 4);
}

/*
* 显示操作状态
*/
void oled_show_operation_status(uint8_t status)
{
	if(status == 0)
	{
		/*
			驱离
		*/
		OLED_ShowCN(40, 0, 10);
		OLED_ShowCN(57, 0, 11);
	}
	else if(status == 1)
	{
		/*
			迫降
		*/
		OLED_ShowCN(40, 0, 12);
		OLED_ShowCN(57, 0, 13);
	}
	else
	{
		/*
			清空
		*/
		OLED_ShowCN(40, 0, 4);
		OLED_ShowCN(57, 0, 4);
	}
}

/*
* 显示温度
*/
void oled_show_tempeture(void)
{
	//sprintf(buff, ":%3.2f", gettmp116());
	sprintf(buff, ":%3.2f", -35.678);
	OLED_ShowStr(35, 0, (unsigned char*)&buff,2);
	OLED_ShowCN(0,   0, 5);
	OLED_ShowCN(17,  0, 6);
}

/*
* 显示距离
*/
void oled_show_distance(unsigned char * distence)
{
	OLED_ShowCN(42,0,7);
	OLED_ShowStr(2,0,distence,2);
}

void oled_show_gps_info(GPS_INFO_T *gps_info)
{
	if(gps_info == NULL)
	{
		/*
		* 清空gps相关内容
		*/
		OLED_ShowCN(0,   0, 4);
		OLED_ShowCN(17,  0, 4);
		sprintf(buff, "%s", "          ");
		OLED_ShowStr(0, 2, (unsigned char*)&buff, 2);
		sprintf(buff, "%s", "          ");
		OLED_ShowStr(0, 4, (unsigned char*)&buff, 2);
		
		return;
	}
	
	/*
	* 位置
	*/
	OLED_ShowCN(0,   0, 14);
	OLED_ShowCN(17,  0, 15);
	
	/*
	* 定位成功
	*/
	if(gps_info->status)
	{
		/* 显示经纬度 */
		sprintf(buff, "%3.6f", gps_info->lon);
		OLED_ShowStr(0, 2, (unsigned char*)&buff, 2);
		sprintf(buff, "%3.6f", gps_info->lat);	
		OLED_ShowStr(0, 4, (unsigned char*)&buff, 2);		
	}
	else
	{
		/* 显示未定位成功 */
		sprintf(buff, "%3.6f", 0.0f);
		OLED_ShowStr(0, 2, (unsigned char*)&buff, 2);
		sprintf(buff, "%3.6f", 0.0f);
		OLED_ShowStr(0, 4, (unsigned char*)&buff,2);
	}
}


/*
* 清空指定屏
*/
void oled_clear_which(uint8_t which)
{
	switch(which)
	{
		case FIRST_SCREEN:
			/* 清空操作状态 */
			oled_show_operation_status(2);
			/* 清空目标位置信息 */
			oled_clear_target_info();
		break;
		
		case SECOND_SCREEN:
			/* 清空gps信息 */
			oled_show_gps_info(NULL);
		break;
		
		default:
			break;
	}
}

void oled_set_switch_which(uint8_t which)
{
	if(cur_which != which)
	{
		oled_clear_which(cur_which);
		cur_which = which;
	}
}

uint8_t oled_get_switch_which(void)
{
	return cur_which;
}

/*
* @brief	显示控制
*
* @param[in]	which	第几屏内容
* 				=0		第一屏
*				=1		第二屏
*
* @param[in]	type	类别
*				=0		电量
*				=1		目标位置信息
*				=2		干扰模块状态
*				=3		驱离/迫降状态
*				=4		距离
*				=5		温度
*				=6		gps
*
* @param[in]	op		操作
*				=NULL	不操作
*				=
*
* @return		无
*/
void oled_display_control(uint8_t which, uint8_t type, void *op)
{
	int16_t azimuth;
	uint8_t operation_status;
	GPS_INFO_T *gps_info;
	
	/*
	* 非当前显示，退出
	*/
	if(which != oled_get_switch_which())
	{
		return;
	}
	
	/*
	* 显示相应内容
	*/
	switch(type)
	{
		case TYPE_TARGET_INFO:
			azimuth = *((int16_t*)op);
			oled_show_target_info(azimuth);
		break;
		
		case TYPE_OPERATION:
			operation_status = *((uint8_t*)op);
			oled_show_operation_status(operation_status);
		break;
		
		case TYPE_GPS:
			gps_info = (GPS_INFO_T*)op;
			oled_show_gps_info(gps_info);
		break;
		
		default:
			
		break;
	}
}

void oled_display_style(void)
{
	/*
	* 有网络命令或者干扰模块打开
	*/
	if((switch_screen_flag==1) && (control_status.net_beep	\
								  ||control_status.disturb_status))
	{
		oled_set_switch_which(FIRST_SCREEN);
	}
	else
	{
		switch_screen_flag = 0;
		oled_set_switch_which(SECOND_SCREEN);
	}
	
	if(switch_screen_time >= SWITCH_SCREEN_TIME)
	{
		switch_screen_time = 0;
		if(switch_screen_flag == 0)
		{
			switch_screen_flag = 1;		
		}
		else
		{
			switch_screen_flag = 0;
		}
	}
}	

void oled_display_style_test(uint8_t screen)
{
	control_status.net_beep = 1;
	
	oled_display_style();
}



