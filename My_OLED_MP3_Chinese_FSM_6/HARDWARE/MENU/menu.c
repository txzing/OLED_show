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
extern const unsigned char asc2_2412[95][36];//assic编码组

/*这四个没有*/
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

//变量定义
unsigned char  KeyFuncIndex=0;    //存放当前的菜单索引
unsigned char  KEY_TYPE=0;		  //当前按下的键值类型， KEY_TYPE=0代表当前无按键按下
void (*KeyFuncPtr)(void);            //定义按键功能指针,指向一个函数  函数指针

//////////////////////////////////////////////////////////////////////////////////
/*****************************************************************
定义一个菜单节点的结构体链表
typedef struct 
{
   unsigned char KeyStatusIndex;   //当前的状态索引号
   unsigned char KeyOKStatus;   	//按下确认键时的状态索引号
   unsigned char KeyUpStatus;      //按下向上键时的状态索引号     
   unsigned char KeyDownStatus;    //按下向下键时的状态索引号
   unsigned char KeyBackStatus;     //按下返回键时的状态索引号
   void (*CurrentOperate)(void);      //当前状态应该执行的功能操作
}StatusTab;
放这里只是为了看代码理解更方便
****************************************************************/

//定义菜单切换链表   结构体数组
StatusTab KeyTab[MENU_SIZE]=
{
	{0,9,4,1,0,(*InitMenuIterface)},   //初始化主菜单界面_音乐
	{1,5,0,2,0,(*state0)},    				 //设置
	{2,10,1,3,0,(*state1)},    				 //录音
 	{3,11,2,4,0,(*state2)},    				 //时钟
	{4,14,3,0,0,(*state3)},    				 //游戏
	
	{5,5,26,6,1,(*state0_1)},    				 //设置子菜单1（音乐设置）
	{6,6,5,7,1,(*state0_2)},    				 //设置子菜单2（时间设置）
	{7,8,6,19,1,(*state0_3)},    				 //设置子菜单3（背光设置）
	{8,8,8,8,7,(*state0_3_a)},    			 //背光设置子菜单（亮度）
	
	{9,9,9,9,0,(*InitMenuIterface_1)},   	//音乐播放界面
	
	{10,10,10,10,2,(*state1_a)},    			//录音子菜单
	
	
	{11,11,11,12,3,(*state2_a)},      		//时钟子菜单（模拟时钟）
	{12,12,11,13,3,(*state2_b)},      		//时钟子菜单（时间设置）
	{13,13,12,13,3,(*state2_c)},      		//时钟子菜单（闹钟设置）
	
	{14,14,14,15,4,(*state3_a)},      		//贪吃蛇
	{15,15,14,16,4,(*state3_b)},      		//俄罗斯方块 
	{16,16,15,16,4,(*state3_c)},      		//坦克大战
	
 	{17,17,5,18,1,(*state0_4)},      		    //设置子菜单（时间设置）
	{18,18,17,19,1,(*state0_5)},      		  //设置子菜单（背光设置）
	{19,19,18,22,1,(*state0_6)},      	    //设置子菜单（语言设置）
	{20,20,17,21,1,(*state0_7)},      		  //设置子菜单（背光设置）
	{21,21,20,22,1,(*state0_8)},      		  //设置子菜单（语言设置）
	{22,22,21,25,1,(*state0_9)},      	    //设置子菜单（字体设置）
	{23,23,20,24,1,(*state0_10)},      	    //设置子菜单（语言设置）
	{24,24,23,7,1,(*state0_11)},      		  //设置子菜单（字体设置）
	{25,25,24,28,1,(*state0_12)},      		  //设置子菜单（音乐设置）
	{26,26,23,27,1,(*state0_13)},      		  //设置子菜单（字体设置）
	{27,27,26,28,1,(*state0_14)},      		  //设置子菜单（音乐设置）
	{28,28,27,7,1,(*state0_15)},      		  //设置子菜单（时间设置）

};





void MenuOperate(void)		  //运行菜单
{	
    switch(KEY_TYPE) //根据按键索引进入相应界面，执行相应的操作
	{
		case  1:			  
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyOKStatus;  //确认按键
				break; 
		case  2:			 
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpStatus;	//向上按键s2
				break; 
	  case  3:		      
		    	KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDownStatus; //向下按键s3
				break; 
		case  4:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyBackStatus; //返回按键
				break; 
		default : 
				break;
				//此处添加按键错误代码
	}	
/*
通过函数指针，首先KeyFuncIndex=0，在KeyTab[KeyFuncIndex]中即为主菜单部分，
再通过赋值KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate，从而再调用操作函数
*/
	KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;	 //下面是执行按键的操作
	(*KeyFuncPtr)();     //执行当前的按键操作 
}	 



