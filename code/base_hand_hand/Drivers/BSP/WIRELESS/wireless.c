#include "wireless.h"
#include "gpio.h"
#include "usart.h"
#include "gps.h"
#include "delay.h"
#include "communication.h"
#include "bhky_errno.h"
#include "oled.h"
#include "key_switch.h"
#include "uart_config.h"

uint16_t alarm_count;

uint8_t wireless_send_buffer[100];
uint8_t wireless_send_length;
WIRELESS_INFO_T wireless_info_t;
WIRELESS_DATA_T wireless_data;
uint16_t response_time = 0;
uint16_t response_flag = 0;
uint8_t command_send_frame[12];
uint8_t command_send_length;	
uint8_t command_recv_frame[12];
uint8_t command_recv_length;
uint8_t mcu_result;
OPERATION_TYPE operation_type;

/*
	����ģʽѡ��
*/
void work_mode_select(E22_WORK_MODE mode)
{                       
	switch(mode)
	{
		case NORMAL_MODE:
			HAL_GPIO_WritePin(WIRELESS_M1_Port, WIRELESS_M1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(WIRELESS_M0_Port, WIRELESS_M0_Pin, GPIO_PIN_RESET);
		break;
		case WOR_MODE:
			HAL_GPIO_WritePin(WIRELESS_M1_Port, WIRELESS_M1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(WIRELESS_M0_Port, WIRELESS_M0_Pin, GPIO_PIN_SET);
		break;		
		case CONFIG_MODE:
			HAL_GPIO_WritePin(WIRELESS_M1_Port, WIRELESS_M1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(WIRELESS_M0_Port, WIRELESS_M0_Pin, GPIO_PIN_RESET);
		break;	
		case SLEEP_MODE:
			HAL_GPIO_WritePin(WIRELESS_M1_Port, WIRELESS_M1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(WIRELESS_M0_Port, WIRELESS_M0_Pin, GPIO_PIN_SET);
		break;
		default:
			HAL_GPIO_WritePin(WIRELESS_M1_Port, WIRELESS_M1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(WIRELESS_M0_Port, WIRELESS_M0_Pin, GPIO_PIN_RESET);
		break;
	}
	
	#if 1
	if(mode == CONFIG_MODE)
	{
		uart_e22_reinit(9600);
	}
	else
	{
		uart_e22_reinit(115200);
	}
	#endif
	
	delay_ms(100);//�ȴ�ģ������µ�ģʽ
}

/*
	������÷��͸�e22
*/
void config_e22(CONFIG_COMMAND config_command)
{
	if(config_command.command != GET_COMMAND)
	{
		command_send_length = config_command.length+3;
	}
	else
	{
		command_send_length = 3;
	}
	memcpy(command_send_frame, (uint8_t*)&config_command, command_send_length);
	HAL_UART_Transmit(&E22_UART, command_send_frame, command_send_length, 0xffff);

	#if 0
	printf("send cmd: \n");
	for(uint8_t i=0; i<command_send_length; i++)
	{
		printf("%02X ", command_send_frame[i]);
	}
	printf("\n");
	#endif
}

/*
	��յȴ���ʱ
*/
void clear_response_time(void)
{
	response_time = 0;
}

/*
	��ȡ��Ӧ��ʱ
*/
uint16_t get_response_time(void)
{
	return response_time;
}

/*
	��ȡ��Ӧ��ʱ
*/
void set_response_time_plus1(void)
{
	response_time++;
}

/*
	������ý��
*/
uint8_t check_config_result(uint8_t *send_command, uint8_t send_length, uint8_t *recv_command, uint8_t recv_length)
{
	CONFIG_COMMAND *config_command = (CONFIG_COMMAND*)send_command;
	uint8_t config_result = 0;
	uint8_t length;
	
	switch(config_command->command)
	{
		case SET_COMMAND:
			if(recv_command[0] != GET_COMMAND)
			{
				config_result = 2;
			}
			else
			{
				for(length=1; length<send_length; length++)
				{
					if(send_command[length] != recv_command[length])
					{
						config_result = 3;
						break;
					}
				}
			}
		break;
			
		case GET_COMMAND:
			#if 0
			printf("get cmd:\n");
			for(length=0; length<recv_length; length++)
			{
				printf("%02X ", recv_command[length]);
			}
			printf("\n");
			#endif
			if(recv_command[1] == ADD_H)
			{
				printf("[wireless addr]: %02X - %02X\n", recv_command[3], recv_command[4]);
				wireless_info_t.net_id = recv_command[5];
				wireless_info_t.channel = 20;
			}
		break;
		default:
			
		break;
	}
	
	return config_result;
}

/*
	�л�E22����״̬
*/
void switch_operation_type(OPERATION_TYPE oper_type)
{
	if(operation_type==OPERATION_MCU_CONFIG || operation_type==OPERATION_UART_CONFIG)
	{
		//ֻ���л�ΪOPERATION_WORK�ſ����л�
		if(oper_type == OPERATION_WORK)
		{
			operation_type = oper_type;
		}
	}
	else
	{	//ֻ����OPERATION_WORK�²ſ����л�
		operation_type = oper_type;
	}
}

/*
	mcu������Ӧ����
*/
void mcu_response_process(WIRELESS_DATA_T wireless_data)
{
	if(get_response_time() > RESPONSE_TIMEOUT_1S)
	{
		response_flag = 1;
	}
	
	if(wireless_data.wireless_state == FRAME_STOP)
	{
		response_flag = 2;
		if(wireless_data.wireless_len <= 12)
		{
			command_recv_length = wireless_data.wireless_len;
			memcpy(command_recv_frame, wireless_data.wireless_buffer, wireless_data.wireless_len);
		}
	}
}

/*
	mcu����e22
*/
uint8_t uart_set_e22(CONFIG_COMMAND config_command)
{
	response_flag = 0;
	
	//��������ģʽ
	work_mode_select(CONFIG_MODE);
	//mcu����
	switch_operation_type(OPERATION_MCU_CONFIG);
	//�������
	config_e22(config_command);
	//�ȴ�Ӧ��
	clear_response_time();
	while(response_flag == 0)
	{
		wireless_process();
	}
	//ƥ����
	if(response_flag == 2)
	{
		response_flag = check_config_result(command_send_frame, command_send_length, \
											command_recv_frame, command_recv_length);
	}
	work_mode_select(NORMAL_MODE);
	switch_operation_type(OPERATION_WORK);
	
	return response_flag;
}

/*
	mcu��ʼ��e22
*/
void e22_init_set(void)
{
	CONFIG_COMMAND config_command;
	
#if BASE_PROJECT	
	config_command.command = SET_COMMAND;
	config_command.addr = ADD_H;
	config_command.length = 0x09;
	config_command.data[0] = BASE_ADDR_H;	//��ַ��8λ
	config_command.data[1] = BASE_ADDR_L;	//��ַ��8λ
	config_command.data[2] = 0x00;	//����ID
	config_command.data[3] = UART_BAUD_115200|PARITY_CHECK_8N1|AIR_BAUD_9p6;	//ͨ������
	config_command.data[4] = PACKET_240|RSSI_DISABLE|TX_POWER_30;	//�������
	config_command.data[5] = 0x00;	//�ŵ�
	config_command.data[6] = RSSI_CLOSE|TRANS_MODE_POINT|REPLAY_DISABLE|LBT_DISABLE|WOR_RECV|WOR_500;	//����ģʽ����
	config_command.data[7] = 0x00;	//��Կ���ֽ�
	config_command.data[8] = 0x00;	//��Կ���ֽ�	
#else
	config_command.command = SET_COMMAND;
	config_command.addr = ADD_H;
	config_command.length = 0x09;
	config_command.data[0] = HAND_ADDR_H;	//��ַ��8λ
	config_command.data[1] = HAND_ADDR_L;	//��ַ��8λ
	config_command.data[2] = 0x00;	//����ID
	config_command.data[3] = UART_BAUD_115200|PARITY_CHECK_8N1|AIR_BAUD_9p6;	//ͨ������
	config_command.data[4] = PACKET_240|RSSI_DISABLE|TX_POWER_30;	//�������
	config_command.data[5] = 0x00;	//�ŵ�
	config_command.data[6] = RSSI_CLOSE|TRANS_MODE_POINT|REPLAY_DISABLE|LBT_DISABLE|WOR_RECV|WOR_500;	//����ģʽ����
	config_command.data[7] = 0x00;	//��Կ���ֽ�
	config_command.data[8] = 0x00;	//��Կ���ֽ�	
#endif
	uint8_t i;
	for(i=0; i<9; i++)
	{
		printf("%02X ", config_command.data[i]);
	}
	printf("\n");
	
	printf("config_result: %d\n", uart_set_e22(config_command));
}

/*
	mcu��ȡe22����
*/
void e22_init_get(void)
{
	CONFIG_COMMAND config_command;
	
	config_command.command = GET_COMMAND;
	config_command.addr = ADD_H;
	config_command.length = 0x09;

	if(!uart_set_e22(config_command))
	{
		printf("read wireless addr [success]\n");		
	}
	else
	{
		printf("read wireless addr [fail]\n");		
	}
}

/*
	����Ӳ����ģʽ
*/
void enter_config_mode(void)
{
	work_mode_select(CONFIG_MODE);
	switch_operation_type(OPERATION_UART_CONFIG);
}

/*
	����һ��ģʽ
*/
void enter_normal_mode(void)
{
	work_mode_select(NORMAL_MODE);
	switch_operation_type(OPERATION_WORK);
}

/*
	����͸������
*/
void uart_trans_set(uint8_t *trans_data, uint8_t trans_length)
{
	//��������ģʽ
	enter_config_mode();
	//��������
	HAL_UART_Transmit(&E22_UART, trans_data, trans_length, 0xffff);
	//��ճ�ʱ��ʱ
	clear_response_time();
}

/*
	����͸��������Ӧ����
*/
void uart_response_process(WIRELESS_DATA_T wireless_data)
{
	if(get_response_time() > RESPONSE_TIMEOUT_1S)
	{
		//uart_config_clear();
		enter_normal_mode();
	}
	
	if(wireless_data.wireless_state == FRAME_STOP)
	{
		enter_normal_mode();
		//uart_config_clear();
		#if 1
		HAL_UART_Transmit(&DEBUG_UART, wireless_data.wireless_buffer, wireless_data.wireless_len, 0xffff);
		#else
		printf("\n### wireless response ###\n");
		for(uint8_t i=0; i<wireless_data.wireless_len; i++)
		{
			printf("%02X ", wireless_data.wireless_buffer[i]);
		}
		printf("\n");
		#endif
	}
}

/*
	�����������
*/
void clear_wireless_data(WIRELESS_DATA_T *wireless_data)
{
	wireless_data->wireless_len = 0;
	wireless_data->wireless_state = FRAME_NULL;
	wireless_data->wireless_flag = 0;
	wireless_data->parameter_len = 0;
}

/*
	����֡��ʽ
	1.����������Ӧ����
		C1+��ʼ��ַ+����+����
	2.ҵ�������ϱ�
		$ length data0 data1 ... #
*/
//��������֡���
void wireless_recv_check_frame(void)
{    
	uint8_t status = BHKY_SUCCESS;
	uint8_t recv_char = 0;
	
	if(wireless_data.wireless_flag == 0)
	{
		if(wireless_data.wireless_state == FRAME_NULL)
		{
			status = uart_read_buf_one_char(&uart_e22_buff, &recv_char);

			/*
				֡ͷ�ж�
			*/
			if (status == BHKY_SUCCESS)
			{
				//printf("recv_char: %02X\n", recv_char);
				switch(recv_char)
				{
					case GET_COMMAND:
						wireless_data.wireless_type = recv_char;
						wireless_data.wireless_state = FRAME_START;
						wireless_data.wireless_len = 0;	
						wireless_data.wireless_buffer[wireless_data.wireless_len] = recv_char;
						wireless_data.wireless_len++;
					break;
					case '$':
						wireless_data.wireless_type = recv_char;						
						wireless_data.wireless_state = FRAME_START;
						wireless_data.wireless_len = 0;		
					break;
					default:
						printf("wireless_type: %d\n", wireless_data.wireless_type);
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
				status = uart_read_buf_one_char(&uart_e22_buff, &recv_char);
				if (status == BHKY_SUCCESS)
				{
					switch(wireless_data.wireless_type)
					{
						/*
							����Ӧ��
						*/
						case GET_COMMAND:
							wireless_data.wireless_state = FRAME_DATA;
							wireless_data.wireless_buffer[wireless_data.wireless_len] = recv_char;
							wireless_data.wireless_len++;	
							/*
								���������ж�
							*/
							if(wireless_data.wireless_len == 3)
							{
								wireless_data.parameter_len = recv_char;
								if(wireless_data.parameter_len > PARAMETER_MAX_LEN)
								{
									wireless_data.wireless_state = FRAME_ERROR;
								}
							}	
							/*
								�����ж�
							*/
							else if(wireless_data.parameter_len+3 == wireless_data.wireless_len)
							{
								wireless_data.wireless_state = FRAME_STOP;
							}	
						break;
						case '$':
							/*
								�����ж�
							*/
							wireless_data.wireless_state = FRAME_DATA;
							if(wireless_data.parameter_len == 0)
							{
								wireless_data.parameter_len = recv_char;
								
								if(wireless_data.parameter_len > sizeof(DEVICE_INFO))
								{
									wireless_data.wireless_state = FRAME_ERROR;
								}
							}
							else if(wireless_data.parameter_len == wireless_data.wireless_len)
							{
								wireless_data.wireless_state = FRAME_STOP;
							}
							else
							{
								wireless_data.wireless_buffer[wireless_data.wireless_len] = recv_char;
								wireless_data.wireless_len++;
							}
						break;
						default:
							printf("No support frame: %02X\n", wireless_data.wireless_type);
							break;
					}
				}
				else 
				{
					wireless_data.wireless_state = FRAME_DATA;
					break;
				}
				
				/*
					����̫��
				*/
				if(wireless_data.wireless_len >= WIRELESS_BUFFER_LEN)
				{
					wireless_data.wireless_state = FRAME_ERROR;
				}
			}while(wireless_data.wireless_state!=FRAME_ERROR && wireless_data.wireless_state!=FRAME_STOP);
			
			/*
				����֡
			*/
			if(wireless_data.wireless_state == FRAME_ERROR)
			{
				//���wireless
				clear_wireless_data(&wireless_data);
			}
			
			/*
				����֡
			*/
			if(wireless_data.wireless_state == FRAME_STOP)
			{
				//wireless_data.wireless_flag = 1;
				//printf("Have a frame\n");
			}
		}
	}
}

void hand_device_upload_gps(void)
{
	WIRELESS_INFO wireless_target_info;
	HAND_GPS_REPORT_INFO hand_gps_info;
	
	wireless_target_info.addr_h = BASE_ADDR_H;
	wireless_target_info.addr_l = BASE_ADDR_L;	
	wireless_target_info.channel = wireless_info_t.channel;
	
	hand_gps_info.device_id = HAND_DEVICE_ID;
	hand_gps_info.cmd_id = COMM_GPS_INFO;
	hand_gps_info.longitude = minmea_tocoord(&nmea_gga.longitude);
	hand_gps_info.latitude = minmea_tocoord(&nmea_gga.latitude);	
	hand_gps_info.altitude = minmea_tocoord(&nmea_gga.altitude);	
	
//	printf("longitude: %f\n", hand_gps_info.longitude);
	
	if(nmea_gga.fix_quality > 0)
	{
		hand_gps_info.valid = 1;
	}
	else
	{
		hand_gps_info.valid = 0;
	}
	
	wireless_send_length = sizeof(wireless_target_info);
	memcpy(wireless_send_buffer, (uint8_t*)&wireless_target_info, wireless_send_length);
	wireless_send_buffer[wireless_send_length] = '$';
	wireless_send_length++;
	wireless_send_buffer[wireless_send_length] = sizeof(hand_gps_info);
	wireless_send_length++;	
	memcpy(wireless_send_buffer+wireless_send_length, (uint8_t*)&hand_gps_info, sizeof(hand_gps_info));
	wireless_send_length += sizeof(hand_gps_info);
	wireless_send_buffer[wireless_send_length] = '#';
	wireless_send_length++;
	
	#if 0
	uint8_t i;
	printf("report: \n");
	for(i=0; i<wireless_send_length; i++)
	{
		printf("%02X ", wireless_send_buffer[i]);
	}
	printf("\n");
	#endif
	
	HAL_UART_Transmit(&E22_UART, (uint8_t*)wireless_send_buffer, wireless_send_length, 0xfff);
}

void wireless_response_process(WIRELESS_DATA_T wireless_data)
{
	HAND_INFO	*hand_info;
	TARGET_INFO_NOTICE *target_info_notice;
	HAND_GPS_REPORT_INFO *hand_gps_report_notice;
	DEVICE_INFO device_info;
	
	if(wireless_data.wireless_state == FRAME_STOP)
	{
		/*
			ҵ������
			1.�ϱ�GPSʹ��ָ��
				DEVICE_ID + CMD_ID[0x01] 
			2.���շ�λָ��
				DEVICE_ID + CMD_ID[0x02] + ANGEL_H + ANGEL_L + DISTANCE_H + DISTANCE_L
			3.�ϱ�GPS��Ϣָ��
				DEVICE_ID + CMD_ID[0x03] + Longitude(float) + Latitude(float) + Altitude + Active
		*/

		//ȷ���Ǹ���ǰ�豸ָ��
		switch(wireless_data.wireless_buffer[1])
		{
			case COMM_GPS_ENABLE:
				if(wireless_data.wireless_buffer[0] == HAND_DEVICE_ID)
				{
					hand_info = (HAND_INFO *)wireless_data.wireless_buffer;
					hand_device_upload_gps();
				}
			break;
				
			case COMM_TARGET_INFO:
				if(wireless_data.wireless_buffer[0] == HAND_DEVICE_ID)
				{
					target_info_notice = (TARGET_INFO_NOTICE *)wireless_data.wireless_buffer; 
					printf("horizontal_angle: %f\n", target_info_notice->horizontal_angle);
					oled_show_location((uint16_t)target_info_notice->horizontal_angle);
					control_status.net_beep = 1;
					beep_time = 0;
				}
			break;
			case COMM_GPS_INFO:
				hand_gps_report_notice = (HAND_GPS_REPORT_INFO *)wireless_data.wireless_buffer;
				device_info.device_id = hand_gps_report_notice->device_id;
				device_info.device_type = HAND_TYPE;	
				device_info.longitude = hand_gps_report_notice->longitude;
				device_info.latitude = hand_gps_report_notice->latitude;		
				device_info.altitude = hand_gps_report_notice->altitude;
				device_info.device_status = hand_gps_report_notice->valid;	
			
				control_status.hand_info_flag = REPORT_ENABLE;
				pack_device_info(&device_info);
			break;
			default:
			{
				printf("No support wireless cmd\n");
			}
			break;
		}
	}
}

void wireless_process_line(void)
{
	/*
		�����ͽ��в�������
	*/
	switch(operation_type)
	{
		case OPERATION_MCU_CONFIG:	//mcu����
			mcu_response_process(wireless_data);
		break;
		case OPERATION_UART_CONFIG:	//uart����
			uart_response_process(wireless_data);
		break;
		case OPERATION_WORK:		//����ҵ��
			wireless_response_process(wireless_data);
		break;
		default:
			
		break;
	}
	
	if(wireless_data.wireless_state == FRAME_STOP)
	{
		#if 0
		uint16_t i;
		uint16_t length;
		length = wireless_data.wireless_len;
		printf("### wireless data: %d ###\n", length);
		for(i=0; i<length; i++)
		{
			printf("%02X ", wireless_data.wireless_buffer[i]);
		}
		printf("\n");
		#endif
		
		clear_wireless_data(&wireless_data);
	}
	else
	{
		//printf("wireless current len: %d ###\n", wireless_data.wireless_len);
	}
}

void wireless_init(void)
{
	memset(&wireless_data, 0, sizeof(wireless_data));
}

void wireless_process(void)
{
	//����Ƿ�������֡
	wireless_recv_check_frame();
	
	//������֡
	wireless_process_line();	
}

