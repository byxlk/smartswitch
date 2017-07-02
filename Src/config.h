
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


#ifndef		__CONFIG_H
#define		__CONFIG_H


/*********************************************************/

//#define MAIN_Fosc		22118400L	//������ʱ��
//#define MAIN_Fosc		12000000L	//������ʱ��
#define MAIN_Fosc		11059200L	//������ʱ��
//#define MAIN_Fosc		 5529600L	//������ʱ��
//#define MAIN_Fosc		24000000L	//������ʱ��


/*********************************************************/
#include	"STC15Fxxxx.H"

#define DEBUG

#define POWER_OK 1            //��·�ź�����
#define POWER_ERR 0           //��·�ź��쳣
#define AUTO_CTRL_MODE 1      //�Զ�����ģʽ
#define EX_CTRL_SIGNAL_LOSS 1 //�ⲿ�����ź��źŶ�ʧ
#define EX_CTRL_SIGNAL_OK 0   //�ⲿ�����ź��ź�����
#define OUT_OF_HOOK 0         //�ѿ۵�λ
#define SWITCH_ON_OK 0        //��բ��λ

#define MONTOR_RIGHT_RUNNING 0x01
#define MONTOR_LEFT_RUNNING 0x10
#define MONTOR_STOP_RUNNING 0xff

#define TIMEOUT_VAL_MAX 200 //about 10s�� 5s max is 250
#define TIMEOUT_VAL_9S 180 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_8S 160 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_7S 140 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_6S 120 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_5S 100 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_4S 80 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_3S 60 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_2S5 50 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_2S 40 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_1S5 30 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_1S 20 //about 2s�� 5s max is 250
#define TIMEOUT_VAL_HS 10 //about 0.5s�� 5s max is 250

#define TIMECOUNTER_START 0
#define STOP_TIMERCOUNT 0xFF

#define EEPROM_ADDR 0x0200

sbit  MotorRunningCtrl_R = P3^7;    //P36=0 P37=1 --- �������
sbit  MotorRunningCtrl_L = P3^6;    //P36=1 P37=0 --- �����ת
sbit  ExAutoCtrlSignal = P1^0;      //�ⲿ�����ź� 1: �����źŶ�ʧ 0���п����ź�
sbit  SystemWorkMode = P1^1;        //1���Զ�  0���ֶ�
sbit  SwitchOnStatus = P1^2;        //0: ��բ��λ 1����բ����λ
sbit  OutOffHookCheck = P1^3;        //�ѿ�λ�ü�� 0: �ѿ۵�λ 1:û���ѿ�
sbit  SystemSleepStatus = P1^4;      //˯��ָʾ�� 0��ϵͳ����  1��ϵͳ����

#endif