void OpenCartoon(void)			 //	 加载开机动画
{

	OLED_Fill(0,16,127,63,0);
	
	OLED_DrawPicture(0,16,24,72,gImage_timer,1);
	OLED_DrawPicture(26,16,24,72,gImage_Game,1);
	OLED_DrawPicture(51,16,24,72,gImage_music_l,0);
	OLED_DrawPicture(76,16,24,72,gImage_set,1);
	OLED_DrawPicture(101,16,24,72,gImage_Record,1);
	OLED_Refresh_Gram();
  delay_ms(500);
 	OLED_Fill(35,48,95,63,0);        //可以通过延时，闪烁，画图片
	OLED_Refresh_Gram();
	delay_ms(500);
  Show_Str(30,48,80,16,"欢迎光临",16,1);
	OLED_Refresh_Gram();
  delay_ms(1000);	
  OLED_Clear();
}

///////////////////////////////////////////////////////////////////////////////
//初始化主菜单界面 默认时钟
void InitMenuIterface(void)   //初始化主菜单界面	默认时钟
{
  OLED_Fill(0,16,127,63,0);
	
	OLED_DrawPicture(0,16,24,72,gImage_timer,1);
	OLED_DrawPicture(26,16,24,72,gImage_Game,1);
	OLED_DrawPicture(51,16,24,72,gImage_music_l,0);
	OLED_DrawPicture(76,16,24,72,gImage_set,1);
	OLED_DrawPicture(101,16,24,72,gImage_Record,1);
	Show_Str(48,48,40,16,"音乐",16,1);		
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))//除了按返回按键无效，其他按键都有效	
		{		
		
			break;	//！！！！无按键或者，按下返回键则都无效，处于等待状态!
		}
	}
}


void InitMenuIterface_1(void)//主菜单下音乐被按下确定后
{
	OLED_Fill(0,16,127,63,0);
	while(1)
	{
		
		LED1=0; 	  	
 		VS_Sine_Test();	   	
		LED1=1;
		Show_Str(0,0,100,16,"秋雨梧桐",16,1);
		Show_Str(64,0,30,16,"MP3",16,1);
		OLED_Refresh_Gram();
		mp3_play();
		
	}
	
}

///////////////////////////////////////////////////////////////////////////////
 //主界面菜单设置
void state0(void)				 //设置
{
		OLED_Fill(0,16,127,63,0);
	
    OLED_DrawPicture(0,16,24,72,gImage_Game,1);
		OLED_DrawPicture(26,16,24,72,gImage_music_l,1);
		OLED_DrawPicture(51,16,24,72,gImage_set,0);
		OLED_DrawPicture(76,16,24,72,gImage_Record,1);
		OLED_DrawPicture(101,16,24,72,gImage_timer,1);
	
		//OLED_Fill(35,48,95,63,0);
	  Show_Str(48,48,40,16,"设置",16,1);	
		//OLED_ShowString(50,48,"Set",16);
	  OLED_Refresh_Gram();
		LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}

}



