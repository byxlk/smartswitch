
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


#include  "config.h"
#include  "USART1.h"
#include  "delay.h"
#include  "GPIO.h"
#include  "timer.h"
#include  "misc.h"
#include  "Exti.h"


/*************	功能说明	**************

双串口全双工中断方式收发通讯程序。

通过PC向MCU发送数据, MCU收到后通过串口把收到的数据原样返回.

******************************************/

/*************	本地常量声明	**************/

/*************	本地变量声明	**************/
//电机控制相关
static unsigned char MotorRunningCtrl_Running = MONTOR_STOP_RUNNING;//电机控制 -- 不转

//定时计数相关
unsigned char TimeoutCount = STOP_TIMERCOUNT;
unsigned char sysClock = STOP_TIMERCOUNT;

//系统工作模式
//bit lastSystemWorkMode = AUTO_CTRL_MODE;
bit firstSystemBoot = TRUE;


/*************	本地函数声明	**************/

/*************  外部函数和变量声明 *****************/


/*************  串口1初始化函数 *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 38400ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2
}

/******************** IO配置函数 **************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		            //结构定义

	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化
	
	GPIO_InitStructure.Pin  = GPIO_Pin_4 ;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化
	
	GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_7;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	
}

/************************ 定时器配置 ****************************/
void	Timer_Config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (12 * 20));	//初值, 节拍为20HZ(50ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
}

void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//结构定义

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE?DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
	
	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE?DISABLE
	Ext_Inilize(EXT_INT1,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}

static void delay_timer(unsigned char iTime)
{
    TimeoutCount = TIMECOUNTER_START;//开始计时
    while(TimeoutCount <= iTime) ;//等1秒
    TimeoutCount = STOP_TIMERCOUNT;//停止计时  
}

static void wait_switch_on(unsigned char time_out)
{
    //等待电机转动到位，超过5秒就认为是超时
    TimeoutCount = TIMECOUNTER_START;//开始计时
    while(SwitchOnStatus != SWITCH_ON_OK)
    {        
        if(TimeoutCount > time_out)
        {
            LOGD("[TimeOut] Wait Montor <=== timeout 5 Sec.");
            break;                        
        }
        //if(SwitchOnStatus == SWITCH_ON_OK)
        //    delay_ms(50);
    }             
    TimeoutCount = STOP_TIMERCOUNT;//停止计时
}

static void wait_out_off_hook(unsigned char time_out)
{
    //等待电机转动到位，超过5秒就认为是超时
    TimeoutCount = TIMECOUNTER_START;//开始计时
    while(OutOffHookCheck != OUT_OF_HOOK)
    {
        if(TimeoutCount > time_out)
        {
            LOGD("[TimeOut] Wait Montor <=== timeout 5 Sec.");
            break;                        
        }
        //if(OutOffHookCheck != OUT_OF_HOOK)
        //    delay_ms(50);
    }                 
    TimeoutCount = STOP_TIMERCOUNT;//停止计时
}
void print_signal_status_info(void)
{
     //LOGD("[SignalStatus]");
     if(ExAutoCtrlSignal == EX_CTRL_SIGNAL_LOSS)
         LOGD("[ Ext Ctrl Signal] LOSS.\r\n");
     else
         LOGD("[ Ext Ctrl Signal] OK.\r\n");
     if(SystemWorkMode == AUTO_CTRL_MODE)
         LOGD("[System Work Mode] AUTO Mode.\r\n");
     else 
         LOGD("[System Work Mode] MANUAL Mode.\r\n");
     if(OutOffHookCheck == OUT_OF_HOOK)
         LOGD("[    Out Off Hook] YES.\r\n");
     else
         LOGD("[    Out Off Hook] NO. \r\n");
     if(SwitchOnStatus == SWITCH_ON_OK)
         LOGD("[   Switch Status] ON. \r\n");
     else
         LOGD("[   Switch Status] OFF. \r\n");
}

