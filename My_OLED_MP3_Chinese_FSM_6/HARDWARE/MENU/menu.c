#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "menu.h"
#include "oled.h"
#include "text.h"
#include "vs10xx.h"
#include "mp3player.h"
//#include "oledfont.h"

extern const unsigned char asc2_0806[10][6];
extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char asc2_2412[95][36];//assic������

/*���ĸ�û��*/
extern unsigned char gImage_bell[32];
extern unsigned char gImage_arrows_right[64];
extern unsigned char gImage_arrows_left[64];
extern unsigned char gImage_explanation[128];

extern unsigned char gImage_timer[72];
extern unsigned char gImage_music_s[32];
extern unsigned char gImage_music_l[72];
extern unsigned char gImage_Game[72];
extern unsigned char gImage_Record[72];
extern unsigned char gImage_txt[72];
extern unsigned char gImage_set[72];

//��������
unsigned char  KeyFuncIndex=0;    //��ŵ�ǰ�Ĳ˵�����
unsigned char  KEY_TYPE=0;		  //��ǰ���µļ�ֵ���ͣ� KEY_TYPE=0����ǰ�ް�������
void (*KeyFuncPtr)(void);            //���尴������ָ��,ָ��һ������  ����ָ��

//////////////////////////////////////////////////////////////////////////////////
/*****************************************************************
����һ���˵��ڵ�Ľṹ������
typedef struct 
{
   unsigned char KeyStatusIndex;   //��ǰ��״̬������
   unsigned char KeyOKStatus;   	//����ȷ�ϼ�ʱ��״̬������
   unsigned char KeyUpStatus;      //�������ϼ�ʱ��״̬������     
   unsigned char KeyDownStatus;    //�������¼�ʱ��״̬������
   unsigned char KeyBackStatus;     //���·��ؼ�ʱ��״̬������
   void (*CurrentOperate)(void);      //��ǰ״̬Ӧ��ִ�еĹ��ܲ���
}StatusTab;
������ֻ��Ϊ�˿�������������
****************************************************************/

//����˵��л�����   �ṹ������
StatusTab KeyTab[MENU_SIZE]=
{
	{0,9,4,1,0,(*InitMenuIterface)},   //��ʼ�����˵�����_����
	{1,5,0,2,0,(*state0)},    				 //����
	{2,10,1,3,0,(*state1)},    				 //¼��
 	{3,11,2,4,0,(*state2)},    				 //ʱ��
	{4,14,3,0,0,(*state3)},    				 //��Ϸ
	
	{5,5,26,6,1,(*state0_1)},    				 //�����Ӳ˵�1���������ã�
	{6,6,5,7,1,(*state0_2)},    				 //�����Ӳ˵�2��ʱ�����ã�
	{7,8,6,19,1,(*state0_3)},    				 //�����Ӳ˵�3���������ã�
	{8,8,8,8,7,(*state0_3_a)},    			 //���������Ӳ˵������ȣ�
	
	{9,9,9,9,0,(*InitMenuIterface_1)},   	//���ֲ��Ž���
	
	{10,10,10,10,2,(*state1_a)},    			//¼���Ӳ˵�
	
	
	{11,11,11,12,3,(*state2_a)},      		//ʱ���Ӳ˵���ģ��ʱ�ӣ�
	{12,12,11,13,3,(*state2_b)},      		//ʱ���Ӳ˵���ʱ�����ã�
	{13,13,12,13,3,(*state2_c)},      		//ʱ���Ӳ˵����������ã�
	
	{14,14,14,15,4,(*state3_a)},      		//̰����
	{15,15,14,16,4,(*state3_b)},      		//����˹���� 
	{16,16,15,16,4,(*state3_c)},      		//̹�˴�ս
	
 	{17,17,5,18,1,(*state0_4)},      		    //�����Ӳ˵���ʱ�����ã�
	{18,18,17,19,1,(*state0_5)},      		  //�����Ӳ˵����������ã�
	{19,19,18,22,1,(*state0_6)},      	    //�����Ӳ˵����������ã�
	{20,20,17,21,1,(*state0_7)},      		  //�����Ӳ˵����������ã�
	{21,21,20,22,1,(*state0_8)},      		  //�����Ӳ˵����������ã�
	{22,22,21,25,1,(*state0_9)},      	    //�����Ӳ˵����������ã�
	{23,23,20,24,1,(*state0_10)},      	    //�����Ӳ˵����������ã�
	{24,24,23,7,1,(*state0_11)},      		  //�����Ӳ˵����������ã�
	{25,25,24,28,1,(*state0_12)},      		  //�����Ӳ˵����������ã�
	{26,26,23,27,1,(*state0_13)},      		  //�����Ӳ˵����������ã�
	{27,27,26,28,1,(*state0_14)},      		  //�����Ӳ˵����������ã�
	{28,28,27,7,1,(*state0_15)},      		  //�����Ӳ˵���ʱ�����ã�

};





