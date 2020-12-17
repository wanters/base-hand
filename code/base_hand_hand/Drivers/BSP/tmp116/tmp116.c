#include "tmp116.h"
#include "I2C1_T.h"
#include "usart.h"
#include "stdio.h"
#include "adc.h"

extern uint8_t u_buf[256];
extern uint8_t rx_buf[5];

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
	I2C1_SendByte(0x92);  //д
	I2C1_wait_ack();
	I2C1_SendByte(0X00);  //�����¶ȼĴ���
	I2C1_wait_ack();
	I2C1_start();
	I2C1_SendByte(0x93);  //��
	I2C1_wait_ack();
	temperature_msb = I2C1_ReadByte(1);
	temperature_lsb = I2C1_ReadByte(1);
	I2C1_stop();
	ST = (temperature_msb<<8) + temperature_lsb;
	tmp0 = (*(int16_t*)&ST) / 128.0;
}

//��������tmp116
void Continuous(void)
{
	I2C1_start();
	I2C1_SendByte(0x92);  //д
	I2C1_wait_ack();
	I2C1_SendByte(0X01);  //�������üĴ���
	I2C1_wait_ack();
	I2C1_SendByte(0x02);
	I2C1_wait_ack();
	I2C1_SendByte(0x20);  //CCģʽ������8��,��ʱ1�롣
	I2C1_wait_ack();
	I2C1_stop();		
}

float gettmp116(void)
{
	return tmp0;
}

