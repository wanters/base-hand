
#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "stm32f4xx.h"
#include "stdbool.h"

#define HAND_MAX  10

#pragma pack(1)

typedef enum
{
	HAND_TYPE,
	BASE_TYPE,
}DEVICE_TYPE;

/*********** ��������start************/

/*
	������������
*/
typedef enum
{
	NET_CONTROL = 1,
	NET_CONTROL_RETURN,
	NET_DEVICE_INFO,
	NET_TARGET_INFO
}NET_COMMAND;

/*
	����֡
*/
typedef struct
{
	int head;
	int type;
	int length;
	uint8_t	payload[100];
}NET_FRAME;

/*
	������������
*/
typedef enum
{
	START_BASE_INFO = 1,
	STOP_BASE_INFO, 
	START_HAND_INFO,
	STOP_HAND_INFO,
	HEART_BEAT,	
}NET_CONTROL_COMMAND;

/*
	��������״̬
*/
typedef enum
{
	STATUS_FAIL = 0,
	STATUS_SUCCESS
}NET_COMMAND_STATUS;

/*
	��������
*/
typedef struct 
{
	int control_command;
}CONTROL_COMMAND;

/*
	���������
*/
typedef struct 
{
	int control_command;
	uint8_t result;
}CONTROL_COMMAND_RETURN;

/*
	�豸��Ϣ(GPS��Ϣ)
*/
typedef struct 
{
	unsigned int device_id;
	uint8_t device_type;
	double longitude;
	double latitude;
	double altitude;
	int    device_status;
}DEVICE_INFO;

/*
	Ŀ�귽λ��Ϣ
*/
typedef struct 
{
	int    hand_id;
	double horizontal_angle;
	double distance;
}TARGET_INFO;

/*********** ��������end ************/

/*********** ���ߴ�������start ************/

typedef enum
{
	COMM_GPS_ENABLE,
	COMM_TARGET_INFO,
	COMM_GPS_INFO,
	
}COMM_CMMAND;

typedef struct 
{
	uint8_t addr_h;
	uint8_t addr_l;
	uint8_t channel;	
}WIRELESS_INFO;

/*
	�ϱ�GPS��Ϣʹ��
*/
typedef struct 
{
	uint8_t device_id;
	uint8_t cmd_id;
}HAND_INFO;

/*
	�ֳ�GPS��Ϣ
*/
typedef struct 
{
	uint8_t device_id;
	uint8_t cmd_id;
	double 	longitude;
	double	latitude;
	double   altitude;
	uint8_t valid;
}HAND_GPS_REPORT_INFO;

/*
	�·�Ŀ�귽λ��Ϣ
*/
typedef struct 
{
	uint8_t device_id;
	uint8_t cmd_id;
	double horizontal_angle;
	double distance;
}TARGET_INFO_NOTICE;

/*********** ���ߴ�������end ************/

enum
{
	REPORT_DISABLE,	
	REPORT_ENABLE,
};

typedef struct 
{
	uint8_t report_base_info;
	uint8_t report_hand_info;
	uint8_t report_hand_info_pc;	
	uint8_t hand_info_flag;
	uint8_t net_beep;
}CONTROL_STATUS;

typedef struct
{
	uint8_t buffer[100];
	uint8_t length;
}DEVICE_INFO_TO_NET;

extern CONTROL_STATUS control_status;
extern DEVICE_INFO_TO_NET base_info_to_net;
extern DEVICE_INFO_TO_NET hand_info_to_net;

#pragma pack()

#endif