/**********************************************/
void main(void)
{
	  //硬件初始化操作
		EA = 0;
    GPIO_Config();//GPIO init
	  setSystemSleepFlag(FALSE);
	
    EXTI_config();
    UART_config(); //UART init
    Timer_Config();//Timer init
		
		//开总中断
    EA = 1;

		//系统断电重启延时4秒启动
    firstSystemBoot = POF_Boot_Delay();
	 
		if(firstSystemBoot)
				PrintSystemInfoToSerial();
	  //LOGD("=====================> Hardware Init Ok <=====================\r\n");

	  //watch dog init
	  init_Watch_Dog();
    LOGD("\r\n\r\n==================================================\r\n");
	  LOGD("System Init Ok, Start Watch Dog OK ... \r\n");
    LOGD("==================================================\r\n");
    
    
    
	//=======================> Start Main Process <==========================//  
	while (1)
	{
        //-------------------------------------------------------------------------------------
        //检查外部控制信号 ---- 丢失
        if(ExAutoCtrlSignal == EX_CTRL_SIGNAL_LOSS)
        {
            //等50毫秒后再次确认信号状态
            //delay_ms(50);
            delay_timer(TIMEOUT_VAL_1S) ;
            if(ExAutoCtrlSignal != EX_CTRL_SIGNAL_LOSS)
                continue;  
            
            //（b） 外部控制信号丢失：  
            // 1、检测到脱扣到位信号， 
            //    自动：电机不做任何动作
            //    手动：电机不做任何动作          
            if(OutOffHookCheck == OUT_OF_HOOK)
            {
                //delay_ms(50);
                //if(OutOffHookCheck != OUT_OF_HOOK)
                //    continue;
                
                if(MotorRunningCtrl_Running != MONTOR_STOP_RUNNING)
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_STOP_RUNNING);//电机停转
            }            
            else //只要不在脱扣到位状态就延时2秒后正转电机到脱扣到位状态
            { 
                //delay_ms(50);
                //if(OutOffHookCheck == OUT_OF_HOOK)
                //    continue;
                
                // 2、检测到合闸到位信号， 
                //    自动：延时2秒，电机（正转）到（脱扣到位）状态就（停止运转）
                //    手动：延时2秒，电机（正转）到（脱扣到位）状态就（停止运转）
                // 3、脱扣到位信号和合闸到位信号都未检测到，
                //    自动：延时2秒，电机（正转）到（脱扣到位）状态就（停止运转）
                //    手动：延时2秒，电机（正转）到（脱扣到位）状态就（停止运转）                
                
                delay_timer(TIMEOUT_VAL_1S5);//等1.5秒     
                if(MotorRunningCtrl_Running != MONTOR_RIGHT_RUNNING)
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);//正传
    
                //等待电机转动到位，超过5秒就认为是超时
                wait_out_off_hook(TIMEOUT_VAL_5S);
    
                if(MotorRunningCtrl_Running != MONTOR_STOP_RUNNING)
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_STOP_RUNNING);//电机停转                
            }            
        }
        else if(ExAutoCtrlSignal == EX_CTRL_SIGNAL_OK)
        {
            //等50毫秒后再次确认信号状态
            //delay_ms(50);       
            //if(ExAutoCtrlSignal != EX_CTRL_SIGNAL_OK)
            //    continue;
            
            //-------------------------------------------------------------------------------------
            //（a） 外部控制信号存在： 
            // 2、检测到合闸到位信号， 
            //    自动：电机不做任何动作
            //    手动：电机不做任何动作
            if(SwitchOnStatus == SWITCH_ON_OK) // 处于合闸到位状态
            {
                //delay_ms(50);
                //if(SwitchOnStatus != SWITCH_ON_OK)
                //    continue;
                
                if(MotorRunningCtrl_Running != MONTOR_STOP_RUNNING)//电机停转 
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            } 
            else //没有检测到合闸到位信号  
            {         
                //delay_ms(50);
                //if(SwitchOnStatus == SWITCH_ON_OK)
                //    continue;
                
                // 1、检测到脱扣到位信号， 
                //    自动：电机（正转）到（合闸到位）状态就（停止）
                //    手动：电机（反转）到（合闸到位）状态就（停止）
                // 3、脱扣到位信号和合闸到位信号都未检测到，
                //    自动：电机（正转）到（合闸到位）状态就（停止）
                //    手动：电机（反转）到（合闸到位）状态就（停止）
                  
                if(SystemWorkMode == AUTO_CTRL_MODE){
                    if(MotorRunningCtrl_Running != MONTOR_RIGHT_RUNNING)//电机正转
                        MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);
                } else {
                    if(MotorRunningCtrl_Running != MONTOR_LEFT_RUNNING)//电机反转
                        MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_LEFT_RUNNING);
                }
              
                //等待电机转动到位，超过5秒就认为是超时
                wait_switch_on(TIMEOUT_VAL_5S);
                
                if(MotorRunningCtrl_Running != MONTOR_STOP_RUNNING)//电机停转  
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            }
        }
        else //异常情况
            Reboot_System(); 
        
        print_signal_status_info();

        /*******************************************************
         * 执行完了让系统进入掉电模式，此时CPU寄存器值保持不变，
         * 收到外部中断INT0/INT1信号唤醒
         * 唤醒后从进入掉电模式的位置继续执行下一条指令
        ********************************************************/
        System_PowerDown();
    }	
}

/********************* INT0中断函数 *************************/
void Ext_INT0 (void) interrupt INT0_VECTOR		//进中断时已经清除标志
{
    _nop_();
    Reboot_System();
    _nop_();
}

/********************* INT1中断函数 *************************/
void Ext_INT1 (void) interrupt INT1_VECTOR		//进中断时已经清除标志
{
	 _nop_();
   Reboot_System();
   _nop_();
}

/********************* Timer0中断函数************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	// process watch dog signal
	WDT_CONTR &= 0x7F;
    WDT_CONTR |= 0x10;

	//timer out counter
	if(TimeoutCount <= TIMEOUT_VAL_MAX)
		TimeoutCount++;	    
}

