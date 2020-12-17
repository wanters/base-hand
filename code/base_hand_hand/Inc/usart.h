/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
	 
#define GPS_UART	huart4
#define E22_UART	huart2 
#define DEBUG_UART  huart1	
	 
	 
#define UART_RX_FIFO_LEN        32
#define UART_TX_FIFO_LEN        32

#define UART_RX_BUFF_LEN_MAX    2048
#define UART_TX_BUFF_LEN_MAX    1024

typedef struct
{
    uint8_t  recv_buf[UART_RX_BUFF_LEN_MAX];
    uint32_t recv_drop; // droped char number due to buffer full
    uint32_t recv_wptr;
    uint32_t recv_rptr;

    uint8_t  send_buf[UART_TX_BUFF_LEN_MAX];
    uint32_t send_wptr;
    uint32_t send_rptr;
} UART_RX_TX_BUFF_T;
	 
extern uint8_t uart_recv_e22;
extern uint8_t uart_recv_gps;
extern uint8_t uart_recv_debug;
extern uint8_t uart_e22_flag;
extern uint8_t uart_gps_flag;
extern uint8_t uart_debug_flag;
extern uint8_t uart_e22_ove_flag;
extern uint8_t uart_gps_ove_flag;
extern uint8_t uart_debug_ove_flag;

extern UART_RX_TX_BUFF_T uart_e22_buff;
extern UART_RX_TX_BUFF_T uart_gps_buff;
extern UART_RX_TX_BUFF_T uart_debug_buff;

void uart_send_char(UART_HandleTypeDef *huart, uint8_t data);
uint8_t uart_receive_char(UART_HandleTypeDef *huart);
int uart_write_buf(UART_HandleTypeDef *huart, UART_RX_TX_BUFF_T *uart_tx_rx_buff, uint8_t *p_buf, uint32_t len);
int uart_read_buf(UART_RX_TX_BUFF_T *uart_tx_rx_buff, uint8_t *p_buf, uint32_t len);
int uart_read_buf_one_char(UART_RX_TX_BUFF_T *uart_tx_rx_buff, uint8_t *ch);
int uart_check_send_complete(UART_HandleTypeDef *huart, UART_RX_TX_BUFF_T *uart_tx_rx_buff);
 
void uart_e22_reinit(uint32_t baud);

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
