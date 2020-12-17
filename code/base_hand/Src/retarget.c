/**
  ******************************************************************************
  * File Name          : retarget.c
  * Description        : 
  *   - 重定向打印串口
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 BHKY.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */
#include "usart.h"
#include "stdio.h"	

#pragma import(__use_no_semihosting_swi)

struct __FILE
{
    int handle; /* Add whatever you need here */
};
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
{
    uart_send_char(&DEBUG_UART, ch);
    return ch;
}

int fgetc(FILE *f)
{
    return uart_receive_char(&DEBUG_UART);
}

int ferror(FILE *f)
{
    /* Your implementation of ferror */
    return EOF;
}

void _ttywrch(int ch)
{
    uart_send_char(&DEBUG_UART, ch);
}

void _sys_exit(int return_code)
{
    while (1);    /* endless loop */
}
