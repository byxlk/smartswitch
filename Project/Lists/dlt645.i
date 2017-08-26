
#line 1 "..\Src\dlt645.c" /0
  
#line 1 "..\Src\dlt645.h" /0
 
 
 
 
 
 
#line 1 "..\Src\dlt645.c" /0
 
  
#line 1 "..\Src\config.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
#line 1 "..\Src\STC15Fxxxx.H" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
#line 1 "C:\Keil_c51\C51\Inc\intrins.h" /0






 
 
 
 
 
 #pragma SAVE
 
 
#line 15 "C:\Keil_c51\C51\Inc\intrins.h" /1
 
 
 
#line 18 "C:\Keil_c51\C51\Inc\intrins.h" /0
 
 extern void          _nop_     (void);
 extern bit           _testbit_ (bit);
 extern unsigned char _cror_    (unsigned char, unsigned char);
 extern unsigned int  _iror_    (unsigned int,  unsigned char);
 extern unsigned long _lror_    (unsigned long, unsigned char);
 extern unsigned char _crol_    (unsigned char, unsigned char);
 extern unsigned int  _irol_    (unsigned int,  unsigned char);
 extern unsigned long _lrol_    (unsigned long, unsigned char);
 extern unsigned char _chkfloat_(float);
 
#line 29 "C:\Keil_c51\C51\Inc\intrins.h" /1
 
 
#line 31 "C:\Keil_c51\C51\Inc\intrins.h" /0
 
 extern void          _push_    (unsigned char _sfr);
 extern void          _pop_     (unsigned char _sfr);
 
 
 #pragma RESTORE
 
 
 
