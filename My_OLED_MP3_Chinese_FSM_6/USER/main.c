
#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "oled.h"
#include "menu.h"
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h" 
#include "vs10xx.h"
#include "mp3player.h"	
 
/******************************************************
OLED�༶�˵�
*******************************************************/
 int main(void)
 {	 
 
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	usmart_dev.init(72);		//��ʼ��USMART	
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	OLED_Init();			   		//��ʼ��LCD     
	W25QXX_Init();				//��ʼ��W25Q128
 	VS_Init();	  				//��ʼ��VS1053 
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH.
  OLED_Clear();
#if  1    
 	while(font_init()) 				//����ֿ�
	{	    
		LED0=!LED0;	
		delay_ms(200);				     
	}
#endif
	
  //OpenCartoon();//�ȼ��ؿ�������
	
	while(1)
	{
	//	battery();
	//	MenuOperate();
	} 	   										    
}































//#include "sys.h"
//#include "delay.h"
//#include "usart.h" 
//#include "led.h" 	
//#include "oled.h"
////#include "lcd.h"  
//#include "menu.h"
//#include "key.h"     
//#include "usmart.h" 
//#include "malloc.h"
//#include "sdio_sdcard.h" 
//#include "w25qxx.h"    
//#include "ff.h"  
//#include "exfuns.h"   
//#include "text.h"
//#include "vs10xx.h"	
//#include "mp3player.h"

// u8 light=0x10;			//����oled����ȫ�ֱ���
// 
///************************************************
// ALIENTEKս��STM32������ʵ��40
// ������ʾ ʵ�� 
// ����֧�֣�www.openedv.com
// �Ա����̣�http://eboard.taobao.com 
// ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
// ������������ӿƼ����޹�˾  
// ���ߣ�����ԭ�� @ALIENTEK
//************************************************/


// int main(void)
// {	    
//	 
//	delay_init();	    	 //��ʱ������ʼ��	  
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
// 	usmart_dev.init(72);		//��ʼ��USMART	
//  	 
// 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
//	KEY_Init();					//��ʼ������
//    OLED_Init();
//	//LCD_Init();			   		//��ʼ��LCD   
//	W25QXX_Init();				//��ʼ��W25Q128
//	VS_Init();	  				//��ʼ��VS1053 
// 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
//	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
// 	f_mount(fs[0],"0:",1); 		//����SD�� 
// 	f_mount(fs[1],"1:",1); 		//����FLASH.
//	 
//	 
//	while(font_init()) //����ֿ�
//	{
//    	LED0=!LED0;		
//	    	delay_ms(200);
//	} 

//	
////  Show_Str(0,0,100,16,"�����",16,1);	
//////Show_Str(0,16,100,12,"etyruuu",12,1);	
//////OLED_ShowString(0,0,"ABCDEFGH",12);
//////OLED_ShowString(0,12,"ABCDEFGH",24); 
//////OLED_ShowChar(0,0,'F',12,1);
////	OLED_Refresh_Gram();	
// 
//	while(1)
//	{
//		LED1=0; 	  
//		//Show_Str(30,170,200,16,"�洢������...",16,0);
//		//printf("Ram Test:0X%04X\r\n",VS_Ram_Test());//��ӡRAM���Խ��	    
//		//Show_Str(30,170,200,16,"���Ҳ�����...",16,0); 	 	 
// 		VS_Sine_Test();	   
//		//Show_Str(30,170,200,16,"<<���ֲ�����>>",16,0); 		 
//		LED1=1;
//		mp3_play();
//		//MenuOperate();
//	} 
//}

















