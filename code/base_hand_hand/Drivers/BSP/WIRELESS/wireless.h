#include "stm32f4xx.h"

/*
	����ģ�鹤��ģʽ
*/
typedef enum
{
	NORMAL_MODE,	//һ��ģʽ
	WOR_MODE,		//����ģʽ
	CONFIG_MODE,	//����ģʽ
	SLEEP_MODE		//�������ģʽ
}E22_WORK_MODE;

/*
	����ģ�����ָ��
*/
typedef enum
{
	SET_COMMAND = 0xC0,	//���üĴ���
	GET_COMMAND = 0xC1,	//��ȡ�Ĵ���
	SET_ONCE_COMMAND = 0xC2,	//��ʱ�Ĵ���(������ʧЧ)
	AIR_COMMAND		 = 0xCF,	//��������
}E22_COMMAND;

/*
	����ģ��Ĵ�����ַ
*/
#define ADD_H				0x00	//��ַ��8λ�Ĵ���
#define ADD_L				0x01	//��ַ��8λ�Ĵ���
#define NET_ID				0x02	//�����ַ�Ĵ���
#define COMM_BAUD			0x03	//ͨ�����ʼĴ���
#define AIR_TRANS_CONTROL	0x04	//���ߴ������
#define AIR_CHANNEL_CONTROL	0x05	//�����ŵ�����(0-64,��65���ŵ�)
#define AIR_TX_MODE			0x06	//����ģʽ����
#define PASSWD_H			0x07	//��Կ���ֽڣ�Ĭ��0
#define PASSWD_L			0x08	//��Կ���ֽڣ�Ĭ��0
#define MODULE_INFO			0x80	//ģ����Ϣ80~86����Ʒ��Ϣ7���ֽ�

/*
	ͨ������0x03�����ڲ�����
*/
#define UART_BAUD_1200		(0<<5)	
#define UART_BAUD_2400		(1<<5)	
#define UART_BAUD_4800		(2<<5)	
#define UART_BAUD_9600		(3<<5)	//Ĭ��
#define UART_BAUD_19200		(4<<5)	
#define UART_BAUD_38400		(5<<5)	
#define UART_BAUD_57600		(6<<5)	
#define UART_BAUD_115200	(7<<5)	
/*
	ͨ������0x03������У��λ
*/
#define PARITY_CHECK_8N1	(0<<3)	//Ĭ��
#define PARITY_CHECK_801	(1<<3)	
#define PARITY_CHECK_8E1	(2<<3)	
#define PARITY_CHECK_8N1_1	(3<<3)	
/*
	ͨ������0x03�����߿�������
*/
#define AIR_BAUD_0p3	(0<<0)	
#define AIR_BAUD_0p6	(1<<0)	
#define AIR_BAUD_1p2	(2<<0)	
#define AIR_BAUD_2p4	(3<<0)	//Ĭ��
#define AIR_BAUD_4p8	(4<<0)	
#define AIR_BAUD_9p6	(5<<0)	
#define AIR_BAUD_15p6	(6<<0)	
#define AIR_BAUD_15p6_1	(7<<0)	

/*
	���ߴ������0x04���ְ��趨
*/
#define PACKET_240	(0<<6)	//Ĭ��
#define PACKET_128	(1<<6)	
#define PACKET_64	(2<<6)	
#define PACKET_32	(3<<6)	
/*
	���ߴ������0x04����������ʹ��
*/
#define RSSI_DISABLE	(0<<5)	//Ĭ�� PACKET_240|RSSI_DISABLE|TX_POWER_30
#define RSSI_ENABLE		(1<<5)	
/*
	���ߴ������0x04�����书��
*/
#define TX_POWER_30		(0<<0)	//Ĭ��
#define TX_POWER_27		(1<<0)	
#define TX_POWER_24		(2<<0)	
#define TX_POWER_21		(3<<0)	

/*
	���߷���ģʽ����0x06�������������
*/
#define RSSI_CLOSE	(0<<7)	//Ĭ��
#define RSSI_OPEN	(1<<7)	

/*
	���߷���ģʽ����0x06�����䷽ʽ
*/
#define TRANS_MODE_ONLY		(0<<6)	//͸����Ĭ�� 
#define TRANS_MODE_POINT	(1<<6)	

/*
	���߷���ģʽ����0x06���м̹���
*/
#define REPLAY_DISABLE		(0<<5)	//Ĭ��
#define REPLAY_ENABLE		(1<<5)	
/*
	���߷���ģʽ����0x06��LBT����
*/
#define LBT_DISABLE		(0<<4)	//Ĭ��
#define LBT_ENABLE		(1<<4)	
/*
	���߷���ģʽ����0x06��WOR����
*/
#define WOR_RECV		(0<<3)	//Ĭ��
#define wOR_SEND		(1<<3)	
/*
	���߷���ģʽ����0x06��WOR����
*/
#define WOR_500			(0<<0)	//Ĭ��
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
	uint8_t  wireless_buffer[WIRELESS_BUFFER_LEN];	//����
	uint8_t  wireless_flag;			//����֡��־
	uint8_t  wireless_state;		//��ǰ����״̬
	uint8_t  wireless_type;			//֡����
	uint16_t wireless_len;			//��ǰ���ճ���
	uint8_t parameter_len;			//��������
	
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











