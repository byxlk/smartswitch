
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


/*************	功能说明	**************

本文件为STC15xxx系列的延时程序,用户几乎可以不修改这个程序.


******************************************/

#include	"delay.h"


#if (MAIN_Fosc == 12000000L)
//========================================================================
// 函数: void  delay_ms(unsigned char ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
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
