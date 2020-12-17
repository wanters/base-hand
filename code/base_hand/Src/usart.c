/**
  ******************************************************************************
  * File Name          : USART.c
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "bhky_errno.h"
#include "gps.h"

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* USER CODE BEGIN 0 */
uint8_t uart_recv_gps;
uint8_t uart_recv_e22;
uint8_t uart_recv_debug;

uint32_t baud_e22;

uint8_t uart_gps_flag = 0;
uint8_t uart_e22_flag = 0;
uint8_t uart_debug_flag = 0;

uint8_t uart_gps_ove_flag = 0;
uint8_t uart_e22_ove_flag = 0;
uint8_t uart_debug_ove_flag = 0;

UART_RX_TX_BUFF_T uart_gps_buff;
UART_RX_TX_BUFF_T uart_e22_buff;
UART_RX_TX_BUFF_T uart_debug_buff;

/* USER CODE END 0 */
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* UART4 init function */
void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**UART4 GPIO Configuration    
    PA0/WKUP     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration    
    PA0/WKUP     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
} 


/* USER CODE BEGIN 1 */
/*
 * 串口接收中断
 *
 * - huart：串口handle
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    UART_RX_TX_BUFF_T *ptr = NULL;

    if(huart->Instance == UART4) //GPS UART
    {
		ptr = &uart_gps_buff;
        if (ptr->recv_wptr + 1 == ptr->recv_rptr)
        {
            /* drop the data if buffer is full */
            ptr->recv_drop++;
        }
        else
        {
            ptr->recv_buf[ptr->recv_wptr] = uart_recv_gps;
            ptr->recv_wptr++;
            if (ptr->recv_wptr >= UART_RX_BUFF_LEN_MAX)
                ptr->recv_wptr = 0;
        }
		uart_gps_flag = 1;
        HAL_UART_Receive_IT(huart, &uart_recv_gps, 1);
    }
    
	if(huart->Instance == USART2) //wireless
	{
		ptr = &uart_e22_buff;
        if (ptr->recv_wptr + 1 == ptr->recv_rptr)
        {
            /* drop the data if buffer is full */
            ptr->recv_drop++;
        }
        else
        {
            ptr->recv_buf[ptr->recv_wptr] = uart_recv_e22;
            ptr->recv_wptr++;
            if (ptr->recv_wptr >= UART_RX_BUFF_LEN_MAX)
                ptr->recv_wptr = 0;
        }
		uart_e22_flag++;
		HAL_UART_Receive_IT(huart, &uart_recv_e22, 1);
	}
	
	if(huart->Instance == USART1) //debug
	{
		ptr = &uart_debug_buff;
        if (ptr->recv_wptr + 1 == ptr->recv_rptr)
        {
            /* drop the data if buffer is full */
            ptr->recv_drop++;
        }
        else
        {
            ptr->recv_buf[ptr->recv_wptr] = uart_recv_debug;
            ptr->recv_wptr++;
            if (ptr->recv_wptr >= UART_RX_BUFF_LEN_MAX)
                ptr->recv_wptr = 0;
        }
		uart_debug_flag = 1;
		HAL_UART_Receive_IT(huart, &uart_recv_debug, 1);
	}
}

/*
 * 串口发送中断
 *
 * - huart：串口handle
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    UART_RX_TX_BUFF_T *ptr = NULL;

    if(huart->Instance == UART4) //GPS UART
    {
		ptr = &uart_gps_buff;
	}
	else if(huart->Instance == USART2) //wireless
	{
		ptr = &uart_e22_buff;
	}
	else if(huart->Instance == USART1) //debug
	{
		ptr = &uart_debug_buff;
	}
	
    if (ptr->send_rptr != ptr->send_wptr) // not empty
    {
        ptr->send_rptr++;
        if (ptr->send_rptr >= UART_TX_BUFF_LEN_MAX)
            ptr->send_rptr = 0;
        /* enable Tx interrupt */
        if (ptr->send_rptr != ptr->send_wptr)
            HAL_UART_Transmit_IT(huart, &(ptr->send_buf[ptr->send_rptr]), 1);
    }
}

