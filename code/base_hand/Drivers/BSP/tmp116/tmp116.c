#include "tmp116.h"
#include "I2C1_T.h"
#include "usart.h"
#include "stdio.h"
#include "adc.h"

extern uint8_t u_buf[256];
extern uint8_t rx_buf[5];
extern MONITOR_IV monitor_iv[];

float tmp0;

void I2C1_ResetAll(void)
{
	I2C1_init();
	I2C1_start();
	I2C1_SendByte(0x00);
	I2C1_wait_ack();
	I2C1_SendByte(0x06);
	I2C1_wait_ack();
	I2C1_stop();
	delay_ms(50);
}

void GetTemperature(void)
{
	uint16_t temperature_msb,temperature_lsb,ST;
	
	I2C1_start();
	I2C1_SendByte(0x92);  //Ð´
	I2C1_wait_ack();
	I2C1_SendByte(0X00);  //·ÃÎÊÎÂ¶È¼Ä´æÆ÷
	I2C1_wait_ack();
	I2C1_start();
	I2C1_SendByte(0x93);  //¶Á
	I2C1_wait_ack();
	temperature_msb = I2C1_ReadByte(1);
	temperature_lsb = I2C1_ReadByte(1);
	I2C1_stop();
	ST = (temperature_msb<<8) + temperature_lsb;
	tmp0 = (*(int16_t*)&ST) / 128.0;
}

//ÅäÖÃËùÓÐtmp116
void Continuous(void)
{
	I2C1_start();
	I2C1_SendByte(0x92);  //Ð´
	I2C1_wait_ack();
	I2C1_SendByte(0X01);  //·ÃÎÊÅäÖÃ¼Ä´æÆ÷
	I2C1_wait_ack();
	I2C1_SendByte(0x02);
	I2C1_wait_ack();
	I2C1_SendByte(0x20);  //CCÄ£Ê½£¬²âÁ¿8´Î,ºÄÊ±1Ãë¡£
	I2C1_wait_ack();
	I2C1_stop();		
}

float gettmp116(void)
{
	return tmp0;
}

void force_shutdown(void)
{
	if(gettmp116()>70 || (monitor_iv[POWER_V].result_v<=(22.41+0.35)))
	{
		HAL_GPIO_WritePin(SWITCH1_GPIO_Port, SWITCH1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SWITCH2_GPIO_Port, SWITCH2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SWITCH3_GPIO_Port, SWITCH3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SWITCH4_GPIO_Port, SWITCH4_Pin, GPIO_PIN_RESET);
	}
}
