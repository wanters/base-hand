//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 1.3������IIC�ӿ���ʾ����(STM32F103C8T6 IIC�ӿ�ϵ��)
//              ˵��: ��ģ��Ĭ��ʹ�õ���SPI�ӿڣ������Ҫ��IIC�ӿڣ��밴�������
//							1.SPIλ�õ��軻��IICλ��
//							2.DC,CS�ܽŽӵأ�DC�ĸߵ׵�ƽ���������IIC�豸��ַ�ģ�����Ȥ�����о�һ��
//							�ο�������ͨ��io��DC,CS�����ˣ������ڳ�ʼ�������п������û�Ҳ����ֱ�ӽӵء�
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES  ��PB0
//              DC   ��PB1//����ֱ�ӽӵ�
//              CS   ��PA4//����ֱ�ӽӵ�               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
 int main(void)
 {	u8 t;
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
		OLED_Init();			//��ʼ��OLED  
		OLED_Clear()  	; 
		/*
		 OLED_ShowString(30,0,"OLED TEST");
		OLED_ShowString(8,2,"ZHONGJINGYUAN");  
		OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		t=' '; 
		OLED_ShowCHinese(0,0,0);//��
		OLED_ShowCHinese(18,0,1);//��
		OLED_ShowCHinese(36,0,2);//԰
		OLED_ShowCHinese(54,0,3);//��
		OLED_ShowCHinese(72,0,4);//��
		OLED_ShowCHinese(90,0,5);//��
		OLED_ShowCHinese(108,0,6);//��
		*/	
		t=' ';
	while(1) 
	{		
		OLED_Clear();
		OLED_ShowCHinese(6,0,0);//��
		OLED_ShowCHinese(22,0,1);//��
		OLED_ShowCHinese(40,0,2);//��
		
		OLED_ShowCHinese(6,2,2);//԰
		OLED_ShowCHinese(22,2,3);//��
		OLED_ShowCHinese(40,2,4);//��
		//OLED_ShowCHinese(90,0,5);//��
		//OLED_ShowCHinese(108,0,6);//��
		OLED_ShowString(0,4,"1.3OLED");
		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 //	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,":64*128:");  
		OLED_ShowString(0,8,"ABCDEFGH:");  
		OLED_ShowString(0,10,"12345678:");  
		OLED_ShowString(0,12,"ABCDEFGH:");  
		OLED_ShowString(0,14,"12345678:");  
		//OLED_ShowChar(48,6,t);//��ʾASCII�ַ�	   
	//	t++;
		//if(t>'~')t=' ';
		//OLED_ShowNum(103,6,t,3,16);//��ʾASCII�ַ�����ֵ 	
			
		
		delay_ms(8000);
		OLED_Clear();
		delay_ms(8000);
		OLED_DrawBMP(0,0,64,16,BMP1); //64*128ͼƬ
		delay_ms(8000);
		delay_ms(8000);
				OLED_DrawBMP(0,0,64,16,BMP2); //64*128ͼƬ
		delay_ms(8000);
		delay_ms(8000);
	}	  
	
}

