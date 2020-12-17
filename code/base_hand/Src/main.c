/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "lwip.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "tmp116.h"
#include "lan8720.h"
#include "oled.h"
#include "gps.h"
#include "wireless.h"
#include "string.h"
#include "udp_echoserver.h"
#include "key_switch.h"
#include "uart_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define TIME_1S 1000
int time_1s = 0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  
  delay_init(180);
	
#if 1  
  #if BASE_PROJECT
	MX_LWIP_Init();	
	MX_TIM2_Init();
	//LAN8720_RESET();
	udp_echoserver_init();
	HAL_TIM_Base_Start_IT(&htim2);
	//printf("[wireless addr]: %02X-%02X\n", BASE_ADDR_H, BASE_ADDR_L);
	printf("[base id]: %02X\n", BASE_DEVICE_ID);
  #else
	MX_ADC1_Init();
	delay_ms(50);
	I2C1_ResetAll();
	Continuous();
	OLED_Init();
	OLED_CLS();
	//printf("[wireless addr]: %02X-%02X\n", HAND_ADDR_H, HAND_ADDR_L);
	printf("[hand id]: %02X\n", HAND_DEVICE_ID);
  #endif
  
  HAL_UART_Receive_IT(&GPS_UART, &uart_recv_gps,  1);
  HAL_UART_Receive_IT(&E22_UART, &uart_recv_e22,   1);
  HAL_UART_Receive_IT(&DEBUG_UART, &uart_recv_debug, 1);  
  
  gps_init();
  wireless_init();
  uart_config_init();
  
  //e22_init_set();
  e22_init_get();
  printf("channel: %d\n", wireless_info_t.channel);
  printf("net_id: %d\n", wireless_info_t.net_id);
  wireless_info_t.channel = 20;
  //printf("uart_e22_ove_flag: %d\n", uart_e22_ove_flag);
  //printf("uart_e22_flag: %d\n", uart_e22_flag);
  #else
	printf("start test...\n");
	while(1)
	{
		delay_ms(1000);
		printf("1234\n");
	}
  #endif
  while (1)
  {
	#if BASE_PROJECT
		MX_LWIP_Process();
	#else
	    monitor_adc(3);				//检测电压
		force_shutdown();			//强制关闭模块
		if(time_1s >= TIME_1S)
		{
		  time_1s = 0;
		  GetTemperature();			//获取温度
		  quan_of_elec();			//电量显示
		}

		//show_tempeture();			//显示温度
		disturb_key_control();		//按键控制
		pd1_period_out();			//pd1周期输出控制蜂鸣器
		disturb_switch_style();
	#endif  
	
	uart_config_process();			//周期处理串口配置无线
	gps_process();					//周期解析GPS数据
	wireless_process();				//周期解析无线数据

	period_process_device_info();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
