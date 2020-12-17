#include "gpio.h"
#include "delay.h"
//#include "debug.h"
//#include "lwip/debug.h"


#define SCL1_0    HAL_GPIO_WritePin(T_I2C1_SCL_Port, T_I2C1_SCL_Pin, GPIO_PIN_RESET);
#define SCL1_1    HAL_GPIO_WritePin(T_I2C1_SCL_Port, T_I2C1_SCL_Pin, GPIO_PIN_SET);

#define SDA1_0    HAL_GPIO_WritePin(T_I2C1_SDA_Port, T_I2C1_SDA_Pin, GPIO_PIN_RESET);
#define SDA1_1    HAL_GPIO_WritePin(T_I2C1_SDA_Port, T_I2C1_SDA_Pin, GPIO_PIN_SET);

#define READ_SDA1     HAL_GPIO_ReadPin(T_I2C1_SDA_Port, T_I2C1_SDA_Pin)


void I2C1_init(void);
void I2C1_start(void);
void I2C1_stop(void);
uint8_t I2C1_wait_ack(void);
void I2C1_ack(void);
void I2C1_nack(void);
void I2C1_SendByte(uint8_t message);
uint8_t I2C1_ReadByte(unsigned char ack);

void SDA1_IN(void);
void SDA1_OUT(void);
void SDA1_level(uint8_t level);

