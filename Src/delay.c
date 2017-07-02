
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


/*************	����˵��	**************

���ļ�ΪSTC15xxxϵ�е���ʱ����,�û��������Բ��޸��������.


******************************************/

#include	"delay.h"



//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  delay_ms(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}
#if 0
void delay_sec(unsigned char sec)
{
     unsigned int i;
     unsigned int sec_count;
    
     sec_count = sec * 200;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
          i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
          i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
          i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
          i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--sec_count);
}

void delay_sec(unsigned char sec)
{
     unsigned char i;
     for(i = 0; i < sec; i++)
	 {
	      delay_ms(250);
          delay_ms(250);
          delay_ms(250);
          delay_ms(250);
     }
}
#endif

void delay_4000ms(void)		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 169;
	j = 24;
	k = 59;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

