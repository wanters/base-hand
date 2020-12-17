
#ifndef __DELAY_H__
#define __DELAY_H__

#include "stdint.h"

void delay_init(uint16_t sys_clk);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
#endif

