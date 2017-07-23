
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
unsigned char VoltExceptionsCount = 0xFF;
unsigned char VoltExceptionsStatus = 0x0; //��ѹ����
SMART_SWITCH_T smartSwitch = {
    0x01,                  //Ĭ���ǵ�һ������
    AUTO_CTRL_MODE,        //Ĭ�����Զ�ģʽ
    MONTOR_STOP_RUNNING,   //������� -- Ĭ�ϲ�ת
    STOP_TIMERCOUNT,       //��ʱ�������
    //{0x68,{0x0},0x68,0x0,0x1,0x0,0xD1,0x16}
};


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
void Timer_Config(void) //20ms@24.000MHz
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (12 * 50));	//��ֵ, ����Ϊ50HZ(20ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
}

void ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����

	ADC_InitStructure.ADC_Px        = ADC_P13;	        //����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
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
    smartSwitch.TimeoutCount = TIMECOUNTER_START;//��ʼ��ʱ
    while(smartSwitch.TimeoutCount <= iTime) ;//��1��
    smartSwitch.TimeoutCount = STOP_TIMERCOUNT;//ֹͣ��ʱ
}

static void wait_switch_on(unsigned char time_out)
{
    //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
    smartSwitch.TimeoutCount = TIMECOUNTER_START;//��ʼ��ʱ
    while(SwitchOnStatus != SWITCH_ON_OK)
    {
        if(smartSwitch.TimeoutCount > time_out)
        {
            LOGD("[TimeOut] Wait Montor <=== timeout 5 Sec.");
            break;
        }
        //if(SwitchOnStatus == SWITCH_ON_OK)
        //    delay_ms(50);
    }
    smartSwitch.TimeoutCount = STOP_TIMERCOUNT;//ֹͣ��ʱ
}

static void wait_out_off_hook(unsigned char time_out)
{
    //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
    smartSwitch.TimeoutCount = TIMECOUNTER_START;//��ʼ��ʱ
    while(OutOffHookCheck != OUT_OF_HOOK)
    {
        if(smartSwitch.TimeoutCount > time_out)
        {
            LOGD("[TimeOut] Wait Montor <=== timeout 5 Sec.");
            break;
        }
        //if(OutOffHookCheck != OUT_OF_HOOK)
        //    delay_ms(50);
    }
    smartSwitch.TimeoutCount = STOP_TIMERCOUNT;//ֹͣ��ʱ
}


