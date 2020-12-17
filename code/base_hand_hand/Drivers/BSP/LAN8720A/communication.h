
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

/*********** 网络命令start************/

/*
	网络命令类型
*/
typedef enum
{
	NET_CONTROL = 1,
	NET_CONTROL_RETURN,
	NET_DEVICE_INFO,
	NET_TARGET_INFO
}NET_COMMAND;

/*
	网络帧
*/
typedef struct
{
	int head;
	int type;
	int length;
	uint8_t	payload[100];
}NET_FRAME;

/*
	控制命令类型
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
	控制命令状态
*/
typedef enum
{
	STATUS_FAIL = 0,
	STATUS_SUCCESS
}NET_COMMAND_STATUS;

/*
	控制命令
*/
typedef struct 
{
	int control_command;
}CONTROL_COMMAND;

/*
	控制命令返回
*/
typedef struct 
{
	int control_command;
	uint8_t result;
}CONTROL_COMMAND_RETURN;

/*
	设备信息(GPS信息)
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
	目标方位信息
*/
typedef struct 
{
	int    hand_id;
	double horizontal_angle;
	double distance;
}TARGET_INFO;

/*********** 网络命令end ************/

/*********** 无线串口命令start ************/

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
	上报GPS信息使能
*/
typedef struct 
{
	uint8_t device_id;
	uint8_t cmd_id;
}HAND_INFO;

/*
	手持GPS信息
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
	下发目标方位信息
*/
typedef struct 
{
	uint8_t device_id;
	uint8_t cmd_id;
	double horizontal_angle;
	double distance;
}TARGET_INFO_NOTICE;

/*********** 无线串口命令end ************/

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
