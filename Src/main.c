
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


/*************	±¾µØ³£Á¿ÉùÃ÷	**************/

/*************	±¾µØ±äÁ¿ÉùÃ÷	**************/
unsigned char VoltExceptionsCount = 0xFF;
unsigned char VoltExceptionsStatus = 0x0; //µçÑ¹Õý³£

unsigned char TimeoutCount = 0xFF;
unsigned char VoltCurrentStatus = LOSS_VOLTAGE;
unsigned char MotorCurrentStatus = MONTOR_STOP_RUNNING;
/*************	±¾µØº¯ÊýÉùÃ÷	**************/

/*************  Íâ²¿º¯ÊýºÍ±äÁ¿ÉùÃ÷ *****************/


/*************  ´®¿Ú1³õÊ¼»¯º¯Êý *****************/
void UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//½á¹¹¶¨Òå
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//Ä£Ê½,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//Ê¹ÓÃ²¨ÌØÂÊ,   BRT_Timer1, BRT_Timer2 (×¢Òâ: ´®¿Ú2¹Ì¶¨Ê¹ÓÃBRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 38400ul;			//²¨ÌØÂÊ, Ò»°ã 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//½ÓÊÕÔÊÐí,   ENABLE»òDISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//²¨ÌØÂÊ¼Ó±¶, ENABLE»òDISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//ÖÐ¶ÏÔÊÐí,   ENABLE»òDISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//ÖÐ¶ÏÓÅÏÈ¼¶, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//ÇÐ»»¶Ë¿Ú,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(±ØÐëÊ¹ÓÃÄÚ²¿Ê±ÖÓ)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//ÄÚ²¿¶ÌÂ·RXDÓëTXD, ×öÖÐ¼Ì, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//³õÊ¼»¯´®¿Ú1 USART1,USART2
}

/******************** IOÅäÖÃº¯Êý **************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		            //½á¹¹¶¨Òå

	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;	   //Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //³õÊ¼»¯

	GPIO_InitStructure.Pin  = GPIO_Pin_4 ;	   //Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //³õÊ¼»¯

	GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_7;	//Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
}

/************************ ¶¨Ê±Æ÷ÅäÖÃ ****************************/
void Timer_Config(void) //20ms@24.000MHz
{
	TIM_InitTypeDef		TIM_InitStructure;					//½á¹¹¶¨Òå
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//Ö¸¶¨¹¤×÷Ä£Ê½,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//Ö¸¶¨ÖÐ¶ÏÓÅÏÈ¼¶, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//ÖÐ¶ÏÊÇ·ñÔÊÐí,   ENABLE»òDISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//Ö¸¶¨Ê±ÖÓÔ´,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//ÊÇ·ñÊä³ö¸ßËÙÂö³å, ENABLE»òDISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (12 * 50));	//³õÖµ, ½ÚÅÄÎª50HZ(20ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//ÊÇ·ñ³õÊ¼»¯ºóÆô¶¯¶¨Ê±Æ÷, ENABLE»òDISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//³õÊ¼»¯Timer0	  Timer0,Timer1,Timer2
}

void ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//½á¹¹¶¨Òå

	ADC_InitStructure.ADC_Px        = ADC_P13;	        //ÉèÖÃÒª×öADCµÄIO,	ADC_P10 ~ ADC_P17(»ò²Ù×÷),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_540T;			//ADCËÙ¶È			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC¹¦ÂÊÔÊÐí/¹Ø±Õ	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H2L8;		//ADC½á¹ûµ÷Õû,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//ÓÅÏÈ¼¶ÉèÖÃ	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//ÖÐ¶ÏÔÊÐí		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//³õÊ¼»¯

	ADC_PowerControl(ENABLE);							//µ¥¶ÀµÄADCµçÔ´²Ù×÷º¯Êý, ENABLE»òDISABLE
}

static void delay_timer(unsigned char iTime)
{
    TimeoutCount = TIMECOUNTER_START;//¿ªÊ¼¼ÆÊ±
    while(TimeoutCount <= iTime) ;//µÈ1Ãë
    TimeoutCount = STOP_TIMERCOUNT;//Í£Ö¹¼ÆÊ±
}

static void wait_switch_on(unsigned char time_out)
{
    //µÈ´ýµç»ú×ª¶¯µ½Î»£¬³¬¹ý5Ãë¾ÍÈÏÎªÊÇ³¬Ê±
    TimeoutCount = TIMECOUNTER_START;//¿ªÊ¼¼ÆÊ±
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
    TimeoutCount = STOP_TIMERCOUNT;//Í£Ö¹¼ÆÊ±
}

