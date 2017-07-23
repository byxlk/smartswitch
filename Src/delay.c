
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


#if (MAIN_Fosc == 12000000L)
//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��, ����ֻ֧��1~255ms. �Զ���Ӧ��ʱ��.
// ����: none.
// �汾: VER1.0
// ����: 2013-4-1
// ��ע: 
//========================================================================
void  delay_ms(unsigned char ms) //@11.0592MHz
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}
#elif (MAIN_Fosc == 24000000L)
void  delay_ms(unsigned char ms) //@24.0000MHz
{
    unsigned char i, j;
    do {
        i = 24;
        j = 85;
        do
        {
            while (--j);
        } while (--i);
    } while(--ms);
}
#endif

#if (MAIN_Fosc == 12000000L)
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
#elif (MAIN_Fosc == 24000000L)
void delay_4000ms(void)		//@24.0000MHz
{
    unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 109;
	j = 200;
	k = 199;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
#endif
