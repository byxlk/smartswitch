
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


/*************	����˵��	**************

˫����ȫ˫���жϷ�ʽ�շ�ͨѶ����

ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

******************************************/

/*************	���س�������	**************/

/*************	���ر�������	**************/
//����������
static unsigned char MotorRunningCtrl_Running = MONTOR_STOP_RUNNING;//������� -- ��ת

//��ʱ�������
unsigned char TimeoutCount = STOP_TIMERCOUNT;
unsigned char sysClock = STOP_TIMERCOUNT;

//ϵͳ����ģʽ
//bit lastSystemWorkMode = AUTO_CTRL_MODE;
bit firstSystemBoot = TRUE;


/*************	���غ�������	**************/

/*************  �ⲿ�����ͱ������� *****************/


/*************  ����1��ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 38400ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2
}

/******************** IO���ú��� **************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		            //�ṹ����

	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	   //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //��ʼ��
	
	GPIO_InitStructure.Pin  = GPIO_Pin_4 ;	   //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //��ʼ��
	
	GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_7;	//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	
}

/************************ ��ʱ������ ****************************/
void	Timer_Config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (12 * 20));	//��ֵ, ����Ϊ20HZ(50ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
}

void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//�ṹ����

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//�ж�ģʽ,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//�ж����ȼ�,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE?DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//��ʼ��INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
	
	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//�ж�ģʽ,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//�ж����ȼ�,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE?DISABLE
	Ext_Inilize(EXT_INT1,&EXTI_InitStructure);				//��ʼ��INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}

static void delay_timer(unsigned char iTime)
{
    TimeoutCount = TIMECOUNTER_START;//��ʼ��ʱ
    while(TimeoutCount <= iTime) ;//��1��
    TimeoutCount = STOP_TIMERCOUNT;//ֹͣ��ʱ  
}

static void wait_switch_on(unsigned char time_out)
{
    //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
    TimeoutCount = TIMECOUNTER_START;//��ʼ��ʱ
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
    TimeoutCount = STOP_TIMERCOUNT;//ֹͣ��ʱ
}

