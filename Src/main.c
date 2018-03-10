
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
#include  "ADC.h"
#include  "dlt645.h"
#include  "rs485.h"


/*************	本地常量声明	**************/

/*************	本地变量声明	**************/
unsigned char VoltExceptionsPlus = 0xFF;    //电压异常脉冲
unsigned char VoltOverLedIndicate = 0xFF;   //电压过压
unsigned char VoltUnderLedIndicate = 0xFF; //电压欠压

unsigned short TimeoutCount = STOP_TIMERCOUNT;
unsigned char VoltCurrentStatus = LOSS_VOLTAGE;
unsigned char MotorCurrentStatus = MONTOR_STOP_RUNNING;
/*************	本地函数声明	**************/

/*************  外部函数和变量声明 *****************/


/*************  串口1初始化函数 *****************/
void UART_config(void)
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

/************************ 定时器配置 ****************************/
void Timer_Config(void) //20ms@24.000MHz
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (12 * 50));	//初值, 节拍为50HZ(20ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
}


/******************** IO配置函数 **************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		            //结构定义

    // 220V电压掉压检测，手动和自动模式选择引脚
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化
    
    //作为AD功能前先要设置IO为高阻输入模式
	GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化

    // 异常指示灯
    GPIO_InitStructure.Pin  =  GPIO_Pin_2;	                //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化
    
    //脱口与合闸状态检测引脚
	GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	           //初始化

    //电机控制引脚
	GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_7;	    //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		            //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
    
    //RS485使能引脚
	GPIO_InitStructure.Pin  = GPIO_Pin_2;           	    //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		            //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
    
    //电压异常时输出100ms脉冲信号引脚
	GPIO_InitStructure.Pin  = GPIO_Pin_3;           	    //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		            //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);

    //初始化部分GPIO
    MotorRunningCtrl_R = 1;
    MotorRunningCtrl_L = 1;            //电机停转
    RS485_Recv_Send_Enable = 0;                             //RS485使能默认为接收模式
    VoltStatusLamp = 1;                                     //状态指示灯默认不亮
    VoltStatusPlus = 1;                                     //脉冲信号默认高电平
}

void ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//结构定义

	ADC_InitStructure.ADC_Px        = ADC_P13;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_540T;			//ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H2L8;		//ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//初始化

	ADC_PowerControl(ENABLE);							//单独的ADC电源操作函数, ENABLE或DISABLE
}


static void delay_timer(unsigned short iTime)
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


/**********************************************/
void main(void)
{
    //unsigned char i = 0;
    unsigned char iVbCount = 0;
    unsigned short Max_Volt = 0;

	//硬件初始化操作
	EA = 0;
    Timer_Config();//Timer init
    UART_config(); //UART init
    GPIO_Config();//GPIO init
    ADC_config(); //ADC init

	//开总中断
    EA = 1;

	//watch dog init
	init_Watch_Dog();

	//=======================> Start Main Process <==========================//
	while (1)
	{
        //if(iVbCount > 9)
        {
            //Max_Volt = Max_Volt / iVbCount;
            iVbCount = 0;
        //} else {
            Max_Volt = getACVppVolt();
            //iVbCount++;
            //continue;
        }

#if DEBUG 
        LOGD("Max Volt: ");
        debug(Max_Volt / 1000 % 10);
        debug(Max_Volt / 100 % 10);
        debug(Max_Volt / 10 % 10);
        debug(Max_Volt % 10);
        LOGD("  ---  ");
#endif

        //检测当前的电压是否出现过压或者欠压
        if(Max_Volt >= OVER_VOLT_DIGIT_VAL){
            VoltCurrentStatus = OVER_VOLTAGE;
            LOGD("OVER_VOLTAGE\r\n");
        } else if(Max_Volt <= UNDER_VOLT_DIGIT_VAL) {
            if(Max_Volt == 0) {
                VoltCurrentStatus = LOSS_VOLTAGE;
                LOGD("LOSS_VOLTAGE\r\n");
            } else {
                VoltCurrentStatus = UNDER_VOLTAGE;
                LOGD("UNDER_VOLTAGE\r\n");
            }
        } else {
            VoltCurrentStatus = NORMAL_VOLTAGE;
            LOGD("NORMAL_VOLTAGE\r\n");
        }
        Max_Volt = 0;

        //-------------------------------------------------------------------------------------
        // 当前电压状态   ---   异常（包括：欠压、过压、失压�
        if(VoltCurrentStatus != NORMAL_VOLTAGE)
        {
            //设置指示灯和脉冲输出信号
            if(VoltExceptionsPlus == 0xFF) VoltExceptionsPlus = 0;     //输出电压异常的脉冲信号
            if(VoltCurrentStatus == UNDER_VOLTAGE && VoltUnderLedIndicate == 0xFF)    VoltUnderLedIndicate = 0;
            else if(VoltCurrentStatus == OVER_VOLTAGE && VoltOverLedIndicate == 0xFF) VoltOverLedIndicate = 0;

            //（b） 外部控制信号丢失：
            // 1、检测到脱扣到位信号，
            //    自动：电机不做任何动作
            //    手动：电机不做任何动作
            if(OutOffHookCheck == OUT_OF_HOOK)
            {
                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);//电机停转
            }
            else //只要不在脱扣到位状态就延时2秒后正转电机到脱扣到位状态
            {
                // 2、检测到合闸到位信号，
                //    自动：电机（正转）到（脱扣到位）状态就（停止运转）
                //    手动：电机（正转）到（脱扣到位）状态就（停止运转）
                // 3、脱扣到位信号和合闸到位信号都未检测到，
                //    自动：电机（正转）到（脱扣到位）状态就（停止运转）
                //    手动：电机（正转）到（脱扣到位）状态就（停止运转）
                //if(VoltCurrentStatus != LOSS_VOLTAGE) delay_ms(50);
                if(MotorCurrentStatus != MONTOR_RIGHT_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);//正传

                //等待电机转动到位，超过5秒就认为是超时
                wait_out_off_hook(TIMEOUT_VAL_5S);

                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);//电机停转
            }
        }
        else //当前电压状态   ---   正常
        {
            //-------------------------------------------------------------------------------------
            //（a） 外部控制信号存在：
            // 2、检测到合闸到位信号，
            //    自动：电机不做任何动作
            //    手动：电机不做任何动作
            if(SwitchOnStatus == SWITCH_ON_OK) // 处于合闸到位状态
            {
                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)//电机停转
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            }
            else //没有检测到合闸到位信号
            {
                // 1、检测到脱扣到位信号，
                //    自动：电机（正转）到（合闸到位）状态就（停止）
                //    手动：电机（反转）到（合闸到位）状态就（停止）
                // 3、脱扣到位信号和合闸到位信号都未检测到，
                //    自动：电机（正转）到（合闸到位）状态就（停止）
                //    手动：电机（反转）到（合闸到位）状态就（停止）

                if(SystemWorkMode == AUTO_CTRL_MODE){
                    if(MotorCurrentStatus != MONTOR_RIGHT_RUNNING)//电机正转
                        MotorCurrentStatus = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);
                } else {
                    if(MotorCurrentStatus != MONTOR_LEFT_RUNNING)//电机反转
                        MotorCurrentStatus = setMontorRunningStatus(MONTOR_LEFT_RUNNING);
                }

                //等待电机转动到位，超过5秒就认为是超时
                wait_switch_on(TIMEOUT_VAL_5S);
                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)//电机停转
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            }
        }
    }
}

