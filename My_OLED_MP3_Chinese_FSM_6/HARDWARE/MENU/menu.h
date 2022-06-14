#ifndef __MENU_H
#define __MENU_H

#include "sys.h"

#define MENU_SIZE 50          //菜单长度

////////////////////////////////////////////////////////////////////////
//定义一个菜单节点的结构体链表
typedef struct 
{
   unsigned char KeyStatusIndex;   //当前的状态索引号
   unsigned char KeyOKStatus;   	//按下确认键时的状态索引号
   unsigned char KeyUpStatus;      //按下向上键时的状态索引号     
   unsigned char KeyDownStatus;    //按下向下键时的状态索引号
   unsigned char KeyBackStatus;     //按下返回键时的状态索引号
   void (*CurrentOperate)(void);      //当前状态应该执行的功能操作
}StatusTab;

///////////////////////////////////////////////////////////////////////////
//函数声明
void MenuOperate(void);		  //运行菜单


//函数声明
void OpenCartoon(void);			 //	 加载开机动画
//主菜单界面
void InitMenuIterface(void); //初始化主菜单界面	默认时钟
void state0(void);				 //设置
void state1(void);				 //录音
void state2(void);		    //时钟
void state3(void);		    //游戏

void state0_1(void);		//设置子菜单1（音乐设置）
void state0_2(void);		//设置子菜单2（时间设置）
void state0_3(void);		//设置子菜单3（背光设置）
void state0_4(void);		//设置子菜单4（语言设置）
void state0_5(void);		//设置子菜单5（字体设置）
void state0_6(void);		//设置子菜单1（音乐设置）
void state0_7(void);		//设置子菜单2（时间设置）
void state0_8(void);		//设置子菜单3（背光设置）
void state0_9(void);		//设置子菜单4（语言设置）
void state0_10(void);		//设置子菜单5（字体设置）
void state0_11(void);		//设置子菜单1（音乐设置）
void state0_12(void);		//设置子菜单2（时间设置）
void state0_13(void);		//设置子菜单3（背光设置）
void state0_14(void);		//设置子菜单4（语言设置）
void state0_15(void);		//设置子菜单5（字体设置）
void state0_3_a(void);		//背光设置子菜单3（亮度）
void InitMenuIterface_1(void);
void state2_a(void);
void state2_b(void);
void state2_c(void);
void state3_a(void);
void state3_b(void);
void state3_c(void);
void state1_a(void);		//模拟时钟

#endif
