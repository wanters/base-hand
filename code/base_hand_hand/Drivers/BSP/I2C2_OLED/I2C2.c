#include "I2C2.h"


 void I2C2_init(void)
{
	SDA2_1;
	SCL2_1;
}

 void I2C2_start()
{
	SDA2_OUT();
	SDA2_1;
	SCL2_1;
	delay_us(1);
	SDA2_0;
	delay_us(1);
	SCL2_0;
	I2C2_SendByte(0x78);
}

void I2C2_stop()
{
	SDA2_OUT();
	SDA2_0;
	delay_us(1);
	SCL2_1;
	delay_us(1);
	SDA2_1;
}
void I2C2_SendByte(unsigned char message)
{
	uint8_t i;
	SDA2_OUT();
	for(i=0;i<8;i++)
	{
		SDA2_level((message&0x80)>> 7);
		message <<= 1;
		delay_us(1);
		SCL2_1;
		delay_us(1);
		SCL2_0;
		delay_us(1);
	}
	I2C2_wait_ack();
}

uint8_t I2C2_wait_ack(void)
{
	uint8_t errtim = 0;
	SDA2_IN();
	SDA2_1;
	delay_us(1);
	SCL2_1;
	delay_us(1);
	while(READ_SDA2)
	{
		errtim++;
		if(errtim > 250)
		{
			I2C2_stop();
			return 1;
		}
	}
	SCL2_0;
	delay_us(1);
	return 0;
}

void SDA2_level(uint8_t level)
{
	if(level == 0)
	SDA2_0;
	if(level == 1)
	SDA2_1;
}
// SDA输入模式和输出模式
void SDA2_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = OLED_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OLED_SDA_Port, &GPIO_InitStruct);
}
void SDA2_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = OLED_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OLED_SDA_Port, &GPIO_InitStruct);
}
