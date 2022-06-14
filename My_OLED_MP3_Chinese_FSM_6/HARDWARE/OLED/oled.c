#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//SSD1306 OLED ����IC��������
//������ʽ:8080����/4�ߴ���
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//OLED���Դ�
//��Ÿ�ʽ����.
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

//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
#if OLED_MODE==1	//8080���� 
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
  OLED_RS=cmd; //д���� 
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
	  	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	

	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}


//���ܣ���ָ����������ʾָ��ͼ��
//		x_sta,y_sta:��ͼ��ʼ����
//		y_end,y_end:��ͼ��������
//		x_sta����С��x_end !!!		y_sta����С��y_end !!!	
//		x_sta�����ܱ�16����
//(0,0�� ---------------- x+(0~127)
//	    |
//		|
//		|
//		|
//		|
//		y+ (0~63)
//		*p:��ͼ�����׵�ַ
//0,��ʾ����;1,��ʾ����;
//		reverse: 0���������򷴰���ʾ 1����������������ʾ

//   һ��Ҫ��16*16����,SIZEΪͼƬ�߶�	
void OLED_DrawPicture(u8 x,u8 y,u8 high,u8 csize,u8 *p,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	//u8 csize=32;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	 
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
//������
//x:0~127
//y:0~63
//color:1 ��� 0,���	
void OLED_DrawVerticalLine(u8 y0,u8 y1,u8 x,u8 color)
{
	for(;y0<y1+1;y0++)
	OLED_DrawPoint(x,y0,color);
}



//��ˮƽ��
//x:0~127
//y:0~63
//color:1 ��� 0,���	
void OLED_DrawLevelLine(u8 x0,u8 x1,u8 y,u8 color)
{
	for(;x0<x1+1;x0++)
	OLED_DrawPoint(x0,y,color);
}


//����˵��
//��� STARTX,STARTY �յ�ENDX,ENDY
void DrawAnyLine(u8 StartX,u8 StartY,u8 EndX,u8 EndY,u8 color )  //������ֱ��(bresenham�㷨)
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
        OLED_DrawVerticalLine( StartX, StartY, EndY,color) ; 	//����ֱ��
    } 
    else 
    { 
        incx = -1 ; 
    } 
    if( delta_y > 0 ) 
    { 
        incy = 1 ; 
    } 
    else if(delta_y == 0 ) 	  //��ˮƽ��
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




//��Բ
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


//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
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
//m^n����
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


//��ʾ���������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		
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
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	OLED_Refresh_Gram();//������ʾ		
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



//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 //clk , mosi
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��
 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);	//�����

 #if OLED_MODE==1
 
 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT�������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,0xFF); //PC0~7�����

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;				 //PG13,14,15 OUT�������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);						 //PG13,14,15 OUT  �����

 #else
 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_13;				 //PC0,1 OUT�������
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOF,GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_13);						 //PC0,1 OUT  �����

 #endif
  							  
	OLED_CS=1;
	OLED_RS=1;	 
	
	OLED_RST=1;
	delay_ms(100);
	OLED_RST=0; 
	delay_ms(100);
	OLED_RST=1;
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(0x80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַaģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0x6F,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	
	
	OLED_Clear();
	
	OLED_Set_Pos(0,0); 
					
}  





























