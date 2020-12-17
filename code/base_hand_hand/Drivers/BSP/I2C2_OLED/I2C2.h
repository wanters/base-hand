#include "gpio.h"
#include "delay.h"
//#include "debug.h"
//#include "lwip/debug.h"


#define SCL2_0    HAL_GPIO_WritePin(OLED_SCL_Port, OLED_SCL_Pin, GPIO_PIN_RESET);
#define SCL2_1    HAL_GPIO_WritePin(OLED_SCL_Port, OLED_SCL_Pin, GPIO_PIN_SET);

#define SDA2_0    HAL_GPIO_WritePin(OLED_SDA_Port, OLED_SDA_Pin, GPIO_PIN_RESET);
#define SDA2_1    HAL_GPIO_WritePin(OLED_SDA_Port, OLED_SDA_Pin, GPIO_PIN_SET);

#define READ_SDA2     HAL_GPIO_ReadPin(OLED_SDA_Port, OLED_SDA_Pin)

void I2C2_init(void);
void I2C2_start(void);
void I2C2_stop(void);
uint8_t I2C2_wait_ack(void);
void I2C2_SendByte(uint8_t message);
void SDA2_IN(void);
void SDA2_OUT(void);
void SDA2_level(uint8_t level);


//void I2C2_ack(void);
//void I2C2_nack(void);

//uint8_t I2C2_ReadByte(unsigned char ack);