/*
 * 串口接收错误中断
 *
 * - huart：串口handle
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == UART4)
	{
		uart_gps_ove_flag = 1;
		CLEAR_BIT(huart->Instance->SR, USART_SR_ORE);
		HAL_UART_Receive_IT(huart, &uart_recv_gps, 1);
	}
	
	if(huart->Instance == USART2)
	{
		uart_e22_ove_flag = 1;
		CLEAR_BIT(huart->Instance->SR, USART_SR_ORE);
		HAL_UART_Receive_IT(huart, &uart_recv_e22, 1);
	}
	
	if(huart->Instance == USART1)
	{
		uart_debug_ove_flag = 1;
		CLEAR_BIT(huart->Instance->SR, USART_SR_ORE);
		HAL_UART_Receive_IT(huart, &uart_recv_debug, 1);
	}	
}

/*
 * 向指定串口发送一个字符
 *
 * - huart：串口handle
 */
void uart_send_char(UART_HandleTypeDef *huart, uint8_t data)
{
    uint8_t send_data;
    if ((data == '\n') && (huart == &DEBUG_UART))
    {
        send_data = '\r';
        HAL_UART_Transmit(huart, &send_data, 1, 0xffff);
    }

    HAL_UART_Transmit(huart, &data, 1, 0xffff);
}

/*
 * 从指定串口接收一个字符
 *
 * - huart：串口handle
 */
uint8_t uart_receive_char(UART_HandleTypeDef *huart)
{
    uint8_t recv_data;

    HAL_UART_Receive(huart, &recv_data, 1, 0xffff);

    return recv_data;
}

/*
 * 向指定缓冲区写指定数据长度
 *
 * - huart：串口handle
 * - uart_tx_rx_buff：缓冲区
 * - p_buf：发送数据指针
 * - len (len <= UART_RX_BUFF_LEN_MAX)：发送长度
 */
int uart_write_buf(UART_HandleTypeDef *huart, UART_RX_TX_BUFF_T *uart_tx_rx_buff, uint8_t *p_buf, uint32_t len)
{
    UART_RX_TX_BUFF_T *ptr = uart_tx_rx_buff;
    uint32_t seg_size;
    uint32_t send_buf_space;
	
    if ((len > UART_TX_BUFF_LEN_MAX) || (p_buf == NULL))
        return BHKY_HOSTIF_OVER_BUFFER_SIZE;

    //将数据存到buffer
    if (ptr->send_wptr < ptr->send_rptr)
        send_buf_space = ptr->send_rptr - ptr->send_wptr - 1;
    else
        send_buf_space = UART_TX_BUFF_LEN_MAX - ptr->send_wptr + ptr->send_rptr - 1;

    if (send_buf_space > len) // buffer足够大
    {
        if (ptr->send_wptr < ptr->send_rptr)
        {
            memcpy(ptr->send_buf + ptr->send_wptr, p_buf, len);
            ptr->send_wptr += len;
        }
        else
        {
            seg_size = UART_TX_BUFF_LEN_MAX - ptr->send_wptr;
            if (seg_size >= len)
            {
                memcpy(ptr->send_buf + ptr->send_wptr, p_buf, len);
                ptr->send_wptr += len;
                if (ptr->send_wptr >= UART_TX_BUFF_LEN_MAX)
                    ptr->send_wptr = 0;
            }
            else
            {
                memcpy(ptr->send_buf + ptr->send_wptr, p_buf, seg_size);
                len -= seg_size;
                memcpy(ptr->send_buf + 0, p_buf + seg_size, len);
                ptr->send_wptr = len;
            }
        }
    }
    else
    {
        return BHKY_HOSTIF_OVER_BUFFER_SIZE;
    }

    // enable tx interrupt
    if (ptr->send_rptr != ptr->send_wptr)
        HAL_UART_Transmit_IT(huart, &(ptr->send_buf[ptr->send_rptr]), 1);

    return BHKY_SUCCESS;
}