void MenuOperate(void)		  //���в˵�
{	
    switch(KEY_TYPE) //���ݰ�������������Ӧ���棬ִ����Ӧ�Ĳ���
	{
		case  1:			  
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyOKStatus;  //ȷ�ϰ���
				break; 
		case  2:			 
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpStatus;	//���ϰ���s2
				break; 
	  case  3:		      
		    	KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDownStatus; //���°���s3
				break; 
		case  4:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyBackStatus; //���ذ���
				break; 
		default : 
				break;
				//�˴���Ӱ����������
	}	
/*
ͨ������ָ�룬����KeyFuncIndex=0����KeyTab[KeyFuncIndex]�м�Ϊ���˵����֣�
��ͨ����ֵKeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate���Ӷ��ٵ��ò�������
*/
	KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;	 //������ִ�а����Ĳ���
	(*KeyFuncPtr)();     //ִ�е�ǰ�İ������� 
}	 



void OpenCartoon(void)			 //	 ���ؿ�������
{

	OLED_Fill(0,16,127,63,0);
	
	OLED_DrawPicture(0,16,24,72,gImage_timer,1);
	OLED_DrawPicture(26,16,24,72,gImage_Game,1);
	OLED_DrawPicture(51,16,24,72,gImage_music_l,0);
	OLED_DrawPicture(76,16,24,72,gImage_set,1);
	OLED_DrawPicture(101,16,24,72,gImage_Record,1);
	OLED_Refresh_Gram();
  delay_ms(500);
 	OLED_Fill(35,48,95,63,0);        //����ͨ����ʱ����˸����ͼƬ
	OLED_Refresh_Gram();
	delay_ms(500);
  Show_Str(30,48,80,16,"��ӭ����",16,1);
	OLED_Refresh_Gram();
  delay_ms(1000);	
  OLED_Clear();
}

///////////////////////////////////////////////////////////////////////////////
//��ʼ�����˵����� Ĭ��ʱ��
void InitMenuIterface(void)   //��ʼ�����˵�����	Ĭ��ʱ��
{
  OLED_Fill(0,16,127,63,0);
	
	OLED_DrawPicture(0,16,24,72,gImage_timer,1);
	OLED_DrawPicture(26,16,24,72,gImage_Game,1);
	OLED_DrawPicture(51,16,24,72,gImage_music_l,0);
	OLED_DrawPicture(76,16,24,72,gImage_set,1);
	OLED_DrawPicture(101,16,24,72,gImage_Record,1);
	Show_Str(48,48,40,16,"����",16,1);		
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))//���˰����ذ�����Ч��������������Ч	
		{		
		
			break;	//���������ް������ߣ����·��ؼ�����Ч�����ڵȴ�״̬!
		}
	}
}


void InitMenuIterface_1(void)//���˵������ֱ�����ȷ����
{
	OLED_Fill(0,16,127,63,0);
	while(1)
	{
		
		LED1=0; 	  	
 		VS_Sine_Test();	   	
		LED1=1;
		Show_Str(0,0,100,16,"������ͩ",16,1);
		Show_Str(64,0,30,16,"MP3",16,1);
		OLED_Refresh_Gram();
		mp3_play();
		
	}
	
}

///////////////////////////////////////////////////////////////////////////////
 //������˵�����
void state0(void)				 //����
{
		OLED_Fill(0,16,127,63,0);
	
    OLED_DrawPicture(0,16,24,72,gImage_Game,1);
		OLED_DrawPicture(26,16,24,72,gImage_music_l,1);
		OLED_DrawPicture(51,16,24,72,gImage_set,0);
		OLED_DrawPicture(76,16,24,72,gImage_Record,1);
		OLED_DrawPicture(101,16,24,72,gImage_timer,1);
	
		//OLED_Fill(35,48,95,63,0);
	  Show_Str(48,48,40,16,"����",16,1);	
		//OLED_ShowString(50,48,"Set",16);
	  OLED_Refresh_Gram();
		LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}

}



void state0_1(void)		//�����Ӳ˵�1���������ã�
{
OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,0);
	Show_Str(0,32,127,16,"ʱ������ ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}



void state0_2(void)		//�����Ӳ˵�2��ʱ�����ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,1);
	Show_Str(0,32,127,16,"ʱ������ ",16,0);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)  //�����������ʵ����Ӧ�Ĺ���
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}



