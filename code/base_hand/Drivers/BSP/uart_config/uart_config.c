#include "wireless.h"
#include "gpio.h"
#include "usart.h"
#include "delay.h"
#include "communication.h"
#include "bhky_errno.h"
#include "uart_config.h"

#define CONFIG_BUFFER_LEN	64

typedef struct
{
	uint8_t  uart_config_buffer[CONFIG_BUFFER_LEN];	//����
	uint8_t  uart_config_flag;			//����֡��־
	uint8_t  uart_config_state;			//��ǰ����״̬
	uint8_t  uart_config_type;			//֡����
	uint16_t uart_config_len;			//��ǰ���ճ���
	uint8_t  uart_config_parameter_len;	//��������
	
} UART_CONFIG_DATA_T;

UART_CONFIG_DATA_T uart_config_data;
uint8_t uart_config_status;

void clear_uart_config_data(UART_CONFIG_DATA_T *uart_config_data)
{
	uart_config_data->uart_config_len = 0;
	uart_config_data->uart_config_state = FRAME_NULL;
	uart_config_data->uart_config_flag = 0;
	uart_config_data->uart_config_parameter_len = 0;
}

void uart_config_set(void)
{
	uart_config_status = 1;
}

uint8_t uart_config_get(void)
{
	return uart_config_status;
}

void uart_config_clear(void)
{
	uart_config_status = 0;
}

/*
	����һ֡����
*/
void uart_config_recv_check_frame(void)
{
	uint8_t status = BHKY_SUCCESS;
	uint8_t recv_char = 0;
	
	if(uart_config_data.uart_config_flag == 0)
	{
		if(uart_config_data.uart_config_state == FRAME_NULL)
		{
			status = uart_read_buf_one_char(&uart_debug_buff, &recv_char);

			/*
				֡ͷ�ж�
			*/
			if (status == BHKY_SUCCESS)
			{
				switch(recv_char)
				{
					case SET_COMMAND:
					case GET_COMMAND:
					case SET_ONCE_COMMAND:
						uart_config_data.uart_config_type = recv_char;
						uart_config_data.uart_config_state = FRAME_START;
						uart_config_data.uart_config_len = 0;	
						uart_config_data.uart_config_buffer[uart_config_data.uart_config_len] = recv_char;
						uart_config_data.uart_config_len++;
					break;
					default:
						printf("noactie data: %02X\n", recv_char);
					break;
				}
			}
		}
		else
		{
			/*
				��Ч������֡�ж�
			*/
			do
			{
				status = uart_read_buf_one_char(&uart_debug_buff, &recv_char);
				if (status == BHKY_SUCCESS)
				{
					switch(uart_config_data.uart_config_type)
					{
						/*
							��ȡ����
						*/
						case GET_COMMAND:
							uart_config_data.uart_config_state = FRAME_DATA;
							uart_config_data.uart_config_buffer[uart_config_data.uart_config_len] = recv_char;
							uart_config_data.uart_config_len++;	
							/*
								�����ж�
							*/
							if(uart_config_data.uart_config_len == 3)
							{
								uart_config_data.uart_config_state = FRAME_STOP;
							}	
						break;
						case SET_COMMAND:
						case SET_ONCE_COMMAND:
							uart_config_data.uart_config_state = FRAME_DATA;
							uart_config_data.uart_config_buffer[uart_config_data.uart_config_len] = recv_char;
							uart_config_data.uart_config_len++;		
							/*
								���������ж�
							*/
							if(uart_config_data.uart_config_len == 3)
							{
								uart_config_data.uart_config_parameter_len = recv_char;
								if(uart_config_data.uart_config_parameter_len > PARAMETER_MAX_LEN)
								{
									uart_config_data.uart_config_state = FRAME_ERROR;
								}
							}	
							/*
								�����ж�
							*/
							else if(uart_config_data.uart_config_parameter_len+3 == uart_config_data.uart_config_len)
							{
								uart_config_data.uart_config_state = FRAME_STOP;
							}	
						break;
						default:
							printf("No support frame: %02X\n", uart_config_data.uart_config_type);
							break;
					}	
				}
				else 
				{
					uart_config_data.uart_config_state = FRAME_DATA;
					break;
				}
				
				/*
					����̫��
				*/
				if(uart_config_data.uart_config_len >= CONFIG_BUFFER_LEN)
				{
					uart_config_data.uart_config_state = FRAME_ERROR;
				}
			}while(uart_config_data.uart_config_state!=FRAME_ERROR && \
				   uart_config_data.uart_config_state!=FRAME_STOP);
			
			/*
				����֡
			*/
			if(uart_config_data.uart_config_state == FRAME_ERROR)
			{
				//���wireless
				clear_uart_config_data(&uart_config_data);
			}
			
			/*
				����֡
			*/
			if(uart_config_data.uart_config_state == FRAME_STOP)
			{
				//uart_config_data.uart_config_flag = 1;
			}
		}
	}
}

/*
	�������ô���
*/
void uart_config_process_line(void)
{
	if(uart_config_data.uart_config_state == FRAME_STOP)
	{
		#if 0
		uint16_t i;
		printf("\n### uart_config data ###\n");
		for(i=0; i<uart_config_data.uart_config_len; i++)
		{
			printf("%02X ", uart_config_data.uart_config_buffer[i]);
		}
		printf("\n");
		#endif
		
		/*
			����e22
		*/
		uart_trans_set(uart_config_data.uart_config_buffer, uart_config_data.uart_config_len);
		//uart_config_set();
		clear_response_time();
		clear_uart_config_data(&uart_config_data);
	}
}

void uart_config_init(void)
{
	memset(&uart_config_data, 0, sizeof(uart_config_data));
}

void uart_config_process(void)
{
	//��顢����E22
	uart_config_recv_check_frame();
	uart_config_process_line();
}