/********************* Timer0中断函数************************/
#define TIMER_VALUE (65536 - (MAIN_Fosc / (12 * 50)))
void timer0_int (void) interrupt TIMER0_VECTOR //20ms@24.000MHz
{
	// process watch dog signal
	WDT_CONTR &= 0x7F;
    WDT_CONTR |= 0x10;

	//timer out counter
	if(TimeoutCount <= TIMEOUT_VAL_MAX)
		TimeoutCount++;

    // Volt exception plus signal
    if(VoltExceptionsPlus != 0xFF)
    {
        VoltExceptionsPlus++;
        if(VoltExceptionsPlus < 5 && VoltExceptionsPlus >= 0 ) VoltStatusPlus = 0;//20 x 5 = 100ms
        else if(VoltExceptionsPlus < 10 && VoltExceptionsPlus >= 5 ) VoltStatusPlus = 1;
        else {VoltExceptionsPlus = 0xFF; VoltStatusPlus = 1;}
    }
    
    //Over Volt indicate
    if(VoltOverLedIndicate != 0xFF)
    {
        VoltOverLedIndicate++;
        if(VoltOverLedIndicate < 25 && VoltOverLedIndicate >= 0) VoltStatusLamp = 0;//20 x 25 = 500ms
        else if(VoltOverLedIndicate < 50 && VoltOverLedIndicate >= 25) VoltStatusLamp = 1;
        else {VoltStatusLamp = 1; VoltOverLedIndicate = 0xFF;}
    }
    
    //Under Volt indication
    if(VoltUnderLedIndicate != 0xFF)
    {
        VoltUnderLedIndicate++;
        if(VoltUnderLedIndicate < 15 && VoltUnderLedIndicate >= 0) VoltStatusLamp = 0; //300ms
        else if(VoltUnderLedIndicate < 20 && VoltUnderLedIndicate >= 15) VoltStatusLamp = 1; //100ms
        else if(VoltUnderLedIndicate < 35 && VoltUnderLedIndicate >= 20) VoltStatusLamp = 0; //300ms
        else if(VoltUnderLedIndicate < 135 && VoltUnderLedIndicate >= 35) VoltStatusLamp = 1; //2000ms
        else {VoltStatusLamp = 1; VoltUnderLedIndicate = 0xFF;}
    }

    /* 减小定时器误差 */
	//TL0 = TIMER_VALUE % 256 - TL0;
	//TH0 = TIMER_VALUE / 256 - TH0;
}

