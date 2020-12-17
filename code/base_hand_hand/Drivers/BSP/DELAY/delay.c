/**
  ******************************************************************************
  * File Name          : delay.c
  * Description        : 
  *   Common delay functions:
  *	  - delay_init:��ʱ��ʼ��
  *   - delay_ms:������ʱ
  *   - delay_us:΢����ʱ
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 BHKY.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "delay.h"

uint32_t fac_us;
void delay_init(uint16_t sys_clk)
{
	fac_us = sys_clk;
}

//��ʱС�ڵ���1000us
void delay_lt1000_us(uint32_t nus)
{	
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload = SysTick->LOAD;			//LOAD��ֵ	    	 
	ticks = nus*fac_us; 						//��Ҫ�Ľ����� 
	told = SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{
			if(tnow < told)
			{
				tcnt += told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			}
			else
			{
				tcnt += reload-tnow+told;	
			}				
			told = tnow;
			if(tcnt >= ticks)
			{
				break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
			}
		}  
	}
}

void delay_ms(uint32_t ms)
{
    uint32_t i;
	
    for (i=0; i<ms; i++)
	{
        delay_lt1000_us(1000);
	}
}

void delay_us(uint32_t us)
{
    uint32_t time_us, time_ms;

	time_ms = us / 1000;
	time_us = (us % 1000);
	
	if (time_ms != 0)
	{
		delay_ms(time_ms);
	}
	
	if(time_us)
	{
		delay_lt1000_us(time_us);
	}
}

