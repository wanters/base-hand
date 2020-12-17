/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : date.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 12/1/2009
* Description        : 日期相关函数
*******************************************************************************/


#include "stm32f4xx.h"

extern struct GNRMC_Info GNRMC;

uint32_t mktimev(struct GNRMC_Info *tm);
void to_tm(uint32_t tim, struct GNRMC_Info * tm);