#line 19 "..\Src\STC15Fxxxx.H" /0
 
 
 
 sfr P0   = 0x80;
 sfr SP   = 0x81;
 sfr DPL  = 0x82;
 sfr DPH  = 0x83;
 sfr	S4CON = 0x84;
 sfr	S4BUF = 0x85;
 sfr PCON = 0x87;
 
 sfr TCON = 0x88;
 sfr TMOD = 0x89;
 sfr TL0  = 0x8A;
 sfr TL1  = 0x8B;
 sfr TH0  = 0x8C;
 sfr TH1  = 0x8D;
 sfr	AUXR = 0x8E;
 sfr WAKE_CLKO = 0x8F;
 sfr INT_CLKO = 0x8F;
 sfr	AUXR2    = 0x8F;
 
 sfr RL_TL0  = 0x8A;
 sfr RL_TL1  = 0x8B;
 sfr RL_TH0  = 0x8C;
 sfr RL_TH1  = 0x8D;
 
 
 sfr P1   = 0x90;
 sfr P1M1 = 0x91;	 
 sfr P1M0 = 0x92;	 
 sfr P0M1 = 0x93;	 
 sfr P0M0 = 0x94;	 
 sfr P2M1 = 0x95;	 
 sfr P2M0 = 0x96;	 
 sfr CLK_DIV = 0x97;
 sfr PCON2   = 0x97;
 
 sfr SCON  = 0x98;
 sfr SBUF  = 0x99;
 sfr S2CON = 0x9A;	 
 sfr S2BUF = 0x9B;	 
 sfr P1ASF = 0x9D;	 
 
 sfr P2    = 0xA0;
 sfr BUS_SPEED = 0xA1;
 sfr AUXR1 = 0xA2;
 sfr P_SW1 = 0xA2;
 
 sfr IE    = 0xA8;
 sfr SADDR = 0xA9;
 sfr WKTCL = 0xAA;	 
 sfr WKTCH = 0xAB;	 
 sfr	S3CON = 0xAC;
 sfr S3BUF = 0xAD;
 sfr IE2   = 0xAF;	 
 
 sfr P3    = 0xB0;
 sfr P3M1  = 0xB1;	 
 sfr P3M0  = 0xB2;	 
 sfr P4M1  = 0xB3;	 
 sfr P4M0  = 0xB4;	 
 sfr IP2   = 0xB5;	 
 sfr IPH2  = 0xB6;	 
 sfr IPH   = 0xB7;
 
 sfr IP        = 0xB8;
 sfr SADEN     = 0xB9;
 sfr	P_SW2     = 0xBA;
 sfr ADC_CONTR = 0xBC;	 
 sfr ADC_RES   = 0xBD;	 
 sfr ADC_RESL  = 0xBE;	 
 
 sfr P4        = 0xC0;
 sfr WDT_CONTR = 0xC1;
 sfr IAP_DATA  = 0xC2;
 sfr IAP_ADDRH = 0xC3;
 sfr IAP_ADDRL = 0xC4;
 sfr IAP_CMD   = 0xC5;
 sfr IAP_TRIG  = 0xC6;
 sfr IAP_CONTR = 0xC7;
 
 sfr ISP_DATA  = 0xC2;
 sfr ISP_ADDRH = 0xC3;
 sfr ISP_ADDRL = 0xC4;
 sfr ISP_CMD   = 0xC5;
 sfr ISP_TRIG  = 0xC6;
 sfr ISP_CONTR = 0xC7;
 
 sfr P5     = 0xC8;	 
 sfr P5M1   = 0xC9;	 
 sfr P5M0   = 0xCA;	 
 sfr P6M1   = 0xCB;	 
 sfr P6M0   = 0xCC;	 
 sfr SPSTAT = 0xCD;	 
 sfr SPCTL  = 0xCE;	 
 sfr SPDAT  = 0xCF;	 
 
 sfr PSW  = 0xD0;
 sfr	T4T3M = 0xD1;
 sfr	T4H  = 0xD2;
 sfr	T4L  = 0xD3;
 sfr	T3H  = 0xD4;
 sfr	T3L  = 0xD5;
 sfr	T2H  = 0xD6;
 sfr	T2L  = 0xD7;
 
 sfr	TH4  = 0xD2;
 sfr	TL4  = 0xD3;
 sfr	TH3  = 0xD4;
 sfr	TL3  = 0xD5;
 sfr	TH2  = 0xD6;
 sfr	TL2  = 0xD7;
 
 sfr	RL_T4H  = 0xD2;
 sfr	RL_T4L  = 0xD3;
 sfr	RL_T3H  = 0xD4;
 sfr	RL_T3L  = 0xD5;
 sfr	RL_T2H  = 0xD6;
 sfr	RL_T2L  = 0xD7;
 
 sfr CCON = 0xD8;	 
 sfr CMOD = 0xD9;	 
 sfr CCAPM0 = 0xDA;	 
 sfr CCAPM1 = 0xDB;	 
 sfr CCAPM2 = 0xDC;	 
 
 sfr ACC    = 0xE0;
 sfr	P7M1   = 0xE1;
 sfr	P7M0   = 0xE2;
 sfr	CMPCR1 = 0xE6;
 sfr	CMPCR2 = 0xE7;
 
 
 sfr	P6     = 0xE8;
 sfr CL     = 0xE9;	 
 sfr CCAP0L = 0xEA;	 
 sfr CCAP1L = 0xEB;	 
 sfr CCAP2L = 0xEC;	 
 
 sfr B      = 0xF0;
 sfr PCA_PWM0 = 0xF2;	 
 sfr PCA_PWM1 = 0xF3;	 
 sfr PCA_PWM2 = 0xF4;	 
 
 sfr	P7     = 0xF8;
 sfr CH     = 0xF9;
 sfr CCAP0H = 0xFA;		 
 sfr CCAP1H = 0xFB;		 
 sfr CCAP2H = 0xFC;		 
 
 
 
 
 sbit CY   = PSW^7;
 sbit AC   = PSW^6;
 sbit F0   = PSW^5;
 sbit RS1  = PSW^4;
 sbit RS0  = PSW^3;
 sbit OV   = PSW^2;
 sbit F1   = PSW^1;
 sbit P    = PSW^0;
 
 
 sbit TF1  = TCON^7;	 
 sbit TR1  = TCON^6;	 
 sbit TF0  = TCON^5;	 
 sbit TR0  = TCON^4;	 
 sbit IE1  = TCON^3;	 
 sbit IT1  = TCON^2;	 
 sbit IE0  = TCON^1;	 
 sbit IT0  = TCON^0;	 
 
 
 sbit  P00 = P0^0;
 sbit  P01 = P0^1;
 sbit  P02 = P0^2;
 sbit  P03 = P0^3;
 sbit  P04 = P0^4;
 sbit  P05 = P0^5;
 sbit  P06 = P0^6;
 sbit  P07 = P0^7;
 
 
 sbit  P10 = P1^0;
 sbit  P11 = P1^1;
 sbit  P12 = P1^2;
 sbit  P13 = P1^3;
 sbit  P14 = P1^4;
 sbit  P15 = P1^5;
 sbit  P16 = P1^6;
 sbit  P17 = P1^7;
 
 sbit  RXD2      = P1^0;
 sbit  TXD2      = P1^1;
 sbit  CCP1      = P1^0;
 sbit  CCP0      = P1^1;
 sbit  SPI_SS    = P1^2;
 sbit  SPI_MOSI  = P1^3;
 sbit  SPI_MISO  = P1^4;
 sbit  SPI_SCLK  = P1^5;
 
 
 sbit  P20 = P2^0;
 sbit  P21 = P2^1;
 sbit  P22 = P2^2;
 sbit  P23 = P2^3;
 sbit  P24 = P2^4;
 sbit  P25 = P2^5;
 sbit  P26 = P2^6;
 sbit  P27 = P2^7;
 
 
 sbit  P30 = P3^0;
 sbit  P31 = P3^1;
 sbit  P32 = P3^2;
 sbit  P33 = P3^3;
 sbit  P34 = P3^4;
 sbit  P35 = P3^5;
 sbit  P36 = P3^6;
 sbit  P37 = P3^7;
 
 sbit RXD  = P3^0;
 sbit TXD  = P3^1;
 sbit INT0 = P3^2;
 sbit INT1 = P3^3;
 sbit T0   = P3^4;
 sbit T1   = P3^5;
 sbit WR   = P3^6;
 sbit RD   = P3^7;
 sbit CCP2  = P3^7;
 
 sbit CLKOUT0   = P3^5;
 sbit CLKOUT1   = P3^4;
 
 
 sbit  P40 = P4^0;
 sbit  P41 = P4^1;
 sbit  P42 = P4^2;
 sbit  P43 = P4^3;
 sbit  P44 = P4^4;
 sbit  P45 = P4^5;
 sbit  P46 = P4^6;
 sbit  P47 = P4^7;
 
 
 sbit  P50 = P5^0;
 sbit  P51 = P5^1;
 sbit  P52 = P5^2;
 sbit  P53 = P5^3;
 sbit  P54 = P5^4;
 sbit  P55 = P5^5;
 sbit  P56 = P5^6;
 sbit  P57 = P5^7;
 
 
 sbit SM0  = SCON^7;	 
 sbit SM1  = SCON^6;	 
 sbit SM2  = SCON^5;	 
 sbit REN  = SCON^4;	 
 sbit TB8  = SCON^3;	 
 sbit RB8  = SCON^2;	 
 sbit TI   = SCON^1;	 
 sbit RI   = SCON^0;	 
 
 
 sbit EA   = IE^7;	 
 sbit ELVD = IE^6;	 
 sbit EADC = IE^5;	 
 sbit ES   = IE^4;	 
 sbit ET1  = IE^3;	 
 sbit EX1  = IE^2;	 
 sbit ET0  = IE^1;	 
 sbit EX0  = IE^0;	 
 
 
 









 
 
 sbit ACC0 = ACC^0;
 sbit ACC1 = ACC^1;
 sbit ACC2 = ACC^2;
 sbit ACC3 = ACC^3;
 sbit ACC4 = ACC^4;
 sbit ACC5 = ACC^5;
 sbit ACC6 = ACC^6;
 sbit ACC7 = ACC^7;
 
 sbit B0 = B^0;
 sbit B1 = B^1;
 sbit B2 = B^2;
 sbit B3 = B^3;
 sbit B4 = B^4;
 sbit B5 = B^5;
 sbit B6 = B^6;
 sbit B7 = B^7;
 
 
 
 
 
 
 
 
 
 
 
 
 sbit PPCA	= IP^7;	 
 sbit PLVD	= IP^6;	 
 sbit PADC	= IP^5;	 
 sbit PS   	= IP^4;	 
 sbit PT1	= IP^3;	 
 sbit PX1	= IP^2;	 
 sbit PT0	= IP^1;	 
 sbit PX0	= IP^0;	 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 542 "..\Src\STC15Fxxxx.H" /1
  
 
  
 
  
 
  
 
  
 
  
 
  
 
