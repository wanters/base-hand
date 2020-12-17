#include "lan8720.h"
#include "delay.h"
#include "gpio.h"
#include "usart.h"
#include "communication.h"
#include <string.h>
#include "gps.h"
#include "udp_echoserver.h"
#include "wireless.h"

#define UDP_RX_BUFSIZE   200
#define UDP_PORT   8089


uint8_t lan_send_buffer[100];
uint8_t lan_send_length;
extern uint8_t receivebuf[2000];

CONTROL_STATUS control_status;

uint16_t location_number = 0;

void LAN8720_RESET(void)
{
	HAL_GPIO_WritePin(ETH_RESET_Port,ETH_REF_CLK_Pin,GPIO_PIN_SET);
	delay_ms(500);
	HAL_GPIO_WritePin(ETH_RESET_Port,ETH_REF_CLK_Pin,GPIO_PIN_RESET);
	delay_ms(100);
}

void net_message_analysis(uint8_t *receive_buf)
{
	NET_FRAME net_frame_return;
	CONTROL_COMMAND	*control_command;
	CONTROL_COMMAND_RETURN	control_command_return;	
	TARGET_INFO *target_info;
	WIRELESS_INFO wireless_target_info;
	TARGET_INFO_NOTICE tagart_position_notice;
	
	uint8_t net_frame_length;	
	
	NET_FRAME *net_frame = (NET_FRAME*)(receive_buf);
	net_frame_return.length = 0;
	
	if(net_frame->head == 0xAABB1122)
	{
		net_frame_return.head = 0xAABB1122;
		net_frame_return.type = NET_CONTROL_RETURN;
		if(net_frame->type == NET_CONTROL) //��������
		{
			control_command = (CONTROL_COMMAND *)net_frame->payload;
			switch(control_command->control_command)
			{
				case START_BASE_INFO :
					control_command_return.control_command = START_BASE_INFO;	//��������
					control_command_return.result = STATUS_SUCCESS;				//��������ִ�н��
				
					net_frame_return.length = sizeof(control_command_return);	//payload����
					memcpy(net_frame_return.payload, (uint8_t*)&control_command_return, net_frame_return.length);
					net_frame_length = 12+net_frame_return.length;				//ҵ��֡��

					udp_senddata(upcb, (char*)&net_frame_return, net_frame_length);//����
				
					control_status.report_base_info = REPORT_ENABLE;	//�ϱ���վλ����Ϣʹ��
					printf("start_base_location\n");
				break;
				case STOP_BASE_INFO:
					control_command_return.control_command = STOP_BASE_INFO;	//��������
					control_command_return.result = STATUS_SUCCESS;				//��������ִ�н��
				
					net_frame_return.length = sizeof(control_command_return);	//payload����
					memcpy(net_frame_return.payload, (uint8_t*)&control_command_return, net_frame_return.length);
					net_frame_length = 12+net_frame_return.length;				//ҵ��֡��

					udp_senddata(upcb, (char*)&net_frame_return, net_frame_length);//����
				
					control_status.report_base_info = REPORT_DISABLE;	//���ͻ�վλ����Ϣ��ʹ��
					printf("stop_base_location\n");
				break;
				case START_HAND_INFO: 
					control_command_return.control_command = START_HAND_INFO;	//��������
					control_command_return.result = STATUS_SUCCESS;				//��������ִ�н��
				
					net_frame_return.length = sizeof(control_command_return);	//payload����
					memcpy(net_frame_return.payload, (uint8_t*)&control_command_return, net_frame_return.length);
					net_frame_length = 12+net_frame_return.length;				//ҵ��֡��

					udp_senddata(upcb, (char*)&net_frame_return, net_frame_length);//����
				
					control_status.report_hand_info = REPORT_ENABLE;	//�ϱ��ֳ�λ����Ϣʹ��
					printf("start_hand_location\r\n");
				break;
				case STOP_HAND_INFO:
					control_command_return.control_command = STOP_HAND_INFO;	//��������
					control_command_return.result = STATUS_SUCCESS;				//��������ִ�н��
				
					net_frame_return.length = sizeof(control_command_return);	//payload����
					memcpy(net_frame_return.payload, (uint8_t*)&control_command_return, net_frame_return.length);
					net_frame_length = 12+net_frame_return.length;				//ҵ��֡��

					udp_senddata(upcb, (char*)&net_frame_return, net_frame_length);//����
				
					control_status.report_hand_info = REPORT_DISABLE;	//�ϱ��ֳ�λ����Ϣ��ʹ��
					printf("stop_hand_location\n");
				break;
				case HEART_BEAT:
					control_command_return.control_command = HEART_BEAT;	//��������
					control_command_return.result = STATUS_SUCCESS;				//��������ִ�н��
				
					net_frame_return.length = sizeof(control_command_return);	//payload����
					memcpy(net_frame_return.payload, (uint8_t*)&control_command_return, net_frame_return.length);
					net_frame_length = 12+net_frame_return.length;				//ҵ��֡��

					udp_senddata(upcb, (char*)&net_frame_return, net_frame_length);//����
				break;
				default: 
					printf("no cmd\n");
				break;
			}
		}
		if(net_frame->type == NET_TARGET_INFO)  //Ŀ�귽λ��Ϣ
		{
			target_info = (TARGET_INFO *)net_frame->payload;
			if(target_info->hand_id != 0)			//û��0���豸����ִ�С�
			{						
				/*
					������Ϣ
				*/
				wireless_target_info.addr_h = BROADCAST_ADDR_H;
				wireless_target_info.addr_l = BROADCAST_ADDR_L;	
				wireless_target_info.channel = wireless_info_t.channel;;
				/*
					�·�Ŀ��λ����Ϣ
				*/
				tagart_position_notice.device_id = target_info->hand_id;
				tagart_position_notice.cmd_id = COMM_TARGET_INFO;
				tagart_position_notice.horizontal_angle = target_info->horizontal_angle;
				tagart_position_notice.distance = target_info->distance;
				
				/*
					�������
				*/
				memcpy(lan_send_buffer, (uint8_t*)&wireless_target_info, sizeof(wireless_target_info));
				lan_send_length = sizeof(wireless_target_info);
				lan_send_buffer[lan_send_length] = '$';
				lan_send_length++;
				lan_send_buffer[lan_send_length] = sizeof(tagart_position_notice);
				lan_send_length++;
				memcpy(lan_send_buffer+lan_send_length, (uint8_t*)&tagart_position_notice, sizeof(tagart_position_notice));
				lan_send_length += sizeof(tagart_position_notice);
				lan_send_buffer[lan_send_length] = '#';
				lan_send_length += 1;
				
				location_number++;
				printf("horizontal_angle: %f, %d\n", tagart_position_notice.horizontal_angle, location_number);
				#if 0
				printf("send target position\n");
				for(uint8_t i=0; i<lan_send_length; i++)
				{
					printf("%02X ", lan_send_buffer[i]);
				}
				printf("\n");
				#endif
				
				
				/*
					ͨ�����ߴ��ڷ���
				*/
				HAL_UART_Transmit(&E22_UART, lan_send_buffer, lan_send_length, 0xffff);
							
				control_status.report_hand_info_pc = REPORT_ENABLE;
			}
		}
	}
}













