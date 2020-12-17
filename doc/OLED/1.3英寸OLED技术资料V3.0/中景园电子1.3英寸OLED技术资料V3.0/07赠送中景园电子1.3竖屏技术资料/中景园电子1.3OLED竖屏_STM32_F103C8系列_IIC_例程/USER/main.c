//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 1.3寸竖屏IIC接口演示例程(STM32F103C8T6 IIC接口系列)
//              说明: 本模块默认使用的是SPI接口；如果需要用IIC接口，请按下面操作
//							1.SPI位置电阻换到IIC位置
//							2.DC,CS管脚接地；DC的高底电平是用来设计IIC设备地址的；有兴趣可以研究一下
//							参考程序中通过io将DC,CS拉低了；可以在初始化函数中看到；用户也可以直接接地。
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）
//              RES  接PB0
//              DC   接PB1//可以直接接地
//              CS   接PA4//可以直接接地               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
 int main(void)
 {	u8 t;
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
		OLED_Init();			//初始化OLED  
		OLED_Clear()  	; 
		/*
		 OLED_ShowString(30,0,"OLED TEST");
		OLED_ShowString(8,2,"ZHONGJINGYUAN");  
		OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		t=' '; 
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		*/	
		t=' ';
	while(1) 
	{		
		OLED_Clear();
		OLED_ShowCHinese(6,0,0);//中
		OLED_ShowCHinese(22,0,1);//中
		OLED_ShowCHinese(40,0,2);//景
		
		OLED_ShowCHinese(6,2,2);//园
		OLED_ShowCHinese(22,2,3);//电
		OLED_ShowCHinese(40,2,4);//子
		//OLED_ShowCHinese(90,0,5);//科
		//OLED_ShowCHinese(108,0,6);//技
		OLED_ShowString(0,4,"1.3OLED");
		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 //	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,":64*128:");  
		OLED_ShowString(0,8,"ABCDEFGH:");  
		OLED_ShowString(0,10,"12345678:");  
		OLED_ShowString(0,12,"ABCDEFGH:");  
		OLED_ShowString(0,14,"12345678:");  
		//OLED_ShowChar(48,6,t);//显示ASCII字符	   
	//	t++;
		//if(t>'~')t=' ';
		//OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
			
		
		delay_ms(8000);
		OLED_Clear();
		delay_ms(8000);
		OLED_DrawBMP(0,0,64,16,BMP1); //64*128图片
		delay_ms(8000);
		delay_ms(8000);
				OLED_DrawBMP(0,0,64,16,BMP2); //64*128图片
		delay_ms(8000);
		delay_ms(8000);
	}	  
	
}

