/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
	 
/*
	4路按键控制 
*/
#define SWITCH1_GPIO_Port      		GPIOE
#define SWITCH1_Pin      			GPIO_PIN_5
#define SWITCH2_GPIO_Port      		GPIOE
#define SWITCH2_Pin      			GPIO_PIN_4
#define SWITCH3_GPIO_Port      		GPIOE
#define SWITCH3_Pin      			GPIO_PIN_3
#define SWITCH4_GPIO_Port     	 	GPIOE
#define SWITCH4_Pin      			GPIO_PIN_2

#define KEY1_Port         			GPIOB
#define KEY1_Pin      				GPIO_PIN_14
#define KEY2_Port         			GPIOB
#define KEY2_Pin      				GPIO_PIN_15
#define KEY3_Port         			GPIOD
#define KEY3_Pin      				GPIO_PIN_8
#define KEY4_Port         			GPIOD
#define KEY4_Pin      				GPIO_PIN_9

#define PD_Port         			GPIOD
#define PD_Pin      				GPIO_PIN_1
#define BEEP_Port         			GPIOD
#define BEEP_Pin      				GPIO_PIN_2

/*

*/
#define MONITER_1V6_GPIO_Port     	GPIOA
#define MONITER_1V6_Pin      		GPIO_PIN_6
#define MONITER_1V4_GPIO_Port     	GPIOA
#define MONITER_1V4_Pin      		GPIO_PIN_4
#define MONITER_1V0_GPIO_Port     	GPIOA
#define MONITER_1V0_Pin      		GPIO_PIN_0
#define MONITER_1V10_GPIO_Port    	GPIOC
#define MONITER_1V10_Pin      		GPIO_PIN_0
#define MONITER_1V8_GPIO_Port     	GPIOB
#define MONITER_1V8_Pin      		GPIO_PIN_0
#define MONITER_1V5_GPIO_Port    	GPIOA
#define MONITER_1V5_Pin      		GPIO_PIN_5
#define MONITER_1V3_GPIO_Port     	GPIOA
#define MONITER_1V3_Pin      		GPIO_PIN_3
#define MONITER_1V9_GPIO_Port     	GPIOB
#define MONITER_1V9_Pin      		GPIO_PIN_1

/*
	debug-uart
*/
#define UART1_TX_Port             	GPIOA
#define UART1_TX_Pin      			GPIO_PIN_9
#define UART1_RX_Port             	GPIOA
#define UART1_RX_Pin      			GPIO_PIN_10

/*
	tmp116-i2c
*/
#define T_I2C1_SDA_Port           	GPIOB
#define T_I2C1_SDA_Pin      		GPIO_PIN_7
#define T_I2C1_SCL_Port           	GPIOB
#define T_I2C1_SCL_Pin      		GPIO_PIN_6

#define USB_DP_Port             	GPIOA
#define USB_DP_Pin      			GPIO_PIN_12
#define USB_DM_Port            	 	GPIOA
#define USB_DM_Pin      			GPIO_PIN_11

/*
	gps-uart
*/
#define GPS_TX_Port             	GPIOC
#define GPS_TX_Pin      			GPIO_PIN_10
#define GPS_RX_Port            	 	GPIOC
#define GPS_RX_Pin      			GPIO_PIN_11

/*
	oled-i2c
*/
#define OLED_SDA_Port             	GPIOC
#define OLED_SDA_Pin      			GPIO_PIN_9
#define OLED_SCL_Port            	GPIOA
#define OLED_SCL_Pin      			GPIO_PIN_8

/*
	wireless-e22
*/
#define WIRELESS_TX_Port          	GPIOD
#define WIRELESS_TX_Pin      		GPIO_PIN_5
#define WIRELESS_RX_Port          	GPIOD
#define WIRELESS_RX_Pin      		GPIO_PIN_6
#define WIRELESS_AUX_Port         	GPIOD
#define WIRELESS_AUX_Pin      		GPIO_PIN_7
#define WIRELESS_M0_Port          	GPIOD
#define WIRELESS_M0_Pin      		GPIO_PIN_3
#define WIRELESS_M1_Port          	GPIOD
#define WIRELESS_M1_Pin      		GPIO_PIN_4

/*
	网路驱动
*/
#define ETH_MDIO_Port             GPIOA
#define ETH_MDIO_Pin              GPIO_PIN_2
#define ETH_MDIC_Port             GPIOC
#define ETH_MDIC_Pin              GPIO_PIN_1
#define ETH_TXD0_Port             GPIOB
#define ETH_TXD0_Pin              GPIO_PIN_12
#define ETH_TXD1_Port             GPIOB
#define ETH_TXD1_Pin              GPIO_PIN_13
#define ETH_TXD_EN_Port           GPIOB
#define ETH_TXD_EN_Pin            GPIO_PIN_11
#define ETH_RXD0_Port             GPIOC
#define ETH_RXD0_Pin              GPIO_PIN_4
#define ETH_RXD1_Port             GPIOC
#define ETH_RXD1_Pin              GPIO_PIN_5
#define ETH_CRS_DV_Port           GPIOA
#define ETH_CRS_DV_Pin            GPIO_PIN_7
#define WTH_REF_CLK_Port          GPIOA
#define ETH_REF_CLK_Pin           GPIO_PIN_1
#define ETH_RESET_Port            GPIOC
#define ETH_RESET_Pin             GPIO_PIN_3


/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
