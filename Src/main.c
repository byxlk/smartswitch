
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


/*************	���س�������	**************/

/*************	���ر�������	**************/
unsigned char VoltExceptionsPlus = 0xFF;    //��ѹ�쳣����
unsigned char VoltOverLedIndicate = 0xFF;   //��ѹ��ѹ
unsigned char VoltUnderLedIndicate = 0xFF; //��ѹǷѹ

unsigned char TimeoutCount = 0xFF;
unsigned char VoltCurrentStatus = LOSS_VOLTAGE;
unsigned char MotorCurrentStatus = MONTOR_STOP_RUNNING;
/*************	���غ�������	**************/

/*************  �ⲿ�����ͱ������� *****************/


/*************  ����1��ʼ������ *****************/
void UART_config(void)
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

/************************ ��ʱ������ ****************************/
void Timer_Config(void) //20ms@24.000MHz
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (12 * 50));	//��ֵ, ����Ϊ50HZ(20ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
}


/******************** IO���ú��� **************************/
//#define	GPIO_PullUp		0	//����׼˫���
//#define	GPIO_HighZ		1	//��������
//#define	GPIO_OUT_OD		2	//��©���
//#define	GPIO_OUT_PP		3	//�������
void GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		            //�ṹ����

    // 220V��ѹ��ѹ��⣬�ֶ����Զ�ģʽѡ������
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;	   //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //��ʼ��
    
    //��ΪAD����ǰ��Ҫ����IOΪ��������ģʽ
	GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //��ʼ��

    // �쳣ָʾ��
    GPIO_InitStructure.Pin  =  GPIO_Pin_2;	                //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //��ʼ��
    
    //�ѿ����բ״̬�������
	GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;	   //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	           //��ʼ��

    //�����������
	GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_7;	    //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_OD;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
    
    //RS485ʹ������
	GPIO_InitStructure.Pin  = GPIO_Pin_2;           	    //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
    
    //��ѹ�쳣ʱ���100ms�����ź�����
	GPIO_InitStructure.Pin  = GPIO_Pin_3;           	    //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);

    //��ʼ������GPIO
    setMontorRunningStatus(MONTOR_STOP_RUNNING);            //���ͣת
    RS485_Recv_Send_Enable = 0;                             //RS485ʹ��Ĭ��Ϊ����ģʽ
    VoltStatusLamp = 1;                                     //״ָ̬ʾ��Ĭ�ϲ���
    VoltStatusPlus = 1;                                     //�����ź�Ĭ�ϸߵ�ƽ
}

void ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����

	ADC_InitStructure.ADC_Px        = ADC_P13;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_540T;			//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H2L8;		//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��

	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE
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


/**********************************************/
void main(void)
{
    //unsigned char i = 0;
    unsigned char iVbCount = 0;
    unsigned short Max_Volt = 0;

	//Ӳ����ʼ������
	EA = 0;
    Timer_Config();//Timer init
    UART_config(); //UART init
    GPIO_Config();//GPIO init
    ADC_config(); //ADC init

	//�����ж�
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
 
        //��⵱ǰ�ĵ�ѹ�Ƿ���ֹ�ѹ����Ƿѹ
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
        // ��ǰ��ѹ״̬   ---   �쳣��������Ƿѹ����ѹ��ʧѹ�
        if(VoltCurrentStatus != NORMAL_VOLTAGE)
        {
            //����ָʾ�ƺ���������ź�
            VoltExceptionsPlus = 0;     //�����ѹ�쳣�������ź�
            if(VoltCurrentStatus == UNDER_VOLTAGE)    VoltUnderLedIndicate = 0;
            else if(VoltCurrentStatus == OVER_VOLTAGE) VoltOverLedIndicate = 0;

            //��b�� �ⲿ�����źŶ�ʧ��
            // 1����⵽�ѿ۵�λ�źţ�
            //    �Զ�����������κζ���
            //    �ֶ�����������κζ���
            if(OutOffHookCheck == OUT_OF_HOOK)
            {
                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);//���ͣת
            }
            else //ֻҪ�����ѿ۵�λ״̬����ʱ2�����ת������ѿ۵�λ״̬
            {
                // 2����⵽��բ��λ�źţ�
                //    �Զ�����ʱ2�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                //    �ֶ�����ʱ2�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                // 3���ѿ۵�λ�źźͺ�բ��λ�źŶ�δ��⵽��
                //    �Զ�����ʱ50ms�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                //    �ֶ�����ʱ50ms�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                if(VoltCurrentStatus != LOSS_VOLTAGE) delay_ms(50);
                if(MotorCurrentStatus != MONTOR_RIGHT_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);//����

                //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
                wait_out_off_hook(TIMEOUT_VAL_5S);

                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);//���ͣת
            }
            //delay_timer(TIMEOUT_VAL_MAX);
            //delay_timer(TIMEOUT_VAL_MAX);
        }
        else //��ǰ��ѹ״̬   ---   ����
        {
            //-------------------------------------------------------------------------------------
            //��a�� �ⲿ�����źŴ��ڣ�
            // 2����⵽��բ��λ�źţ�
            //    �Զ�����������κζ���
            //    �ֶ�����������κζ���
            if(SwitchOnStatus == SWITCH_ON_OK) // ���ں�բ��λ״̬
            {
                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)//���ͣת
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            }
            else //û�м�⵽��բ��λ�ź�
            {
                // 1����⵽�ѿ۵�λ�źţ�
                //    �Զ����������ת��������բ��λ��״̬�ͣ�ֹͣ��
                //    �ֶ����������ת��������բ��λ��״̬�ͣ�ֹͣ��
                // 3���ѿ۵�λ�źźͺ�բ��λ�źŶ�δ��⵽��
                //    �Զ����������ת��������բ��λ��״̬�ͣ�ֹͣ��
                //    �ֶ����������ת��������բ��λ��״̬�ͣ�ֹͣ��

                if(SystemWorkMode == AUTO_CTRL_MODE){
                    if(MotorCurrentStatus != MONTOR_RIGHT_RUNNING)//�����ת
                        MotorCurrentStatus = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);
                } else {
                    if(MotorCurrentStatus != MONTOR_LEFT_RUNNING)//�����ת
                        MotorCurrentStatus = setMontorRunningStatus(MONTOR_LEFT_RUNNING);
                }

                //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
                wait_switch_on(TIMEOUT_VAL_5S);
                delay_timer(TIMEOUT_DELAY_VAL);
                if(MotorCurrentStatus != MONTOR_STOP_RUNNING)//���ͣת
                    MotorCurrentStatus = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            }
        }
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }
}

/********************* Timer0�жϺ���************************/
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
        VoltStatusPlus = 0;//����͵�ƽ
        if(VoltExceptionsPlus++ > 5 ) //20 x 5 = 100ms
        {
            VoltExceptionsPlus = 0xFF;
            VoltStatusPlus = 1;
        }
    }
    
    //Over Volt indicate
    if(VoltOverLedIndicate != 0xFF)
    {
        VoltStatusLamp = 0;//����͵�ƽ
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

