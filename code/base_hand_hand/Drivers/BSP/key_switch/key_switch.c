/**
  ******************************************************************************
  * File Name          : key_switch.c
  * Description        : 
  *	  - ����ģ�鿪��
  *   - �������ؿ���
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 BHKY.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "key_switch.h"
#include "debug.h"
#include "gpio.h"
#include "communication.h"
#include "oled.h"
#include "tmp116.h"
#include "adc.h"

 
#define TEMP_LIMIT				60	    	//�¶ȱ���
#define SWITCH_BEEP_ON_TIME_1S		500	//ģ��򿪺󣬷��������м��
#define SWITCH_BEEP_OFF_TIME_1S		3000	//ģ��򿪺󣬷��������м��
#define NET_BEEP_TIME_500MS		500		//�յ���������󣬷��������м��
//#define ON_TIME_30S				30000	    //ģ�鿪30s
//#define OFF_TIME_60S			60000	    //ģ���60s
#define ON_TIME_5M				300000   	//ģ�鿪5M
#define OFF_TIME_15M			900000	  //ģ���15M

#define NO_NET_TIME_20S			20000	//����������ʱ��

extern MONITOR_IV monitor_iv[];

uint32_t pd1_time;
uint32_t beep_time;
uint8_t beep_on_off=0;

SW_CHANNEL distrub_sw_channel[] = {
	{SWITCH1_GPIO_Port, SWITCH1_Pin},
	{SWITCH2_GPIO_Port, SWITCH2_Pin},	
	{SWITCH3_GPIO_Port, SWITCH3_Pin},
	{SWITCH4_GPIO_Port, SWITCH4_Pin},			
};

DISTURB_KEY_CHANNEL distrub_key_channel[] = {
	{KEY1_Port, KEY1_Pin},
	{KEY2_Port, KEY2_Pin},
	{KEY3_Port, KEY3_Pin},
	{KEY4_Port, KEY4_Pin},		
};

uint8_t disturb_switch_status(DISTURB_SW disturb_sw)
{
	return HAL_GPIO_ReadPin(distrub_sw_channel[disturb_sw].GPIOx, distrub_sw_channel[disturb_sw].GPIO_Pin);
}

void disturb_switch_control(DISTURB_SW disturb_sw, SW_STATUS status)
{
	if(disturb_sw >= DISTURB_SW_MAX)
		return;
	
	if(status == SW_ON)
	{
		HAL_GPIO_WritePin(distrub_sw_channel[disturb_sw].GPIOx, distrub_sw_channel[disturb_sw].GPIO_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(distrub_sw_channel[disturb_sw].GPIOx, distrub_sw_channel[disturb_sw].GPIO_Pin, GPIO_PIN_RESET);
	}
}

GPIO_PinState get_key_status(DISTURB_SW disturb_sw)
{
	return HAL_GPIO_ReadPin(distrub_key_channel[disturb_sw].GPIOx, distrub_key_channel[disturb_sw].GPIO_Pin);
}

void toggle_distrub_sw(DISTURB_SW disturb_sw)
{
	distrub_sw_channel[disturb_sw].disturb_time = 0;
	
	if(gettmp116() < TEMP_LIMIT)
	{
		if(!distrub_sw_channel[disturb_sw].disturb_switch)
		{
			HAL_GPIO_WritePin(distrub_sw_channel[disturb_sw].GPIOx, distrub_sw_channel[disturb_sw].GPIO_Pin, GPIO_PIN_SET);
			distrub_sw_channel[disturb_sw].disturb_switch = SW_ON;
			distrub_sw_channel[disturb_sw].on_off = SW_ON;
		}
		else
		{
			HAL_GPIO_WritePin(distrub_sw_channel[disturb_sw].GPIOx, distrub_sw_channel[disturb_sw].GPIO_Pin, GPIO_PIN_RESET);
			distrub_sw_channel[disturb_sw].disturb_switch = SW_OFF;
			distrub_sw_channel[disturb_sw].on_off = SW_OFF;
		}
	}
}

void disturb_key_control(void)
{
	uint8_t disturb_sw;
	
	for(disturb_sw=0; disturb_sw<DISTURB_SW_MAX; disturb_sw++)
	{
		/*��ȡ����״̬*/
		if(get_key_status(disturb_sw) && (distrub_sw_channel[disturb_sw].change_enble==1))
		{
			/*��ת���ſ���*/
			toggle_distrub_sw(disturb_sw);
			distrub_sw_channel[disturb_sw].change_enble = 0;
			//TRACE_INFO("key channel <%d> press down\n", disturb_sw);
		}
		else if(!get_key_status(disturb_sw) && (distrub_sw_channel[disturb_sw].change_enble==0))
		{
			distrub_sw_channel[disturb_sw].change_enble = 1;
			//TRACE_INFO("key channel <%d> press up\n", disturb_sw);
		}
	}
}

uint8_t disturb_status_check(SW_CHANNEL *distrub_sw_channel)
{
	uint8_t disturb_count;
	uint8_t disturb_flag = 0;
	/*
		��ģ�鿪������PD1��������ߵ͵�ƽ
	*/
	/*
		����ģ�鿪
	*/	
	for(disturb_count=0; disturb_count<DISTURB_SW_MAX-1; disturb_count++)
	{
		if(distrub_sw_channel[disturb_count].disturb_switch == SW_ON)
		{
			disturb_flag = 1;
		}
	}
	
	/*
		����ģ�鿪��GPS��
	*/
	if(distrub_sw_channel[3].disturb_switch==SW_ON && disturb_flag==1)
	{
		disturb_flag = 2;
	}
	else if(distrub_sw_channel[3].disturb_switch == SW_ON)
	{
		/*
			����ģ��أ�����GPS
		*/
		disturb_flag = 3;
	}
	
	return disturb_flag;
}