void state0_3(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,1);
	Show_Str(0,32,127,16,"ʱ������ ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}




void state0_4(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"ʱ������ ",16,0);
	Show_Str(0,32,127,16,"�������� ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}

void state0_5(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"ʱ������ ",16,1);
	Show_Str(0,32,127,16,"�������� ",16,0);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}

void state0_6(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"ʱ������ ",16,1);
	Show_Str(0,32,127,16,"�������� ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}


void state0_7(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,0);
	Show_Str(0,32,127,16,"�������� ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}


void state0_8(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,1);
	Show_Str(0,32,127,16,"�������� ",16,0);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}


void state0_9(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,1);
	Show_Str(0,32,127,16,"�������� ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}



void state0_10(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,0);
	Show_Str(0,32,127,16,"�������� ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}

void state0_11(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,1);
	Show_Str(0,32,127,16,"�������� ",16,0);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}

void state0_12(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,1);
	Show_Str(0,32,127,16,"�������� ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}

void state0_13(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,0);
	Show_Str(0,32,127,16,"�������� ",16,1);
	Show_Str(0,48,127,16,"ʱ������ ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}


void state0_14(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,1);
	Show_Str(0,32,127,16,"�������� ",16,0);
	Show_Str(0,48,127,16,"ʱ������ ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}


void state0_15(void)		//�����Ӳ˵�3���������ã�
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"�������� ",16,1);
	Show_Str(0,32,127,16,"�������� ",16,1);
	Show_Str(0,48,127,16,"ʱ������ ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}


void state0_3_a(void)        //���������Ӳ˵�3�����ȣ�
{
	OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,32,127,16,"����:  0X10",16,1);

	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//������˵�˵��
void state1(void)				 //¼��
{
	
	OLED_Fill(0,16,127,63,0);
	
	OLED_DrawPicture(0,16,24,72,gImage_music_l,1);
	OLED_DrawPicture(26,16,24,72,gImage_set,1);
	OLED_DrawPicture(51,16,24,72,gImage_Record,0);
	OLED_DrawPicture(76,16,24,72,gImage_timer,1);
	OLED_DrawPicture(101,16,24,72,gImage_Game,1);
	
	//OLED_Fill(35,48,95,63,0);
	Show_Str(48,48,40,16,"¼��",16,1);	
  //OLED_ShowString(40,48,"Record",16);
	OLED_Refresh_Gram();
	LED0=!LED0;
	
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		{	
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
	

}




void state1_a(void)		//¼��
{
OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"¼���ļ� ",16,0);
//Show_Str(0,32,127,16,"ʱ������ ",16,1);
//Show_Str(0,48,127,16,"�������� ",16,1);
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}

}


///////////////////////////////////////////////////////////////////////////////
//ʱ��ѡ�����
void state2(void)		//ʱ��
{
	OLED_Fill(0,16,127,63,0);
	
	OLED_DrawPicture(0,16,24,72,gImage_set,1);
	OLED_DrawPicture(26,16,24,72,gImage_Record,1);
	OLED_DrawPicture(51,16,24,72,gImage_timer,0);
	OLED_DrawPicture(76,16,24,72,gImage_Game,1);
	OLED_DrawPicture(101,16,24,72,gImage_music_l,1);
	
//	OLED_Fill(35,48,95,63,0);
	Show_Str(48,48,40,16,"ʱ��",16,1);	
 	//OLED_ShowString(47,48,"Time",16);
	OLED_Refresh_Gram();
	LED0=!LED0;
	
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		{	
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}



	
void state2_a(void)
{
OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"ģ��ʱ�� ",16,0);
	Show_Str(0,32,127,16,"ʱ������ ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}
void state2_b(void)
{
    OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"ģ��ʱ�� ",16,1);
	Show_Str(0,32,127,16,"ʱ������ ",16,0);
	Show_Str(0,48,127,16,"�������� ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}


void state2_c(void)
{
    OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"ģ��ʱ�� ",16,1);
	Show_Str(0,32,127,16,"ʱ������ ",16,1);
	Show_Str(0,48,127,16,"�������� ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
	
}


void state3(void)		//��Ϸ
{
  OLED_Fill(0,16,127,63,0);
	
	
	OLED_DrawPicture(0,16,24,72,gImage_Record,1);
	OLED_DrawPicture(26,16,24,72,gImage_timer,1);
	OLED_DrawPicture(51,16,24,72,gImage_Game,0);
	OLED_DrawPicture(76,16,24,72,gImage_music_l,1);
	OLED_DrawPicture(101,16,24,72,gImage_set,1);
	
	
	//OLED_Fill(35,48,95,63,0);
	Show_Str(48,48,40,16,"��Ϸ",16,1);	
  //OLED_ShowString(47,48,"Game",16);
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}



void state3_a(void)
{
	OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"̰����     ",16,0);
	Show_Str(0,32,127,16,"����˹���� ",16,1);
	Show_Str(0,48,127,16,"̹�˴�ս   ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}
void state3_b(void)
{
	OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"̰����     ",16,1);
	Show_Str(0,32,127,16,"����˹���� ",16,0);
	Show_Str(0,48,127,16,"̹�˴�ս   ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}
void state3_c(void)
{
	OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"̰����     ",16,1);
	Show_Str(0,32,127,16,"����˹���� ",16,1);
	Show_Str(0,48,127,16,"̹�˴�ս   ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//�ް������ߣ����·��ؼ��� ����Ч�����ڵȴ�״̬!
		}
	}
}






