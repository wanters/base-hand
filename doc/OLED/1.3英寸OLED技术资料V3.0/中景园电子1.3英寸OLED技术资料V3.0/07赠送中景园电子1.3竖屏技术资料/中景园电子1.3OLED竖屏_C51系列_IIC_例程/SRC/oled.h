//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖĞ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼ş Ãû   : main.c
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : HuangKai
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üĞŞ¸Ä   : 
//  ¹¦ÄÜÃèÊö   : 1.3´çÊúÆÁIIC½Ó¿ÚÑİÊ¾Àı³Ì(51µ¥Æ¬»ú IIC½Ó¿ÚÏµÁĞ)
//              ËµÃ÷: ±¾Ä£¿éÄ¬ÈÏÊ¹ÓÃµÄÊÇSPI½Ó¿Ú£»Èç¹ûĞèÒªÓÃIIC½Ó¿Ú£¬Çë°´ÏÂÃæ²Ù×÷
//							1.SPIÎ»ÖÃµç×è»»µ½IICÎ»ÖÃ
//							2.DC,CS¹Ü½Å½ÓµØ£»DCµÄ¸ßµ×µçÆ½ÊÇÓÃÀ´Éè¼ÆIICÉè±¸µØÖ·µÄ£»ÓĞĞËÈ¤¿ÉÒÔÑĞ¾¿Ò»ÏÂ
//							²Î¿¼³ÌĞòÖĞÍ¨¹ıio½«DC,CSÀ­µÍÁË£»¿ÉÒÔÔÚ³õÊ¼»¯º¯ÊıÖĞ¿´µ½£»ÓÃ»§Ò²¿ÉÒÔÖ±½Ó½ÓµØ¡£
//              ----------------------------------------------------------------
//              ----------------------------------------------------------------
//              GND    µçÔ´µØ
//              VCC  ½Ó5V»ò3.3vµçÔ´
//              SCL   P1^0£¨SCL£©
//              SDA   P1^1£¨SDA£©
//              RES  ½ÓP12
//              DC   ½ÓP13¿ÉÒÔÖ±½Ó½ÓµØ
//              CS   ½ÓP14¿ÉÒÔÖ±½Ó½ÓµØ               
//              ----------------------------------------------------------------
// ĞŞ¸ÄÀúÊ·   :
// ÈÕ    ÆÚ   : 
// ×÷    Õß   : HuangKai
// ĞŞ¸ÄÄÚÈİ   : ´´½¨ÎÄ¼ş
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÖĞ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//******************************************************************************/¡£
#include "REG51.h"
#ifndef __OLED_H
#define __OLED_H			  	 
//#include "sys.h"
//#include "stdlib.h"	  
#define  u8 unsigned char 
#define  u32 unsigned int 
#define OLED_CMD  0	//Ğ´ÃüÁî
#define OLED_DATA 1	//Ğ´Êı¾İ
#define OLED_MODE 0

sbit OLED_CS=P1^4; //Æ¬Ñ¡
sbit OLED_RST =P1^2;//¸´Î»
sbit OLED_DC =P1^3;//Êı¾İ/ÃüÁî¿ØÖÆ
sbit OLED_SCL=P1^0;//Ê±ÖÓ D0£¨SCLK£
sbit OLED_SDIN=P1^1;//D1£¨MOSI£© Êı¾İ


#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1;





//OLEDÄ£Ê½ÉèÖÃ
//0:4Ïß´®ĞĞÄ£Ê½
//1:²¢ĞĞ8080Ä£Ê½

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED¶Ë¿Ú¶¨Òå----------------  					   

void delay_ms(unsigned int ms);


 		     

//OLED¿ØÖÆÓÃº¯Êı
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



