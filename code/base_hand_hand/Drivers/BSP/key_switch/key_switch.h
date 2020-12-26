
#ifndef __KEY_SWITCH_H__
#define __KEY_SWITCH_H__

#include "main.h"

typedef enum
{
	DISTURB_SW1,
	DISTURB_SW2,	
	DISTURB_SW3,
	DISTURB_SW4,	
	DISTURB_SW_MAX,
}DISTURB_SW;

typedef enum
{
	SW_OFF,	
	SW_ON,
}SW_STATUS;

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	uint8_t change_enble;
	uint32_t disturb_switch;	
	uint32_t disturb_time;
	uint8_t on_off;
}SW_CHANNEL;

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}DISTURB_KEY_CHANNEL;

extern uint32_t beep_time;

void disturb_key_control(void);
void disturb_switch_control(DISTURB_SW disturb_sw, SW_STATUS status);
uint8_t disturb_switch_status(DISTURB_SW disturb_sw);
uint8_t disturb_switch_status(DISTURB_SW disturb_sw);
void pd1_period_out(void);
void set_pd1_time_plus1(void);
void set_sw_time_plus1(DISTURB_SW disturb_sw);
void disturb_switch_style(void);
void force_shutdown(void);
void beep_off(void);
void beep_on(void);

#endif