void clear_pd1_time(void)
{
	pd1_time = 0;
}

uint32_t get_pd1_time(void)
{
	return pd1_time;
}

void set_pd1_time_plus1(void)
{
	pd1_time++;
}

void clear_sw_time(DISTURB_SW disturb_sw)
{
	distrub_sw_channel[disturb_sw].disturb_time = 0;
}

uint32_t get_sw_time(DISTURB_SW disturb_sw)
{
	return distrub_sw_channel[disturb_sw].disturb_time;
}

void set_sw_time_plus1(DISTURB_SW disturb_sw)
{
	distrub_sw_channel[disturb_sw].disturb_time++;
}

void pd1_period_out(void)
{
	uint8_t disturb_status;
	
	disturb_status = disturb_status_check(distrub_sw_channel);
	if(disturb_status)
	{
		/*
			��ģ���
		*/
		if(get_pd1_time()>SWITCH_BEEP_OFF_TIME_1S && beep_on_off==0)	//�����ر�ʱ���
		{
			beep_on_off = 1;
			clear_pd1_time();			
			HAL_GPIO_WritePin(PD_Port, PD_Pin, GPIO_PIN_SET);
		}
		else if(get_pd1_time()>SWITCH_BEEP_ON_TIME_1S && beep_on_off==1)//������ʱ��ر�
		{
			beep_on_off = 0;
			clear_pd1_time();
			HAL_GPIO_WritePin(PD_Port, PD_Pin, GPIO_PIN_RESET);			
		}
		
		/*
			ģ��+GPS��
		*/
		if(disturb_status == 2)
		{
			oled_show_status(1);
		}
		else if(disturb_status == 1)
		{
			/*
				ģ�鿪
			*/
			oled_show_status(0);
		}
		else
		{
			oled_show_status(2);
		}
	}
	else
	{
		beep_on_off = 0;
		/*
			�յ����������С��20s�������������
		*/	
		if((control_status.net_beep==1) && (beep_time<NO_NET_TIME_20S))
		{
			if(get_pd1_time() > NET_BEEP_TIME_500MS)
			{
				//printf("pd1 toggle 5\n");
				clear_pd1_time();
				HAL_GPIO_TogglePin(PD_Port, PD_Pin);
			}	
		}	
		else if(control_status.net_beep == 1)
		{
			/*
				�й���������
			*/
			beep_time = 0;
			control_status.net_beep = 0;
			HAL_GPIO_WritePin(PD_Port, PD_Pin, GPIO_PIN_RESET);
			alarm_release();
		}
		else
		{
			HAL_GPIO_WritePin(PD_Port, PD_Pin, GPIO_PIN_RESET);	
		}
		/*
			���״̬
		*/
		oled_show_status(2);
	}
}

void disturb_switch_style(void)
{
	uint8_t disturb_count;
	
	/*
		ģ��򿪣����趨ʱ�䷴ת
	*/
	for(disturb_count=0; disturb_count<DISTURB_SW_MAX; disturb_count++)
	{
		if(distrub_sw_channel[disturb_count].disturb_switch == SW_ON)
		{
			if(distrub_sw_channel[disturb_count].on_off)
			{
				/*
					��ʱ��
				*/
				if(get_sw_time(disturb_count) >= ON_TIME_5M)//10000
				{
					distrub_sw_channel[disturb_count].on_off = SW_OFF;
					clear_sw_time(disturb_count);
					//HAL_GPIO_TogglePin(distrub_sw_channel[disturb_count].GPIOx, distrub_sw_channel[disturb_count].GPIO_Pin);				
					HAL_GPIO_WritePin(distrub_sw_channel[disturb_count].GPIOx, distrub_sw_channel[disturb_count].GPIO_Pin, GPIO_PIN_RESET);
				}
			}
			else
			{
				/*
					��ʱ��
				*/				
				if(get_sw_time(disturb_count) >= OFF_TIME_15M)//10000
				{
					distrub_sw_channel[disturb_count].on_off = SW_ON;
					clear_sw_time(disturb_count);
					//HAL_GPIO_TogglePin(distrub_sw_channel[disturb_count].GPIOx, distrub_sw_channel[disturb_count].GPIO_Pin);				
					HAL_GPIO_WritePin(distrub_sw_channel[disturb_count].GPIOx, distrub_sw_channel[disturb_count].GPIO_Pin, GPIO_PIN_SET);
				}				
			}
		}
	}
}


void force_shutdown(void)
{
	uint8_t disturb_count;
	
	//if(gettmp116()>TEMP_LIMIT || (monitor_iv[POWER_V].result_v<=(22.41+0.35)))
	if(gettmp116() > TEMP_LIMIT)
	{
		HAL_GPIO_WritePin(SWITCH1_GPIO_Port, SWITCH1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SWITCH2_GPIO_Port, SWITCH2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SWITCH3_GPIO_Port, SWITCH3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SWITCH4_GPIO_Port, SWITCH4_Pin, GPIO_PIN_RESET);
		
		for(disturb_count=0; disturb_count<DISTURB_SW_MAX; disturb_count++)
		{
			distrub_sw_channel[disturb_count].disturb_switch = SW_OFF;
			distrub_sw_channel[disturb_count].on_off = SW_OFF;
		}
	}
}