static void wait_out_off_hook(unsigned char time_out)
{
    //µÈ´ýµç»ú×ª¶¯µ½Î»£¬³¬¹ý5Ãë¾ÍÈÏÎªÊÇ³¬Ê±
    TimeoutCount = TIMECOUNTER_START;//¿ªÊ¼¼ÆÊ±
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
    TimeoutCount = STOP_TIMERCOUNT;//Í£Ö¹¼ÆÊ±
}


/**********************************************/
void main(void)
{
    unsigned char i = 0;
    unsigned short Max_Volt = 0;
    unsigned short voltbuf[10];

	//Ó²¼þ³õÊ¼»¯²Ù×÷
	EA = 0;
    GPIO_Config();//GPIO init

    UART_config(); //UART init
    Timer_Config();//Timer init
    ADC_config(); //ADC init

	//¿ª×ÜÖÐ¶Ï
    EA = 1;

	//watch dog init
	init_Watch_Dog();

	//=======================> Start Main Process <==========================//
	while (1)
	{
        VoltCapturePortC = ~VoltCapturePortC;
        Max_Volt = getACVppVolt();
        LOGD("Max Volt: ");
        debug(Max_Volt / 1000 % 10);
        debug(Max_Volt / 100 % 10);
        debug(Max_Volt / 10 % 10);
        debug(Max_Volt % 10);
        LOGD("  ---  ");
 
        //¼ì²âµ±Ç°µÄµçÑ¹ÊÇ·ñ³öÏÖ¹ýÑ¹»òÕßÇ·Ñ¹
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
        // µ±Ç°µçÑ¹×´Ì¬   ---   Òì³££¨°üÀ¨£ºÇ·Ñ¹¡¢¹ýÑ¹¡¢Ê§Ñ¹£
        if(VoltCurrentStatus != NORMAL_VOLTAGE)
        {            
            //£¨b£© Íâ²¿¿ØÖÆÐÅºÅ¶ªÊ§£º
            // 1¡¢¼ì²âµ½ÍÑ¿Ûµ½Î»ÐÅºÅ£¬
            //    ×Ô¶¯£ºµç»ú²»×öÈÎºÎ¶¯×÷
            //    ÊÖ¶¯£ºµç»ú²»×öÈÎºÎ¶¯×÷
            if(OutOffHookCheck == OUT_OF_HOOK)
            {
                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);//µç»úÍ£×ª
            }
            else //Ö»Òª²»ÔÚÍÑ¿Ûµ½Î»×´Ì¬¾ÍÑÓÊ±2ÃëºóÕý×ªµç»úµ½ÍÑ¿Ûµ½Î»×´Ì¬
            {
                // 2¡¢¼ì²âµ½ºÏÕ¢µ½Î»ÐÅºÅ£¬
                //    ×Ô¶¯£ºÑÓÊ±2Ãë£¬µç»ú£¨Õý×ª£©µ½£¨ÍÑ¿Ûµ½Î»£©×´Ì¬¾Í£¨Í£Ö¹ÔË×ª£©
                //    ÊÖ¶¯£ºÑÓÊ±2Ãë£¬µç»ú£¨Õý×ª£©µ½£¨ÍÑ¿Ûµ½Î»£©×´Ì¬¾Í£¨Í£Ö¹ÔË×ª£©
                // 3¡¢ÍÑ¿Ûµ½Î»ÐÅºÅºÍºÏÕ¢µ½Î»ÐÅºÅ¶¼Î´¼ì²âµ½£¬
                //    ×Ô¶¯£ºÑÓÊ±50msÃë£¬µç»ú£¨Õý×ª£©µ½£¨ÍÑ¿Ûµ½Î»£©×´Ì¬¾Í£¨Í£Ö¹ÔË×ª£©
                //    ÊÖ¶¯£ºÑÓÊ±50msÃë£¬µç»ú£¨Õý×ª£©µ½£¨ÍÑ¿Ûµ½Î»£©×´Ì¬¾Í£¨Í£Ö¹ÔË×ª£©
                if(VoltCurrentStatus != LOSS_VOLTAGE)
                    delay_ms(50);
                if(MotorCurrentStatus != MONTOR_RIGHT_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);//Õý´«

                //µÈ´ýµç»ú×ª¶¯µ½Î»£¬³¬¹ý5Ãë¾ÍÈÏÎªÊÇ³¬Ê±
                wait_out_off_hook(TIMEOUT_VAL_5S);

                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);//µç»úÍ£×ª
            }
        }
        else //µ±Ç°µçÑ¹×´Ì¬   ---   Õý³£
        {
            VoltCapturePortB = ~VoltCapturePortB;
            //-------------------------------------------------------------------------------------
            //£¨a£© Íâ²¿¿ØÖÆÐÅºÅ´æÔÚ£º
            // 2¡¢¼ì²âµ½ºÏÕ¢µ½Î»ÐÅºÅ£¬
            //    ×Ô¶¯£ºµç»ú²»×öÈÎºÎ¶¯×÷
            //    ÊÖ¶¯£ºµç»ú²»×öÈÎºÎ¶¯×÷
            if(SwitchOnStatus == SWITCH_ON_OK) // ´¦ÓÚºÏÕ¢µ½Î»×´Ì¬
            {
                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)//µç»úÍ£×ª
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            }
            else //Ã»ÓÐ¼ì²âµ½ºÏÕ¢µ½Î»ÐÅºÅ
            {
                // 1¡¢¼ì²âµ½ÍÑ¿Ûµ½Î»ÐÅºÅ£¬
                //    ×Ô¶¯£ºµç»ú£¨Õý×ª£©µ½£¨ºÏÕ¢µ½Î»£©×´Ì¬¾Í£¨Í£Ö¹£©
                //    ÊÖ¶¯£ºµç»ú£¨·´×ª£©µ½£¨ºÏÕ¢µ½Î»£©×´Ì¬¾Í£¨Í£Ö¹£©
                // 3¡¢ÍÑ¿Ûµ½Î»ÐÅºÅºÍºÏÕ¢µ½Î»ÐÅºÅ¶¼Î´¼ì²âµ½£¬
                //    ×Ô¶¯£ºµç»ú£¨Õý×ª£©µ½£¨ºÏÕ¢µ½Î»£©×´Ì¬¾Í£¨Í£Ö¹£©
                //    ÊÖ¶¯£ºµç»ú£¨·´×ª£©µ½£¨ºÏÕ¢µ½Î»£©×´Ì¬¾Í£¨Í£Ö¹£©

                if(SystemWorkMode == AUTO_CTRL_MODE){
                    if(MotorCurrentStatus != MONTOR_RIGHT_RUNNING)//µç»úÕý×ª
                        MotorCurrentStatus = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);
                } else {
                    if(MotorCurrentStatus != MONTOR_LEFT_RUNNING)//µç»ú·´×ª
                        MotorCurrentStatus = setMontorRunningStatus(MONTOR_LEFT_RUNNING);
                }

                //µÈ´ýµç»ú×ª¶¯µ½Î»£¬³¬¹ý5Ãë¾ÍÈÏÎªÊÇ³¬Ê±
                wait_switch_on(TIMEOUT_VAL_5S);
                delay_timer(TIMEOUT_DELAY_VAL);
                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)//µç»úÍ£×ª
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            }
        }
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }
}