#line 556 "..\Src\STC15Fxxxx.H" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 sbit CCF0  = CCON^0;	 
 sbit CCF1  = CCON^1;	 
 sbit CCF2  = CCON^2;	 
 sbit CR    = CCON^6;	 
 sbit CF    = CCON^7;	 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 























 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 typedef 	unsigned char	u8;
 typedef 	unsigned int	u16;
 typedef 	unsigned long	u32;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 









 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 29 "..\Src\config.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 sbit  SwitchStatus           = P1^0;       
 sbit  SystemWorkMode         = P1^1;       
 sbit  VoltStatusLamp         = P1^2;       
 sbit  VoltStatusPlus         = P3^3;       
 sbit  VoltCapturePortA       = P1^3;       
 sbit  VoltCapturePortB       = P1^4;       
 sbit  VoltCapturePortC       = P1^5;       
 sbit  SwitchOnStatus         = P5^4;       
 sbit  OutOffHookCheck        = P5^5;       
 
 sbit  RS485_Recv_Send_Enable = P3^2;       
 sbit  MotorRunningCtrl_R     = P3^7;       
 sbit  MotorRunningCtrl_L     = P3^6;       
 
 
#line 2 "..\Src\dlt645.c" /0
 
 
 
 
 
 
 
 
 unsigned char Get_Chksum(unsigned char *ps,unsigned char length)
 {
 unsigned char i = 0;
 unsigned char checkSum = 0;
 
 for(i = 0; i < length; i++) 
 checkSum += ps[i];
 
 return checkSum;		
 }
 
 
#line 22 "..\Src\dlt645.c" /1
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 





 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 326 "..\Src\dlt645.c" /0
 
 