/**********************************************/
void main(void)
{
    unsigned short Cur_Volt = 0;
    unsigned short Max_Volt = 0;
    unsigned short Min_Volt = 0;
    unsigned char underVoltCount = 0;
    unsigned char overVoltCount = 0;

	//Ӳ����ʼ������
	EA = 0;
    GPIO_Config();//GPIO init

    UART_config(); //UART init
    Timer_Config();//Timer init
    ADC_config(); //ADC init

	//�����ж�
    EA = 1;

	//watch dog init
	init_Watch_Dog();

	//=======================> Start Main Process <==========================//
	while (1)
	{
        //��⵱ǰ��һ����������ߺ���͵�ѹ
        Cur_Volt = Get_ADC10bitResult(3);
        if(Cur_Volt > Max_Volt) {
            Max_Volt = Cur_Volt;
            Min_Volt = Cur_Volt;
            continue;
        } else {
            if(Cur_Volt < Min_Volt) {
                Min_Volt = Cur_Volt;
                continue;
            }
        }
        
        //��⵱ǰ�ĵ�ѹ�Ƿ���ֹ�ѹ����Ƿѹ
        if(Max_Volt <= UNDER_VOLT_DIGIT_VAL) {
            if((underVoltCount++) > 5){
                if(Max_Volt == Min_Volt && Max_Volt == 0) 
                    smartSwitch.VoltCurrentStatus = LOSS_VOLTAGE;
                else
                    smartSwitch.VoltCurrentStatus = UNDER_VOLTAGE;
                underVoltCount = 0;
            }
        } else if(Max_Volt >= OVER_VOLT_DIGIT_VAL){
            if((overVoltCount++) > 5) {
                smartSwitch.VoltCurrentStatus = OVER_VOLTAGE;
                overVoltCount = 0;
            }
        } else {
            smartSwitch.VoltCurrentStatus = NORMAL_VOLTAGE;
        }
        Max_Volt = Min_Volt;

        //-------------------------------------------------------------------------------------
        // ��ǰ��ѹ״̬   ---   �쳣��������Ƿѹ����ѹ��ʧѹ�
        if(smartSwitch.VoltCurrentStatus != NORMAL_VOLTAGE)
        {
            //��b�� �ⲿ�����źŶ�ʧ��
            // 1����⵽�ѿ۵�λ�źţ�
            //    �Զ�����������κζ���
            //    �ֶ�����������κζ���
            if(OutOffHookCheck == OUT_OF_HOOK)
            {
                if(smartSwitch.MotorCurrentSttaus != MONTOR_STOP_RUNNING)
                    smartSwitch.MotorCurrentSttaus = setMontorRunningStatus(MONTOR_STOP_RUNNING);//���ͣת
            }
            else //ֻҪ�����ѿ۵�λ״̬����ʱ2�����ת������ѿ۵�λ״̬
            {
                // 2����⵽��բ��λ�źţ�
                //    �Զ�����ʱ2�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                //    �ֶ�����ʱ2�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                // 3���ѿ۵�λ�źźͺ�բ��λ�źŶ�δ��⵽��
                //    �Զ�����ʱ50ms�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                //    �ֶ�����ʱ50ms�룬�������ת�������ѿ۵�λ��״̬�ͣ�ֹͣ��ת��
                if(smartSwitch.VoltCurrentStatus != LOSS_VOLTAGE)
                    delay_ms(50);
                if(smartSwitch.MotorCurrentSttaus != MONTOR_RIGHT_RUNNING)
                    smartSwitch.MotorCurrentSttaus = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);//����

                //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
                wait_out_off_hook(TIMEOUT_VAL_5S);

                if(smartSwitch.MotorCurrentSttaus != MONTOR_STOP_RUNNING)
                    smartSwitch.MotorCurrentSttaus = setMontorRunningStatus(MONTOR_STOP_RUNNING);//���ͣת
            }
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
                if(smartSwitch.MotorCurrentSttaus != MONTOR_STOP_RUNNING)//���ͣת
                    smartSwitch.MotorCurrentSttaus = setMontorRunningStatus(MONTOR_STOP_RUNNING);
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
                    if(smartSwitch.MotorCurrentSttaus != MONTOR_RIGHT_RUNNING)//�����ת
                        smartSwitch.MotorCurrentSttaus = setMontorRunningStatus(MONTOR_RIGHT_RUNNING);
                } else {
                    if(smartSwitch.MotorCurrentSttaus != MONTOR_LEFT_RUNNING)//�����ת
                        smartSwitch.MotorCurrentSttaus = setMontorRunningStatus(MONTOR_LEFT_RUNNING);
                }

                //�ȴ����ת����λ������5�����Ϊ�ǳ�ʱ
                wait_switch_on(TIMEOUT_VAL_5S);
                delay_timer(TIMEOUT_DELAY_VAL);
                if(smartSwitch.MotorCurrentSttaus != MONTOR_STOP_RUNNING)//���ͣת
                    smartSwitch.MotorCurrentSttaus = setMontorRunningStatus(MONTOR_STOP_RUNNING);
            }
        }
     }
}

/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt TIMER0_VECTOR //20ms@24.000MHz
{
	// process watch dog signal
	WDT_CONTR &= 0x7F;
    WDT_CONTR |= 0x10;

	//timer out counter
	if(smartSwitch.TimeoutCount <= TIMEOUT_VAL_MAX)
		smartSwitch.TimeoutCount++;

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

        if(smartSwitch.VoltCurrentStatus == UNDER_VOLTAGE)
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
        else if(smartSwitch.VoltCurrentStatus == OVER_VOLTAGE)
        {
            if(VoltExceptionsCount > 25) VoltStatusLamp = 1;
            else VoltStatusLamp = 0;
        }
    }
}

