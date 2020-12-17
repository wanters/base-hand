/**
  ******************************************************************************
  * File Name          : ADC.c
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

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "delay.h"
/* USER CODE END 0 */

uint8_t monitor_channel = POWER_V;

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10
    PC2     ------> ADC1_IN12
    PA0/WKUP     ------> ADC1_IN0
    PA3     ------> ADC1_IN3
    PA4     ------> ADC1_IN4
    PA5     ------> ADC1_IN5
    PA6     ------> ADC1_IN6
    PB0     ------> ADC1_IN8
    PB1     ------> ADC1_IN9 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10
    PC2     ------> ADC1_IN12
    PA0/WKUP     ------> ADC1_IN0
    PA3     ------> ADC1_IN3
    PA4     ------> ADC1_IN4
    PA5     ------> ADC1_IN5
    PA6     ------> ADC1_IN6
    PB0     ------> ADC1_IN8
    PB1     ------> ADC1_IN9 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_2);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

MONITOR_IV monitor_iv[] = 
{
	{
		.point = A1_V,
		.adc_channel = ADC_CHANNEL_6,
	},
	{
		.point = A2_V,
		.adc_channel = ADC_CHANNEL_4,
	},
	{
		.point = A3_V,
		.adc_channel = ADC_CHANNEL_0,
	},
	{
		.point = A4_V,
		.adc_channel = ADC_CHANNEL_10,
	},
	{
		.point = A1_I,
		.adc_channel = ADC_CHANNEL_8,
	},
	{
		.point = A2_I,
		.adc_channel = ADC_CHANNEL_5,
	},
	{
		.point = A3_I,
		.adc_channel = ADC_CHANNEL_3,
	},
	{
		.point = A4_I,
		.adc_channel = ADC_CHANNEL_12,
	},
	{
		.point = POWER_V,
		.adc_channel = ADC_CHANNEL_9,
	}
};

uint16_t GetADC(uint32_t channel)
{
	ADC_ChannelConfTypeDef ADC1_ChanConf;
	
	ADC1_ChanConf.Channel = channel;
	ADC1_ChanConf.Rank = 1;
	ADC1_ChanConf.Offset = 0;
	ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	
	HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,10);
	return (uint16_t)HAL_ADC_GetValue(&hadc1);
}

uint16_t GetADC_Average(uint32_t channel,uint8_t times)
{
	uint32_t temp = 0;
	
	for(uint8_t i=0; i<times; i++)
	{
		temp += GetADC(channel);
		delay_ms(5);
	}
	return temp/times;
}

/*
	获取采集值
*/
void monitor_adc(uint8_t times)
{
	#if 0
	for(uint8_t i=0; i<=POWER_V; i++)
	{
		monitor_iv[i].adc_value = GetADC_Average(monitor_iv[i].adc_channel, times);
		if((i<=A4_V) || (i==POWER_V))
		{
			monitor_iv[i].result_v = 3.3*20.1*monitor_iv[i].adc_value/4095;
		}
		if((i>=A1_I) && (i<=A4_I))
		{
			monitor_iv[i].result_i = 3.3*2*monitor_iv[i].adc_value/4095 - 2.45;
		}
	}
	#else
		monitor_iv[monitor_channel].adc_value = GetADC_Average(monitor_iv[monitor_channel].adc_channel, times);
		if((monitor_channel<=A4_V) || (monitor_channel==POWER_V))
		{
			monitor_iv[monitor_channel].result_v = 3.3*20.1*monitor_iv[monitor_channel].adc_value/4095;
		}
		if((monitor_channel>=A1_I) && (monitor_channel<=A4_I))
		{
			monitor_iv[monitor_channel].result_i = 3.3*2*monitor_iv[monitor_channel].adc_value/4095 - 2.45;
		}
		monitor_channel++;
		if(monitor_channel > POWER_V)
		{
			monitor_channel = 0;
		}
	#endif
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