/********************* Timer0ÖÐ¶Ïº¯Êý************************/
void timer0_int (void) interrupt TIMER0_VECTOR //20ms@24.000MHz
{
	// process watch dog signal
	WDT_CONTR &= 0x7F;
    WDT_CONTR |= 0x10;

	//timer out counter
	if(TimeoutCount <= TIMEOUT_VAL_MAX)
		TimeoutCount++;

    //status indicator lamp
    if(!VoltExceptionsStatus)
    {
        VoltStatusLamp = 1;
        VoltStatusPlus = 1;
        if(VoltExceptionsCount != 0xFF) VoltExceptionsCount = 0xFF;
    }
    else
    {
        VoltExceptionsCount++;

        if(VoltExceptionsCount > 5) VoltStatusPlus = 1;
        else VoltStatusPlus = 0;

        if(VoltCurrentStatus == UNDER_VOLTAGE)
        {
            if(VoltExceptionsCount < 15 && VoltExceptionsCount > 0) VoltStatusLamp = 0; //300ms
            else if(VoltExceptionsCount < 20 && VoltExceptionsCount >= 15) VoltStatusLamp = 1; //100ms
            else if(VoltExceptionsCount < 35 && VoltExceptionsCount >= 20) VoltStatusLamp = 0; //300ms
            else if(VoltExceptionsCount < 135 && VoltExceptionsCount >= 35) VoltStatusLamp = 1; //2000ms
            else if(VoltExceptionsCount < 150 && VoltExceptionsCount >= 135) VoltStatusLamp = 0; //300ms
            else if(VoltExceptionsCount < 155 && VoltExceptionsCount >= 150) VoltStatusLamp = 1; //100ms
            else if(VoltExceptionsCount < 170 && VoltExceptionsCount >= 155) VoltStatusLamp = 0; //300ms
            else if(VoltExceptionsCount >= 175) VoltStatusLamp = 0; //300ms
        }
        else if(VoltCurrentStatus == OVER_VOLTAGE)
        {
            if(VoltExceptionsCount > 25) VoltStatusLamp = 1;
            else VoltStatusLamp = 0;
        }
    }
}

