#include "config.h"
#include "misc.h"
#include "delay.h"
#include "USART1.h"

void init_Watch_Dog(void)
{
		//
		WDT_CONTR = 0x3A;  //0011 1010
}

void setSystemSleepFlag(bit sleep)
{
		SystemSleepStatus = (sleep)? FALSE : TRUE ;
}

void System_PowerDown(void)
{
    //PCON |= 2	//MCU 进入 睡眠 模式
    LOGD("System entery Power Down mode ...\r\n\r\n");
		setSystemSleepFlag(TRUE);
	  _nop_();
    _nop_();
    _nop_();
    MCU_POWER_DOWN();
    _nop_();    // 唤醒后首先执行此语句，再进入中断服务子程序
    _nop_();
    _nop_();
    _nop_();
}

bit POF_Boot_Delay(void)
{
    if(PCON & 0x10)
    {
				LOGD("Cold reset, delay 4s entery system ...\r\n");
        delay_4000ms()	; //boot delay 4s
        PCON &= 0xEF; //清零POF寄存器 
				
				return TRUE;
    }
		else
				return FALSE;
}

void Reboot_System(void)
{
    //LOGD("Reboot System ...\n");
	//if run to here, system must be reboot now from user space
	if(IAP_CONTR & 0x40)
		IAP_CONTR &= 0x3F; // 0011 1111
	if(!(IAP_CONTR & 0x20))
		IAP_CONTR |= 0x60; // 0010 0000
}

unsigned char setMontorRunningStatus(unsigned char runStat)
{
    unsigned char MotorRunStatus = MONTOR_STOP_RUNNING;
    
    if(MONTOR_RIGHT_RUNNING == runStat) //正转
    {
        MotorRunningCtrl_R = 1;
        MotorRunningCtrl_L = 0;
        MotorRunStatus = MONTOR_RIGHT_RUNNING;
        delay_ms(1);
        LOGD("[Motor status] ===> \r\n");
    }
    else if(MONTOR_LEFT_RUNNING == runStat) //反转
    {
        MotorRunningCtrl_R = 0;
        MotorRunningCtrl_L = 1;
        MotorRunStatus = MONTOR_LEFT_RUNNING;
        delay_ms(1);
        LOGD("[Motor status] <=== \r\n");
    }
    else //停转
    {
        MotorRunningCtrl_R = 1;
        MotorRunningCtrl_L = 1;
        MotorRunStatus = MONTOR_STOP_RUNNING;
        delay_ms(1);
        LOGD("[Motor status] Stop.\r\n\r\n");
    }
    
    return MotorRunStatus;
}

#ifdef DEBUG
void PrintSameString(u8 *puts, unsigned char nSize)
{
	unsigned char i = 0;
	if(nSize > 0)
	{ 
		for(i = 0; i < nSize; i++) 
			LOGD(puts);
		LOGD("\r\n");
	}
}
#endif

void PrintSystemInfoToSerial(void)
{	
#ifdef DEBUG
	PrintSameString("*", 62);	   
	PrintSameString(" ", 1);
	LOGD("        湖北盛佳电器设备有限公司 - 智能自动合闸控制系统\r\n");
	PrintSameString(" ", 1);
	PrintSameString(" ", 1);
	//LOGD("  Project Name  : Smart Switch Control System\r\n");
	LOGD("  Version No.   : Ver 1.0\r\n");
	LOGD("  Designed Time : 2016-03-21\r\n");
	//LOGD("  Auth Name     : Li Shuliang\r\n");
	//LOGD("  Auth Email    : 313424410@qq.com\r\n");
	//LOGD("  Project Owner : SJDQ\r\n");
	LOGD("  Telephone     : 027 - 83520066 / 83567077\r\n");
	PrintSameString(" ", 1);
	LOGD("        All rights reserved (c)2016  http://www.sjdq.com/\r\n");
	PrintSameString(" ", 1);
    PrintSameString("*", 62);
#endif
}

////////////////////////////////////////////////////////////////////////