#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//SSD1306 OLED 驱动IC驱动代码
//驱动方式:8080并口/4线串口
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[128][8];	
//extern u8 light;

//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
#if OLED_MODE==1	//8080并口 
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
 	OLED_RS=cmd;
	OLED_CS=0;	   
	OLED_WR=0;	 
	OLED_WR=1;
	OLED_CS=1;	  
	OLED_RS=1;	 
} 	    	    
#else
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
  OLED_RS=cmd; //写命令 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_RS=1;   	  
} 
#endif
	  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	

	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}


//功能：在指定区域内显示指定图形
//		x_sta,y_sta:绘图开始坐标
//		y_end,y_end:绘图结束坐标
//		x_sta必须小于x_end !!!		y_sta必须小于y_end !!!	
//		x_sta必须能被16整除
//(0,0） ---------------- x+(0~127)
//	    |
//		|
//		|
//		|
//		|
//		y+ (0~63)
//		*p:绘图数据首地址
//0,表示命令;1,表示数据;
//		reverse: 0：绘制区域反白显示 1：绘制区域正常显示

//   一定要是16*16格子,SIZE为图片高度	
void OLED_DrawPicture(u8 x,u8 y,u8 high,u8 csize,u8 *p,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	//u8 csize=32;		//得到字体一个字符对应点阵集所占的字节数
	 
		for(t=0;t<csize;t++)
		{   
			temp = p[t];
		
				for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==high)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
		}          
}
//画垂线
//x:0~127
//y:0~63
//color:1 填充 0,清空	
void OLED_DrawVerticalLine(u8 y0,u8 y1,u8 x,u8 color)
{
	for(;y0<y1+1;y0++)
	OLED_DrawPoint(x,y0,color);
}



//画水平线
//x:0~127
//y:0~63
//color:1 填充 0,清空	
void OLED_DrawLevelLine(u8 x0,u8 x1,u8 y,u8 color)
{
	for(;x0<x1+1;x0++)
	OLED_DrawPoint(x0,y,color);
}


//参数说明
//起点 STARTX,STARTY 终点ENDX,ENDY
void DrawAnyLine(u8 StartX,u8 StartY,u8 EndX,u8 EndY,u8 color )  //画任意直线(bresenham算法)
{ 
    int t, distance;      
    int x = 0 , y = 0 , delta_x, delta_y ; 
    signed char incx, incy ; 
    delta_x = EndX - StartX ; 
    delta_y = EndY - StartY ; 
    if( delta_x > 0 ) 
    { 
        incx = 1; 
    } 
    else if( delta_x == 0 ) 
    { 				   
        OLED_DrawVerticalLine( StartX, StartY, EndY,color) ; 	//画垂直线
    } 
    else 
    { 
        incx = -1 ; 
    } 
    if( delta_y > 0 ) 
    { 
        incy = 1 ; 
    } 
    else if(delta_y == 0 ) 	  //画水平线
    { 
        OLED_DrawLevelLine( StartX, EndX, StartY,color) ;     
    } 
    else 
    { 
        incy = -1 ; 
    } 
    delta_x = fabs( delta_x );     
    delta_y = fabs( delta_y ); 
    if( delta_x > delta_y ) 
    { 
        distance = delta_x ; 
    } 
    else 
    { 
        distance = delta_y ;  
    } 
 //   LCD12864DrawDot( StartX, StartY,1 ) ;     
    /* Draw Line*/ 
    for( t = 0 ; t <= distance+1; t++ ) 
    { 
         OLED_DrawPoint( StartX, StartY,color ) ;  
        x += delta_x ; 
        y += delta_y ; 
        if( x > distance ) 
        { 
            x -= distance ; 
            StartX += incx ; 
        } 
        if( y > distance ) 
        { 
            y -= distance ; 
            StartY += incy ; 
        } 
    } 
} 




//画圆
//x:0~127
//y:0~63
void DrawCircle(int xc,int yc,int r)	
{
    int x,y,d;
    y = r;
    d = 3 - (r + r);
    x = 0;
    while(x <= y)
    {
        OLED_DrawPoint(xc+x,yc+y,1);
  		OLED_DrawPoint(xc+x,yc-y,1);
   		OLED_DrawPoint(xc-x,yc+y,1);
   		OLED_DrawPoint(xc-x,yc-y,1);
   		OLED_DrawPoint(xc+y,yc+x,1);
   		OLED_DrawPoint(xc+y,yc-x,1);
   		OLED_DrawPoint(xc-y,yc+x,1);
   		OLED_DrawPoint(xc-y,yc-x,1);

        if(d < 0)
            d += (x + x + x + x) + 6;
        else
        {
            d+=((x - y) + (x - y) + (x - y) + (x - y)) + 10;
            y = y - 1;
        }
        x = x + 1;
    }
}


//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}		


void OLED_Show_Mini_Num(u8 x,u8 y,u32 num)
{
	u8 j,n;
 
	for(j=0;j<6;j++)
	{
		u8 m=0x80;
		for(n=0;n<8;n++)
		{
		
			if(asc2_0806[num][j]&m)
			{
				
				OLED_DrawPoint(x+j,y+n,1);
				m>>=1;
			}
			else{
				
				OLED_DrawPoint(x+j,y+n,0);
				m>>=1;
			}
		}
	}
	
}


//显示多个个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	OLED_Refresh_Gram();//更新显示		
}	   

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}

void battery(void)
{
  OLED_Fill(110,2,111,5,1);
	OLED_Fill(113,2,114,5,1);	
	OLED_Fill(116,2,117,5,1);
	OLED_Fill(119,2,120,5,1);
	OLED_Fill(122,2,123,5,1);
		
		
		
	OLED_Fill(125,1,126,6,1);	
	OLED_Fill(105,2,108,5,1);
	DrawAnyLine(108,7,127,7,1);
	DrawAnyLine(108,0,127,0,1);
	DrawAnyLine(127,0,127,7,1);	
	DrawAnyLine(108,0,108,7,1);
	OLED_Refresh_Gram();
}



//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOF, ENABLE);	 //使能PC,D,G端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 //clk , mosi
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化
 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);	//输出高

 #if OLED_MODE==1
 
 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT推挽输出
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,0xFF); //PC0~7输出高

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;				 //PG13,14,15 OUT推挽输出
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);						 //PG13,14,15 OUT  输出高

 #else
 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_13;				 //PC0,1 OUT推挽输出
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOF,GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_13);						 //PC0,1 OUT  输出高

 #endif
  							  
	OLED_CS=1;
	OLED_RS=1;	 
	
	OLED_RST=1;
	delay_ms(100);
	OLED_RST=0; 
	delay_ms(100);
	OLED_RST=1;
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(0x80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0

	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址a模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
		 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0x6F,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	
	
	OLED_Clear();
	
	OLED_Set_Pos(0,0); 
					
}  





























