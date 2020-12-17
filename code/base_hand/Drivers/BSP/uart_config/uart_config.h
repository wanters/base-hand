
#ifndef __UART_CONFIG_H__
#define __UART_CONFIG_H__

#include "stm32f4xx.h"

void uart_config_init(void);
void uart_config_process(void);
void uart_config_process_line(void);
void uart_config_set(void);
uint8_t uart_config_get(void);
void uart_config_clear(void);

#endif