static void wait_out_off_hook(unsigned char time_out)
{
    //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
    TimeoutCount = TIMECOUNTER_START;//��ʼ��ʱ
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
    TimeoutCount = STOP_TIMERCOUNT;//ֹͣ��ʱ
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
	  //Ӳ����ʼ������
		EA = 0;
    GPIO_Config();//GPIO init
	  setSystemSleepFlag(FALSE);
	
    EXTI_config();
    UART_config(); //UART init
    Timer_Config();//Timer init
		
		//�����ж�
    EA = 1;

		//ϵͳ�ϵ�������ʱ4������
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
        //����ⲿ�����ź� ---- ��ʧ
        if(ExAutoCtrlSignal == EX_CTRL_SIGNAL_LOSS)
        {
            //��50������ٴ�ȷ���ź�״̬
            //delay_ms(50);
            delay_timer(TIMEOUT_VAL_1S) ;
            if(ExAutoCtrlSignal != EX_CTRL_SIGNAL_LOSS)
                continue;  
            
            //��b�� �ⲿ�����źŶ�ʧ��  
            // 1����⵽�ѿ۵�λ�źţ� 
            //    �Զ�����������κζ���
            //    �ֶ�����������κζ���          
            if(OutOffHookCheck == OUT_OF_HOOK)
            {
                //delay_ms(50);
                //if(OutOffHookCheck != OUT_OF_HOOK)
                //    continue;
                
                if(MotorRunningCtrl_Running != MONTOR_STOP_RUNNING)
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_STOP_RUNNING);//���ͣת
            }            
            else //ֻҪ�����ѿ۵�λ״̬����ʱ2�����ת������ѿ۵�λ״̬
            { 
                //delay_ms(50);
                //if(OutOffHookCheck == OUT_OF_HOOK)
                //    continue;
                
                // 2����⵽��բ��λ�źţ� 
                //    �Զ�����ʱ2�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                //    �ֶ�����ʱ2�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                // 3���ѿ۵�λ�źźͺ�բ��λ�źŶ�δ��⵽��
                //    �Զ�����ʱ2�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                //    �ֶ�����ʱ2�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��                
                
                delay_timer(TIMEOUT_VAL_1S5);//��1.5��     
                if(MotorRunningCtrl_Running != MONTOR_RIGHT_RUNNING)
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);//����
    
                //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
                wait_out_off_hook(TIMEOUT_VAL_5S);
    
                if(MotorRunningCtrl_Running != MONTOR_STOP_RUNNING)
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_STOP_RUNNING);//���ͣת                
            }            
        }
        else if(ExAutoCtrlSignal == EX_CTRL_SIGNAL_OK)
        {
            //��50������ٴ�ȷ���ź�״̬
            //delay_ms(50);       
            //if(ExAutoCtrlSignal != EX_CTRL_SIGNAL_OK)
            //    continue;
            
            //-------------------------------------------------------------------------------------
            //��a�� �ⲿ�����źŴ��ڣ� 
            // 2����⵽��բ��λ�źţ� 
            //    �Զ�����������κζ���
            //    �ֶ�����������κζ���
            if(SwitchOnStatus == SWITCH_ON_OK) // ���ں�բ��λ״̬
            {
                //delay_ms(50);
                //if(SwitchOnStatus != SWITCH_ON_OK)
                //    continue;
                
                if(MotorRunningCtrl_Running != MONTOR_STOP_RUNNING)//���ͣת 
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            } 
            else //û�м�⵽��բ��λ�ź�  
            {         
                //delay_ms(50);
                //if(SwitchOnStatus == SWITCH_ON_OK)
                //    continue;
                
                // 1����⵽�ѿ۵�λ�źţ� 
                //    �Զ����������ת��������բ��λ��״̬�ͣ�ֹͣ��
                //    �ֶ����������ת��������բ��λ��״̬�ͣ�ֹͣ��
                // 3���ѿ۵�λ�źźͺ�բ��λ�źŶ�δ��⵽��
                //    �Զ����������ת��������բ��λ��״̬�ͣ�ֹͣ��
                //    �ֶ����������ת��������բ��λ��״̬�ͣ�ֹͣ��
                  
                if(SystemWorkMode == AUTO_CTRL_MODE){
                    if(MotorRunningCtrl_Running != MONTOR_RIGHT_RUNNING)//�����ת
                        MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);
                } else {
                    if(MotorRunningCtrl_Running != MONTOR_LEFT_RUNNING)//�����ת
                        MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_LEFT_RUNNING);
                }
              
                //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
                wait_switch_on(TIMEOUT_VAL_5S);
                
                if(MotorRunningCtrl_Running != MONTOR_STOP_RUNNING)//���ͣת  
                    MotorRunningCtrl_Running = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            }
        }
        else //�쳣���
            Reboot_System(); 
        
        print_signal_status_info();

        /*******************************************************
         * ִ��������ϵͳ�������ģʽ����ʱCPU�Ĵ���ֵ���ֲ��䣬
         * �յ��ⲿ�ж�INT0/INT1�źŻ���
         * ���Ѻ�ӽ������ģʽ��λ�ü���ִ����һ��ָ��
        ********************************************************/
        System_PowerDown();
    }	
}

/********************* INT0�жϺ��� *************************/
void Ext_INT0 (void) interrupt INT0_VECTOR		//���ж�ʱ�Ѿ������־
{
    _nop_();
    Reboot_System();
    _nop_();
}

/********************* INT1�жϺ��� *************************/
void Ext_INT1 (void) interrupt INT1_VECTOR		//���ж�ʱ�Ѿ������־
{
	 _nop_();
   Reboot_System();
   _nop_();
}

/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	// process watch dog signal
	WDT_CONTR &= 0x7F;
    WDT_CONTR |= 0x10;

	//timer out counter
	if(TimeoutCount <= TIMEOUT_VAL_MAX)
		TimeoutCount++;	    
}

