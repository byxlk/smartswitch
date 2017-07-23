
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


//���ڷ���һ���ֽ�����
void SendByteData(unsigned char dat)
{
	//ES=0;  //�ش����ж�
    TI=0;  //���㴮�ڷ�������ж������־
    SBUF=dat;
    while(TI ==0); //�ȴ��������
    TI=0;  //���㴮�ڷ�������ж������־
    //ES=1;  //�������ж�
}

//����һ֡���ݰ�
void SendDataFrame(unsigned char Length,unsigned char *str)
{
	unsigned char i;
//	Rs485Enable = TransferEnable;//485��������ʹ��
//	DelayFun(600);
	for (i=0;i<Length;i++)
	{
		SendByteData(*(str++));
//        DelayFun(20);
	}
//	Rs485Enable = RevEnable;//���ݷ������485�ȴ���������
//	DelayFun(600);
}







































































































