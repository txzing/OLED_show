#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"	 	
#include "text.h"  
#include "oled.h"
 
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)Show_Str,"void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)", 
		
	(void*)delay_ms,"void delay_ms(u16 nms)",
	(void*)delay_us,"void delay_us(u32 nus)",	
	(void*)OLED_ShowString,"void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)",
	(void*)OLED_DrawPoint,"void OLED_DrawPoint(x,y,mode)",
	(void*)OLED_DrawVerticalLine,"void OLED_DrawVerticalLine(u8 y0,u8 y1,u8 x,u8 color)",	
	(void*)OLED_DrawLevelLine,"void OLED_DrawLevelLine(u8 x0,u8 x1,u8 y,u8 color)",
//	(void*)OLED_DrawPicture,"void OLED_DrawPicture(u8 x,u8 y,u8 high,u8 csize,u8 *p,u8 mode)",
	(void*)OLED_ShowChar,"void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);",
	(void*)OLED_ShowNum,"void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);",
		
	(void*)DrawAnyLine,"void DrawAnyLine(u8 StartX,u8 StartY,u8 EndX,u8 EndY,u8 color );",
	(void*)DrawCircle,"void DrawCircle(int xc,int yc,int r)",
	(void*)OLED_Fill,"void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)",
		
	(void*)OLED_Clear,"void OLED_Clear()",
		
	(void*)OLED_Show_Mini_Num,"void OLED_Show_Mini_Num(u8 x,u8 y,u32 num)",
			  	    
	(void*)OLED_Refresh_Gram,"void OLED_Refresh_Gram()",
};		
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















