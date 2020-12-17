#include "stm32f4xx.h"

/*
	无线模块工作模式
*/
typedef enum
{
	NORMAL_MODE,	//一般模式
	WOR_MODE,		//唤醒模式
	CONFIG_MODE,	//配置模式
	SLEEP_MODE		//深度休眠模式
}E22_WORK_MODE;

/*
	无线模块控制指令
*/
typedef enum
{
	SET_COMMAND = 0xC0,	//设置寄存器
	GET_COMMAND = 0xC1,	//读取寄存器
	SET_ONCE_COMMAND = 0xC2,	//临时寄存器(重启后失效)
	AIR_COMMAND		 = 0xCF,	//无线配置
}E22_COMMAND;

/*
	无线模块寄存器地址
*/
#define ADD_H				0x00	//地址高8位寄存器
#define ADD_L				0x01	//地址低8位寄存器
#define NET_ID				0x02	//网络地址寄存器
#define COMM_BAUD			0x03	//通信速率寄存器
#define AIR_TRANS_CONTROL	0x04	//无线传输控制
#define AIR_CHANNEL_CONTROL	0x05	//无线信道控制(0-64,共65个信道)
#define AIR_TX_MODE			0x06	//发射模式控制
#define PASSWD_H			0x07	//密钥高字节，默认0
#define PASSWD_L			0x08	//密钥低字节，默认0
#define MODULE_INFO			0x80	//模块信息80~86，产品信息7个字节

/*
	通信速率0x03，串口波特率
*/
#define UART_BAUD_1200		(0<<5)	
#define UART_BAUD_2400		(1<<5)	
#define UART_BAUD_4800		(2<<5)	
#define UART_BAUD_9600		(3<<5)	//默认
#define UART_BAUD_19200		(4<<5)	
#define UART_BAUD_38400		(5<<5)	
#define UART_BAUD_57600		(6<<5)	
#define UART_BAUD_115200	(7<<5)	
/*
	通信速率0x03，串口校验位
*/
#define PARITY_CHECK_8N1	(0<<3)	//默认
#define PARITY_CHECK_801	(1<<3)	
#define PARITY_CHECK_8E1	(2<<3)	
#define PARITY_CHECK_8N1_1	(3<<3)	
/*
	通信速率0x03，无线空中速率
*/
#define AIR_BAUD_0p3	(0<<0)	
#define AIR_BAUD_0p6	(1<<0)	
#define AIR_BAUD_1p2	(2<<0)	
#define AIR_BAUD_2p4	(3<<0)	//默认
#define AIR_BAUD_4p8	(4<<0)	
#define AIR_BAUD_9p6	(5<<0)	
#define AIR_BAUD_15p6	(6<<0)	
#define AIR_BAUD_15p6_1	(7<<0)	

/*
	无线传输控制0x04，分包设定
*/
#define PACKET_240	(0<<6)	//默认
#define PACKET_128	(1<<6)	
#define PACKET_64	(2<<6)	
#define PACKET_32	(3<<6)	
/*
	无线传输控制0x04，环境噪声使能
*/
#define RSSI_DISABLE	(0<<5)	//默认 PACKET_240|RSSI_DISABLE|TX_POWER_30
#define RSSI_ENABLE		(1<<5)	
/*
	无线传输控制0x04，发射功率
*/
#define TX_POWER_30		(0<<0)	//默认
#define TX_POWER_27		(1<<0)	
#define TX_POWER_24		(2<<0)	
#define TX_POWER_21		(3<<0)	

/*
	无线发射模式控制0x06，环境噪声输出
*/
#define RSSI_CLOSE	(0<<7)	//默认
#define RSSI_OPEN	(1<<7)	

/*
	无线发射模式控制0x06，传输方式
*/
#define TRANS_MODE_ONLY		(0<<6)	//透传，默认 
#define TRANS_MODE_POINT	(1<<6)	

/*
	无线发射模式控制0x06，中继功能
*/
#define REPLAY_DISABLE		(0<<5)	//默认
#define REPLAY_ENABLE		(1<<5)	
/*
	无线发射模式控制0x06，LBT功能
*/
#define LBT_DISABLE		(0<<4)	//默认
#define LBT_ENABLE		(1<<4)	
/*
	无线发射模式控制0x06，WOR功能
*/
#define WOR_RECV		(0<<3)	//默认
#define wOR_SEND		(1<<3)	
/*
	无线发射模式控制0x06，WOR周期
*/
#define WOR_500			(0<<0)	//默认
#define wOR_1000		(1<<0)	
#define WOR_1500		(2<<0)
#define wOR_2000		(3<<0)	
#define WOR_2500		(4<<0)
#define wOR_3000		(5<<0)	
#define WOR_3500		(6<<0)
#define wOR_4000		(7<<0)	

#define RESPONSE_TIMEOUT_1S 	1000
#define PARAMETER_MAX_LEN	9

typedef struct
{
	E22_COMMAND command;
	uint8_t addr;
	uint8_t length;
	uint8_t data[9];
}CONFIG_COMMAND;

#define WIRELESS_BUFFER_LEN	64
typedef struct
{
	uint8_t  wireless_buffer[WIRELESS_BUFFER_LEN];	//数据
	uint8_t  wireless_flag;			//完整帧标志
	uint8_t  wireless_state;		//当前接收状态
	uint8_t  wireless_type;			//帧类型
	uint16_t wireless_len;			//当前接收长度
	uint8_t parameter_len;			//参数长度
	
} WIRELESS_DATA_T;

enum 
{
	FRAME_NULL,
	FRAME_START,
	FRAME_DATA,
	FRAME_ERROR,
	FRAME_STOP,	
};

typedef enum
{
	OPERATION_MCU_CONFIG,
	OPERATION_UART_CONFIG,
	OPERATION_WORK,
}OPERATION_TYPE;

typedef struct 
{
	uint8_t add_h;
	uint8_t add_l;
	uint8_t net_id;
	uint8_t channel;
}WIRELESS_INFO_T;

extern WIRELESS_INFO_T wireless_info_t;
extern uint16_t alarm_count;

void set_response_time_plus1(void);
void work_mode_select(E22_WORK_MODE mode);
void clear_response_time(void);
void wireless_process(void);
void uart_trans_set(uint8_t *trans_data, uint8_t trans_length);
void e22_init_get(void);
void e22_init_set(void);
void wireless_init(void);











