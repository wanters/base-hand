#include "I2C1_T.h"


//I2C1��ʼ��//
void I2C1_init(void)
{
	SDA1_1;
	SCL1_1;
}

//I2C1��ʼ�ź�//
void I2C1_start(void)
{
	SDA1_OUT();
	SDA1_1;
	SCL1_1;
	delay_us(2);
	SDA1_0;
	delay_us(2);
	SCL1_0;
}

//I2C4��ֹ�ź�//
void I2C1_stop()
{
	SDA1_OUT();
	SCL1_0;
	SDA1_0;
	delay_us(2);
	SCL1_1;
	delay_us(2);
	SDA1_1;
	delay_us(2);
}

//�ȴ�ACK
//����ֵ   0   ���ճɹ�
//         1   ����ʧ��
uint8_t I2C1_wait_ack(void)
{
	uint8_t errtim = 0;
	SDA1_IN();
	SDA1_1;
	delay_us(1);
	SCL1_1;
	delay_us(1);
	while(READ_SDA1)
	{
		errtim++;
		if(errtim > 250)
		{
			I2C1_stop();
			return 1;
		}
	}
	SCL1_0;
	delay_us(2);
	return 0;
}

//����ACK
void I2C1_ack()
{
	SCL1_0;
	SDA1_OUT();
	SDA1_0;
	delay_us(2);
	SCL1_1;
	delay_us(2);
	SCL1_0;
}

//������ACK
void I2C1_nack(void)
{
	SCL1_0;
	SDA1_OUT();
	SDA1_1;
	delay_us(2);
	SCL1_1;
	delay_us(2);
	SCL1_0;
}

//I2C4����һ���ֽ�
//��ack����1
//��ack����0
void I2C1_SendByte(uint8_t message)
{
	uint8_t i;
	SDA1_OUT();
//	SCL4_0;
	for(i=0;i<8;i++)
	{
		SDA1_level((message&0x80)>> 7);
		message <<= 1;
		delay_us(2);
		SCL1_1;
		delay_us(2);
		SCL1_0;
		delay_us(2);
	}
}

uint8_t I2C1_ReadByte(unsigned char ack)
{
	unsigned char i,receive = 0;
	SDA1_IN();
	for(i=0;i<8;i++)
	{
		SCL1_0;
		delay_us(1);
		SCL1_1;
		receive <<= 1;
		if(READ_SDA1)
		{
			receive++;
		}
		delay_us(1);
	}
	if(ack)
	{
		I2C1_ack();
	}
	else
	{
		I2C1_nack();
	}
	return receive;
}


//sda1�����ƽ
void SDA1_level(uint8_t level)
{
	if(level == 0)
	SDA1_0;
	if(level == 1)
	SDA1_1;
}

// SDA����ģʽ�����ģʽ
void SDA1_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = T_I2C1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void SDA1_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = T_I2C1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
