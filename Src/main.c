
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
unsigned char VoltExceptionsPlus = 0xFF;    //µçÑ¹Òì³£Âö³å
unsigned char VoltOverLedIndicate = 0xFF;   //µçÑ¹¹ýÑ¹
unsigned char VoltUnderLedIndicate = 0xFF; //µçÑ¹Ç·Ñ¹

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

/************************ ¶¨Ê±Æ÷ÅäÖÃ ****************************/
void Timer_Config(void) //20ms@24.000MHz
{
	TIM_InitTypeDef		TIM_InitStructure;					//½á¹¹¶¨Òå
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//Ö¸¶¨¹¤×÷Ä£Ê½,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//Ö¸¶¨ÖÐ¶ÏÓÅÏÈ¼¶, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//ÖÐ¶ÏÊÇ·ñÔÊÐí,   ENABLE»òDISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//Ö¸¶¨Ê±ÖÓÔ´,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//ÊÇ·ñÊä³ö¸ßËÙÂö³å, ENABLE»òDISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (12 * 50));	//³õÖµ, ½ÚÅÄÎª50HZ(20ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//ÊÇ·ñ³õÊ¼»¯ºóÆô¶¯¶¨Ê±Æ÷, ENABLE»òDISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//³õÊ¼»¯Timer0	  Timer0,Timer1,Timer2
}


/******************** IOÅäÖÃº¯Êý **************************/
//#define	GPIO_PullUp		0	//ÉÏÀ­×¼Ë«Ïò¿Ú
//#define	GPIO_HighZ		1	//¸¡¿ÕÊäÈë
//#define	GPIO_OUT_OD		2	//¿ªÂ©Êä³ö
//#define	GPIO_OUT_PP		3	//ÍÆÍìÊä³ö
void GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		            //½á¹¹¶¨Òå

    // 220VµçÑ¹µôÑ¹¼ì²â£¬ÊÖ¶¯ºÍ×Ô¶¯Ä£Ê½Ñ¡ÔñÒý½Å
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;	   //Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //³õÊ¼»¯
    
    //×÷ÎªAD¹¦ÄÜÇ°ÏÈÒªÉèÖÃIOÎª¸ß×èÊäÈëÄ£Ê½
	GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //³õÊ¼»¯

    // Òì³£Ö¸Ê¾µÆ
    GPIO_InitStructure.Pin  =  GPIO_Pin_2;	                //Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //³õÊ¼»¯
    
    //ÍÑ¿ÚÓëºÏÕ¢×´Ì¬¼ì²âÒý½Å
	GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;	   //Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	           //³õÊ¼»¯

    //µç»ú¿ØÖÆÒý½Å
	GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_7;	    //Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_OUT_OD;		            //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
    
    //RS485Ê¹ÄÜÒý½Å
	GPIO_InitStructure.Pin  = GPIO_Pin_2;           	    //Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_PullUp;		            //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
    
    //µçÑ¹Òì³£Ê±Êä³ö100msÂö³åÐÅºÅÒý½Å
	GPIO_InitStructure.Pin  = GPIO_Pin_3;           	    //Ö¸¶¨Òª³õÊ¼»¯µÄIO, GPIO_Pin_0 ~ GPIO_Pin_7, »ò²Ù×÷
	GPIO_InitStructure.Mode = GPIO_PullUp;		            //Ö¸¶¨IOµÄÊäÈë»òÊä³ö·½Ê½,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);

    //³õÊ¼»¯²¿·ÖGPIO
    setMontorRunningStatus(MONTOR_STOP_RUNNING);            //µç»úÍ£×ª
    RS485_Recv_Send_Enable = 0;                             //RS485Ê¹ÄÜÄ¬ÈÏÎª½ÓÊÕÄ£Ê½
    VoltStatusLamp = 1;                                     //×´Ì¬Ö¸Ê¾µÆÄ¬ÈÏ²»ÁÁ
    VoltStatusPlus = 1;                                     //Âö³åÐÅºÅÄ¬ÈÏ¸ßµçÆ½
}

void ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//½á¹¹¶¨Òå

	ADC_InitStructure.ADC_Px        = ADC_P13;	//ÉèÖÃÒª×öADCµÄIO,	ADC_P10 ~ ADC_P17(»ò²Ù×÷),ADC_P1_All
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
    //unsigned char i = 0;
    unsigned char iVbCount = 0;
    unsigned short Max_Volt = 0;

	//Ó²¼þ³õÊ¼»¯²Ù×÷
	EA = 0;
    Timer_Config();//Timer init
    UART_config(); //UART init
    GPIO_Config();//GPIO init
    ADC_config(); //ADC init

	//¿ª×ÜÖÐ¶Ï
    EA = 1;

	//watch dog init
	init_Watch_Dog();

	//=======================> Start Main Process <==========================//
	while (1)
	{
        RS485_Recv_Send_Enable = ~RS485_Recv_Send_Enable;
        Max_Volt = getACVppVolt();
        //if(iVbCount++ < 10) continue;
        //else {
        //    Max_Volt = Max_Volt / iVbCount;
        //    iVbCount = 0;
        //}
        
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
            //ÉèÖÃÖ¸Ê¾µÆºÍÂö³åÊä³öÐÅºÅ
            VoltExceptionsPlus = 0;     //Êä³öµçÑ¹Òì³£µÄÂö³åÐÅºÅ
            if(VoltCurrentStatus == UNDER_VOLTAGE)    VoltUnderLedIndicate = 0;
            else if(VoltCurrentStatus == OVER_VOLTAGE) VoltOverLedIndicate = 0;

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
                if(VoltCurrentStatus != LOSS_VOLTAGE) delay_ms(50);
                if(MotorCurrentStatus != MONTOR_RIGHT_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);//Õý´«

                //µÈ´ýµç»ú×ª¶¯µ½Î»£¬³¬¹ý5Ãë¾ÍÈÏÎªÊÇ³¬Ê±
                wait_out_off_hook(TIMEOUT_VAL_5S);

                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);//µç»úÍ£×ª
            }
            //delay_timer(TIMEOUT_VAL_MAX);
            //delay_timer(TIMEOUT_VAL_MAX);
        }
        else //µ±Ç°µçÑ¹×´Ì¬   ---   Õý³£
        {
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

    // Volt exception plus signal
    if(VoltExceptionsPlus != 0xFF)
    {
        VoltStatusPlus = 0;//Êä³öµÍµçÆ½
        if(VoltExceptionsPlus++ > 5 ) //20 x 5 = 100ms
        {
            VoltExceptionsPlus = 0xFF;
            VoltStatusPlus = 1;
        }
    }
    
    //Over Volt indicate
    if(VoltOverLedIndicate != 0xFF)
    {
        VoltStatusLamp = 0;//Êä³öµÍµçÆ½
        if(VoltOverLedIndicate++ > 25 ) //20 x 25 = 500ms
        {
            VoltOverLedIndicate = 0xFF;
            VoltStatusLamp = 1;
        }
    }
    
    //Under Volt indication
    if(VoltUnderLedIndicate != 0xFF)
    {
        VoltUnderLedIndicate++;
        if(VoltUnderLedIndicate < 15 && VoltUnderLedIndicate >= 0) VoltStatusLamp = 0; //300ms
        else if(VoltUnderLedIndicate < 20 && VoltUnderLedIndicate >= 15) VoltStatusLamp = 1; //100ms
        else if(VoltUnderLedIndicate < 35 && VoltUnderLedIndicate >= 20) VoltStatusLamp = 0; //300ms
        else if(VoltUnderLedIndicate < 135 && VoltUnderLedIndicate >= 35) VoltStatusLamp = 1; //2000ms
        else if(VoltUnderLedIndicate < 150 && VoltUnderLedIndicate >= 135) VoltStatusLamp = 0; //300ms
        else if(VoltUnderLedIndicate < 155 && VoltUnderLedIndicate >= 150) VoltStatusLamp = 1; //100ms
        else if(VoltUnderLedIndicate < 170 && VoltUnderLedIndicate >= 155) VoltStatusLamp = 0; //300ms
        else if(VoltUnderLedIndicate >= 170) {VoltStatusLamp = 1; VoltUnderLedIndicate = 0xFF;}
    }
}