void state0_1(void)		//设置子菜单1（音乐设置）
{
OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"音乐设置 ",16,0);
	Show_Str(0,32,127,16,"时间设置 ",16,1);
	Show_Str(0,48,127,16,"背光设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}



void state0_2(void)		//设置子菜单2（时间设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"音乐设置 ",16,1);
	Show_Str(0,32,127,16,"时间设置 ",16,0);
	Show_Str(0,48,127,16,"背光设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)  //在这里面可以实现相应的功能
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}



void state0_3(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"音乐设置 ",16,1);
	Show_Str(0,32,127,16,"时间设置 ",16,1);
	Show_Str(0,48,127,16,"背光设置 ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}




void state0_4(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"时间设置 ",16,0);
	Show_Str(0,32,127,16,"背光设置 ",16,1);
	Show_Str(0,48,127,16,"语言设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}

void state0_5(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"时间设置 ",16,1);
	Show_Str(0,32,127,16,"背光设置 ",16,0);
	Show_Str(0,48,127,16,"语言设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}

void state0_6(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"时间设置 ",16,1);
	Show_Str(0,32,127,16,"背光设置 ",16,1);
	Show_Str(0,48,127,16,"语言设置 ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}


void state0_7(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"背光设置 ",16,0);
	Show_Str(0,32,127,16,"语言设置 ",16,1);
	Show_Str(0,48,127,16,"字体设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}


void state0_8(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"背光设置 ",16,1);
	Show_Str(0,32,127,16,"语言设置 ",16,0);
	Show_Str(0,48,127,16,"字体设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}


void state0_9(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"背光设置 ",16,1);
	Show_Str(0,32,127,16,"语言设置 ",16,1);
	Show_Str(0,48,127,16,"字体设置 ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}



void state0_10(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"语言设置 ",16,0);
	Show_Str(0,32,127,16,"字体设置 ",16,1);
	Show_Str(0,48,127,16,"音乐设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}

void state0_11(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"语言设置 ",16,1);
	Show_Str(0,32,127,16,"字体设置 ",16,0);
	Show_Str(0,48,127,16,"音乐设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}

void state0_12(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"语言设置 ",16,1);
	Show_Str(0,32,127,16,"字体设置 ",16,1);
	Show_Str(0,48,127,16,"音乐设置 ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}

void state0_13(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"字体设置 ",16,0);
	Show_Str(0,32,127,16,"音乐设置 ",16,1);
	Show_Str(0,48,127,16,"时间设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}


void state0_14(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"字体设置 ",16,1);
	Show_Str(0,32,127,16,"音乐设置 ",16,0);
	Show_Str(0,48,127,16,"时间设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}


void state0_15(void)		//设置子菜单3（背光设置）
{
  OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"字体设置 ",16,1);
	Show_Str(0,32,127,16,"音乐设置 ",16,1);
	Show_Str(0,48,127,16,"时间设置 ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}


void state0_3_a(void)        //背光设置子菜单3（亮度）
{
	OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,32,127,16,"亮度:  0X10",16,1);

	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//主界面菜单说明
void state1(void)				 //录音
{
	
	OLED_Fill(0,16,127,63,0);
	
	OLED_DrawPicture(0,16,24,72,gImage_music_l,1);
	OLED_DrawPicture(26,16,24,72,gImage_set,1);
	OLED_DrawPicture(51,16,24,72,gImage_Record,0);
	OLED_DrawPicture(76,16,24,72,gImage_timer,1);
	OLED_DrawPicture(101,16,24,72,gImage_Game,1);
	
	//OLED_Fill(35,48,95,63,0);
	Show_Str(48,48,40,16,"录音",16,1);	
  //OLED_ShowString(40,48,"Record",16);
	OLED_Refresh_Gram();
	LED0=!LED0;
	
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		{	
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
	

}




void state1_a(void)		//录音
{
OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"录音文件 ",16,0);
//Show_Str(0,32,127,16,"时间设置 ",16,1);
//Show_Str(0,48,127,16,"闹钟设置 ",16,1);
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}

}


///////////////////////////////////////////////////////////////////////////////
//时钟选择界面
void state2(void)		//时钟
{
	OLED_Fill(0,16,127,63,0);
	
	OLED_DrawPicture(0,16,24,72,gImage_set,1);
	OLED_DrawPicture(26,16,24,72,gImage_Record,1);
	OLED_DrawPicture(51,16,24,72,gImage_timer,0);
	OLED_DrawPicture(76,16,24,72,gImage_Game,1);
	OLED_DrawPicture(101,16,24,72,gImage_music_l,1);
	
//	OLED_Fill(35,48,95,63,0);
	Show_Str(48,48,40,16,"时钟",16,1);	
 	//OLED_ShowString(47,48,"Time",16);
	OLED_Refresh_Gram();
	LED0=!LED0;
	
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		{	
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}



	
void state2_a(void)
{
OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"模拟时钟 ",16,0);
	Show_Str(0,32,127,16,"时间设置 ",16,1);
	Show_Str(0,48,127,16,"闹钟设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}
void state2_b(void)
{
    OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"模拟时钟 ",16,1);
	Show_Str(0,32,127,16,"时间设置 ",16,0);
	Show_Str(0,48,127,16,"闹钟设置 ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}


void state2_c(void)
{
    OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"模拟时钟 ",16,1);
	Show_Str(0,32,127,16,"时间设置 ",16,1);
	Show_Str(0,48,127,16,"闹钟设置 ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
	
}


void state3(void)		//游戏
{
  OLED_Fill(0,16,127,63,0);
	
	
	OLED_DrawPicture(0,16,24,72,gImage_Record,1);
	OLED_DrawPicture(26,16,24,72,gImage_timer,1);
	OLED_DrawPicture(51,16,24,72,gImage_Game,0);
	OLED_DrawPicture(76,16,24,72,gImage_music_l,1);
	OLED_DrawPicture(101,16,24,72,gImage_set,1);
	
	
	//OLED_Fill(35,48,95,63,0);
	Show_Str(48,48,40,16,"游戏",16,1);	
  //OLED_ShowString(47,48,"Game",16);
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
		if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}



void state3_a(void)
{
	OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"贪吃蛇     ",16,0);
	Show_Str(0,32,127,16,"俄罗斯方块 ",16,1);
	Show_Str(0,48,127,16,"坦克大战   ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}
void state3_b(void)
{
	OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"贪吃蛇     ",16,1);
	Show_Str(0,32,127,16,"俄罗斯方块 ",16,0);
	Show_Str(0,48,127,16,"坦克大战   ",16,1);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}
void state3_c(void)
{
	OLED_Fill(0,16,127,63,0);
	
	Show_Str(0,16,127,16,"贪吃蛇     ",16,1);
	Show_Str(0,32,127,16,"俄罗斯方块 ",16,1);
	Show_Str(0,48,127,16,"坦克大战   ",16,0);
	
	OLED_Refresh_Gram();
	LED0=!LED0;
	while(1)
	{
		KEY_TYPE = KEY_Scan(0);
	//	if((KEY_TYPE!=0)&&(KEY_TYPE!=4))
		if(KEY_TYPE!=0)
		{
			break;	//无按键或者，按下返回键则 都无效，处于等待状态!
		}
	}
}






