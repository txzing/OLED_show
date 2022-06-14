#ifndef __MENU_H
#define __MENU_H

#include "sys.h"

#define MENU_SIZE 50          //�˵�����

////////////////////////////////////////////////////////////////////////
//����һ���˵��ڵ�Ľṹ������
typedef struct 
{
   unsigned char KeyStatusIndex;   //��ǰ��״̬������
   unsigned char KeyOKStatus;   	//����ȷ�ϼ�ʱ��״̬������
   unsigned char KeyUpStatus;      //�������ϼ�ʱ��״̬������     
   unsigned char KeyDownStatus;    //�������¼�ʱ��״̬������
   unsigned char KeyBackStatus;     //���·��ؼ�ʱ��״̬������
   void (*CurrentOperate)(void);      //��ǰ״̬Ӧ��ִ�еĹ��ܲ���
}StatusTab;

///////////////////////////////////////////////////////////////////////////
//��������
void MenuOperate(void);		  //���в˵�


//��������
void OpenCartoon(void);			 //	 ���ؿ�������
//���˵�����
void InitMenuIterface(void); //��ʼ�����˵�����	Ĭ��ʱ��
void state0(void);				 //����
void state1(void);				 //¼��
void state2(void);		    //ʱ��
void state3(void);		    //��Ϸ

void state0_1(void);		//�����Ӳ˵�1���������ã�
void state0_2(void);		//�����Ӳ˵�2��ʱ�����ã�
void state0_3(void);		//�����Ӳ˵�3���������ã�
void state0_4(void);		//�����Ӳ˵�4���������ã�
void state0_5(void);		//�����Ӳ˵�5���������ã�
void state0_6(void);		//�����Ӳ˵�1���������ã�
void state0_7(void);		//�����Ӳ˵�2��ʱ�����ã�
void state0_8(void);		//�����Ӳ˵�3���������ã�
void state0_9(void);		//�����Ӳ˵�4���������ã�
void state0_10(void);		//�����Ӳ˵�5���������ã�
void state0_11(void);		//�����Ӳ˵�1���������ã�
void state0_12(void);		//�����Ӳ˵�2��ʱ�����ã�
void state0_13(void);		//�����Ӳ˵�3���������ã�
void state0_14(void);		//�����Ӳ˵�4���������ã�
void state0_15(void);		//�����Ӳ˵�5���������ã�
void state0_3_a(void);		//���������Ӳ˵�3�����ȣ�
void InitMenuIterface_1(void);
void state2_a(void);
void state2_b(void);
void state2_c(void);
void state3_a(void);
void state3_b(void);
void state3_c(void);
void state1_a(void);		//ģ��ʱ��

#endif
