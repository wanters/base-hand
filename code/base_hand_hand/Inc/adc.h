/**
  ******************************************************************************
  * File Name          : ADC.h
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
typedef enum
{
	A1_V,
	A2_V,
	A3_V,
	A4_V,
	A1_I,
	A2_I,
	A3_I,
	A4_I,
	POWER_V,
}MONITOR_POINT;

typedef struct
{
	MONITOR_POINT point;
	uint32_t adc_channel;
	uint16_t adc_value;
	float result_i;
	float result_v;
}MONITOR_IV;
/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle);
uint16_t GetADC_Average(uint32_t channel,uint8_t times);
uint16_t GetADC(uint32_t channel);
void monitor_adc(uint8_t times);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
