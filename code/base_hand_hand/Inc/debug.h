/**
  ******************************************************************************
  * File Name          : DEBUG.h
  * Description        : This file provides code for the configuration
  *                      of the DEBUG instances.
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
#ifndef __debug_H
#define __debug_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"	 

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#if 1
#define TRACE_INFO(fmt, ...) \
	 printf("<INFO>	[%s, %d, %s]:\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define TRACE_DBG(fmt, ...) \
	//printf("<DEBUG>	[%s, %d, %s]:\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define TRACE_WARNING(fmt, ...) \
	printf("<WARNING>	[%s, %d, %s]:\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
	
#define TRACE_ERR(fmt, ...) \
	printf("<ERROR>	[%s, %d, %s]\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#else
#define TRACE_INFO(fmt, ...)
#define TRACE_DBG(fmt, ...) 
#define TRACE_WARNING(fmt, ...) 
#define TRACE_ERR(fmt, ...)
#endif


#if 0
#define GPS_INFO(fmt, ...) \
	printf("<ERROR>	[%s, %d, %s]\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define GPS_INFO(fmt, ...)
#endif
/* USER CODE END Private defines */

void MX_DEBUG_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ debug_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
