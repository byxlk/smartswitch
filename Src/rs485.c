
#include "rs485.h"
#include "config.h"


static void rs485_enable_send(void)
{
    RS485_Recv_Send_Enable = 1;
}

static void rs485_enable_recv(void)
{
    RS485_Recv_Send_Enable = 0;
}


//串口发送一个字节数据
void SendByteData(unsigned char dat)
{
	//ES=0;  //关串口中断
    TI=0;  //清零串口发送完成中断请求标志
    SBUF=dat;
    while(TI ==0); //等待发送完成
    TI=0;  //清零串口发送完成中断请求标志
    //ES=1;  //允许串口中断
}

//发送一帧数据包
void SendDataFrame(unsigned char Length,unsigned char *str)
{
	unsigned char i;
//	Rs485Enable = TransferEnable;//485发送数据使能
//	DelayFun(600);
	for (i=0;i<Length;i++)
	{
		SendByteData(*(str++));
//        DelayFun(20);
	}
//	Rs485Enable = RevEnable;//数据发送完毕485等待接受数据
//	DelayFun(600);
}







































































