/*
 * 从指定缓冲区读指定数据长度
 *
 * - uart_tx_rx_buff：缓冲区
 * - p_buf：接收数据指针
 * - len (len <= UART_RX_BUFF_LEN_MAX)：接收长度
 */
int uart_read_buf(UART_RX_TX_BUFF_T *uart_tx_rx_buff, uint8_t *p_buf, uint32_t len)
{
    UART_RX_TX_BUFF_T *ptr = uart_tx_rx_buff;
    uint32_t seg_size;
    uint32_t recv_buf_avail;

    if (ptr->recv_rptr <= ptr->recv_wptr)
        recv_buf_avail = ptr->recv_wptr - ptr->recv_rptr;
    else
        recv_buf_avail = UART_RX_BUFF_LEN_MAX - ptr->recv_rptr + ptr->recv_wptr;

    if (recv_buf_avail < len)
    {
        return BHKY_BUSY;
    }

    // 把接收到的数据放到用户buffer
    if (ptr->recv_rptr < ptr->recv_wptr)
    {
        if (p_buf)
            memcpy(p_buf, ptr->recv_buf + ptr->recv_rptr, len);
        ptr->recv_rptr += len;
    }
    else
    {
        seg_size = UART_RX_BUFF_LEN_MAX - ptr->recv_rptr;
        if (seg_size >= len)
        {
            if (p_buf)
                memcpy(p_buf, ptr->recv_buf + ptr->recv_rptr, len);
            ptr->recv_rptr += len;
            if (ptr->recv_rptr >= UART_RX_BUFF_LEN_MAX)
                ptr->recv_rptr = 0;
        }
        else
        {
            if (p_buf)
                memcpy(p_buf, ptr->recv_buf + ptr->recv_rptr, seg_size);
            len -= seg_size;
            if (p_buf)
                memcpy(p_buf + seg_size, ptr->recv_buf + 0, len);
            ptr->recv_rptr = len;
        }
    }

    return BHKY_SUCCESS;
}

/*
 * 从指定buffer中读一个字符
 *
 * - uart_tx_rx_buff：缓冲区
 * - ch：字符指针
 */
int uart_read_buf_one_char(UART_RX_TX_BUFF_T *uart_tx_rx_buff, uint8_t *ch)
{
    UART_RX_TX_BUFF_T *ptr = uart_tx_rx_buff;
    if (ptr->recv_rptr != ptr->recv_wptr)
    {
        *ch = ptr->recv_buf[ptr->recv_rptr];
        ptr->recv_rptr++;
        if (ptr->recv_rptr >= UART_RX_BUFF_LEN_MAX)
            ptr->recv_rptr = 0;
    }
    else
        return BHKY_BUSY;

    return BHKY_SUCCESS;
}

/*
 * 检查指定串口的缓冲区是否发送完成
 *
 * - huart：串口handle
 * - uart_tx_rx_buff：缓冲区
 */
int uart_check_send_complete(UART_HandleTypeDef *huart, UART_RX_TX_BUFF_T *uart_tx_rx_buff)
{
    UART_RX_TX_BUFF_T *ptr = uart_tx_rx_buff;

    if ((ptr->send_rptr == ptr->send_wptr) && (HAL_UART_GetState(huart) & HAL_UART_STATE_READY))
        return BHKY_SUCCESS;
    else
        return BHKY_FAIL;
}

void uart_e22_reinit(uint32_t baud)
{
  HAL_UART_AbortReceive_IT(&E22_UART);
	
  huart2.Instance = USART2;
  huart2.Init.BaudRate = baud;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  
  HAL_UART_Receive_IT(&E22_UART, &uart_recv_e22,   1);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
