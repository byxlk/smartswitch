
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

//#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		 5529600L	//定义主时钟
//#define MAIN_Fosc		24000000L	//定义主时钟


/*********************************************************/
#include	"STC15Fxxxx.H"

//#define DEBUG

#define ENABLE_BOOT_4S_DELAY_AFTER_COLDRESET 0
#define ENABLE_OUT_OFF_HOOK_DELAY 0
#define ENABLE_CPU_SLEEP_SUPPORT 1

#define POWER_OK 1            //短路信号正常
#define POWER_ERR 0           //短路信号异常
#define AUTO_CTRL_MODE 1      //自动控制模式
#define EX_CTRL_SIGNAL_LOSS 1 //外部控制信号信号丢失
#define EX_CTRL_SIGNAL_OK 0   //外部控制信号信号正常
#define OUT_OF_HOOK 0         //脱扣到位
#define SWITCH_ON_OK 0        //合闸到位

#define MONTOR_RIGHT_RUNNING 0x01
#define MONTOR_LEFT_RUNNING 0x10
#define MONTOR_STOP_RUNNING 0xff

#define TIMEOUT_VAL_MAX 200 //about 10s， 5s max is 250
#define TIMEOUT_VAL_9S 180 //about 2s， 5s max is 250
#define TIMEOUT_VAL_8S 160 //about 2s， 5s max is 250
#define TIMEOUT_VAL_7S 140 //about 2s， 5s max is 250
#define TIMEOUT_VAL_6S 120 //about 2s， 5s max is 250
#define TIMEOUT_VAL_5S 100 //about 2s， 5s max is 250
#define TIMEOUT_VAL_4S 80 //about 2s， 5s max is 250
#define TIMEOUT_VAL_3S 60 //about 2s， 5s max is 250
#define TIMEOUT_VAL_2S5 50 //about 2s， 5s max is 250
#define TIMEOUT_VAL_2S 40 //about 2s， 5s max is 250
#define TIMEOUT_VAL_1S5 30 //about 2s， 5s max is 250
#define TIMEOUT_VAL_1S 20 //about 2s， 5s max is 250
#define TIMEOUT_VAL_HS 10 //about 0.5s， 5s max is 250
#define TIMEOUT_DELAY_VAL 4

#define TIMECOUNTER_START 0
#define STOP_TIMERCOUNT 0xFF

#define EEPROM_ADDR 0x0200

sbit  MotorRunningCtrl_R = P3^7;    //P36=0 P37=1 --- 电机正传
sbit  MotorRunningCtrl_L = P3^6;    //P36=1 P37=0 --- 电机反转
sbit  ExAutoCtrlSignal = P1^0;      //外部控制信号 1: 控制信号丢失 0：有控制信号
sbit  SystemWorkMode = P1^1;        //1：自动  0：手动
sbit  SwitchOnStatus = P1^2;        //0: 合闸到位 1：合闸不到位
sbit  OutOffHookCheck = P1^3;        //脱扣位置检测 0: 脱扣到位 1:没有脱扣
sbit  SystemSleepStatus = P1^4;      //睡眠指示灯 0：系统掉电  1：系统唤醒

#endif
