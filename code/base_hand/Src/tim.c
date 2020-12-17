/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "lan8720.h"
#include "usart.h"
#include "udp_echoserver.h"
#include "wireless.h"
#include "gps.h"
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;

uint8_t tim_send_buffer[100];
uint8_t tim_send_length;
uint8_t current_hand_id = 0;
uint8_t tim_flag = 0;

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 9000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim2)
	{
		tim_flag = 1;
	}
}

void period_process_device_info(void)
{
	uint8_t wireless_command;
	
	if (tim_flag == 1)
	{
		tim_flag = 0;
		
		/*
			周期通知手持设备上报GPS信息
		*/
		if(control_status.report_hand_info == REPORT_ENABLE)
		{
			wireless_command = COMM_GPS_ENABLE;
			tim_send_buffer[0] = 0xFF;	/*广播*/
			tim_send_buffer[1] = 0xFF;
			tim_send_buffer[2] = wireless_info_t.channel;
			tim_send_length = 3;
			tim_send_buffer[tim_send_length] = '$';
			tim_send_length++;
			tim_send_buffer[tim_send_length] = 2;
			tim_send_length++;	
			tim_send_buffer[tim_send_length] = current_hand_id;
			tim_send_length += 1;
			tim_send_buffer[tim_send_length] = wireless_command;
			tim_send_length += 1;	
			tim_send_buffer[tim_send_length] = '#';
			tim_send_length += 1;	
			
			HAL_UART_Transmit(&E22_UART, tim_send_buffer, tim_send_length, 0xfff);
			printf("current_hand_id: %d\n", current_hand_id);
			current_hand_id++;
			if(current_hand_id >= HAND_MAX)
			{
				current_hand_id = 0;
			}
			
			#if 0
			printf("send config: \n");
			for(uint8_t i=0; i<tim_send_length; i++)
			{
				printf("%02X ", tim_send_buffer[i]);
			}
			printf("\n");
			#endif
		}

		/*
			周期上报手持设备信息给上位机
		*/
		if(control_status.report_hand_info == REPORT_ENABLE)
		{	
			#if 1
			if(control_status.hand_info_flag == REPORT_ENABLE)
			{
				udp_senddata(upcb, (char *)hand_info_to_net.buffer, hand_info_to_net.length);
				control_status.hand_info_flag = REPORT_DISABLE;
			}
			#endif
		}
		
		/*
			周期上报基站设备信息给上位机
		*/
		if(control_status.report_base_info == REPORT_ENABLE) 
		{
			udp_senddata(upcb, (char *)base_info_to_net.buffer, base_info_to_net.length);
		}
	}
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
