#include "dlt645.h"
#include "config.h"

// DLT645 Frame EOF/BOF Flag
#define DLT645_FRAME_BOF   0x68
#define DLT645_FRAME_EOF   0x16


//��һ�������У���
unsigned char Get_Chksum(unsigned char *ps,unsigned char length)
{
	unsigned char i = 0;
    unsigned char checkSum = 0;
 
	for(i = 0; i < length; i++) 
        checkSum += ps[i];
 
	return checkSum;		
}

#if 0
//��ַ�˶�
bit Compare_Device_ID(void)
{
	unsigned char i;
	//����豸�ĵ�ַ
	i	=	P3;
//    DeviceAddr[5]=((i >> 2) & 0x0F);
	
//	for(i=0;i<DEVICE_ID_LEN;i++)
	{
		//��ַֻҪ�κ�һ���ֽڲ��Ծ��˳�
		if(DataFrameBuf[i+1] != DeviceAddr[i])	break;
	}
	if(i==DEVICE_ID_LEN) return 1;//��ַ�ȶ���ȷ
	else return 0;//��ַ�ȶԲ���ȷ
}

/**********************************************
��������Relay_CtrlFun����	�̵������ƺ���
����һ��RelayNumber 		�̵������ 1����9,ALL
��������RelayStatusCtrl		�̵���״̬ 1�����ϣ�0���Ͽ�
���ܣ������ƶ���ŵļ̵����򿪻��߹رգ��̵���Ĭ��״̬Ϊ����
***********************************************/
void Relay_CtrlFun(unsigned char RelayNumber,bit RelayStatusCtrl)
{
	switch (RelayNumber)
		{
		case 1:
			Relay_One = RelayStatusCtrl == 1 ? 1:0;
			break;
		case 2:
			Relay_Two = RelayStatusCtrl == 1 ? 1:0;
			break;
		case 3:
			Relay_Three = RelayStatusCtrl == 1 ? 1:0;
			break;
		case 4:
			Relay_Four = RelayStatusCtrl == 1 ? 1:0;
			break;
		case 5:
			Relay_Five = RelayStatusCtrl == 1 ? 1:0;
			break;
		case 6:
			Relay_Six = RelayStatusCtrl == 1 ? 1:0;
			break;
		case 7:
			Relay_Seven = RelayStatusCtrl == 1 ? 1:0;
			break;
		case 8:
			Relay_Eight = RelayStatusCtrl == 1 ? 1:0;
			break;
		case 9:
			Relay_Nine = RelayStatusCtrl == 1 ? 1:0;
			break;
		case 10:
			if (RelayStatusCtrl == 1)
			{
				P1=0xff;
				Relay_Seven = 1;				
			}
			else
			{
				P1=0x00;
				Relay_Seven = 0;	
			}
			break;
		default:
			break;
		}
	
}

void Return_Error_Data(unsigned char ErrorCode)
{
	//�����������
	unsigned char TempDat_L;
	unsigned char TempDat_H;
	TempDat_L=IapReadByte(EERORCOUNT0_ADDR);
	TempDat_H=IapReadByte(EERORCOUNT1_ADDR);
	if (TempDat_L==0xff)
	{
		IapProgramByte(EERORCOUNT0_ADDR,0x00);
		if (TempDat_H==0xff)
		{
			IapProgramByte(EERORCOUNT1_ADDR,0x00);
		}
		else IapProgramByte(EERORCOUNT1_ADDR,TempDat_H+1);
	}
	else IapProgramByte(EERORCOUNT0_ADDR,TempDat_L+1);
	
	//�������ݴ���
	switch (ErrorCode)
	{
		case CTRLCODE_ERROR:	//���������
			DataFrameBuf[CTRL_CODE]=0xEE;
			DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
			break;
			
		case DATACODE_ERROR:	//���������
			DataFrameBuf[10]=0xEE;
			DataFrameBuf[11]=0xEE;
			DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
			break;
			
		case VERYCODE_ERROR:	//У�������
			DataFrameBuf[CTRL_CODE]=0xEE;
			DataFrameBuf[10]=0xEE;
			DataFrameBuf[11]=0xEE;
			DataFrameBuf[12]=0xEE;
			break;
			
		default:break;
	}	
	SendDataFrame(MaxFrameLength,DataFrameBuf); 
}

//����֡����
void DataFrameProcess(void)
{
	//���յ����ݸ������ڵ���ָ��ĸ���14��������յ����ݲ�ȫ������֡����������
	if(pointer_buf485==MaxFrameLength)
	{		//SendByteData(0x18);
		//ES=0;				//�����������֡��ʱ���ֹ��������		
		//�жϵ�һ�͵ڶ���ʼ��68h�Լ�������16h
		if((DataFrameBuf[0]==0x66) && (DataFrameBuf[7]==0x68) && 
							(DataFrameBuf[DATA_LENGTH]==0x02) && (DataFrameBuf[13]==0x16))
		{
			//�ж��豸�ĵ�ַ�뱾����ַ�Ƿ���ͬ
			if(Compare_Device_ID())
			{
				//�ж�У����
				if(Get_Chksum(DataFrameBuf,VERYDAT_NUM)==DataFrameBuf[12])
				{
					if(DataFrameBuf[CTRL_CODE]==0x01) //ȫ������
					{
						if (DataFrameBuf[10]==0x00 && DataFrameBuf[11]==0X7F)	
						{
							Relay_CtrlFun(RELAY_ALL,RELAY_OFF);//���Ƽ̵�������
							
							//�Ѽ̵���״̬д��洢��
							IapProgramByte(STATUS0_ADDR,DataFrameBuf[10]);
							IapProgramByte(STATUS1_ADDR,DataFrameBuf[11]);
							SendDataFrame(MaxFrameLength,DataFrameBuf);							
						}
						else if(DataFrameBuf[10]==0xFF && DataFrameBuf[11]==0X80) 
						{
							Relay_CtrlFun(RELAY_ALL,RELAY_ON);
							IapProgramByte(STATUS0_ADDR,DataFrameBuf[10]);
							IapProgramByte(STATUS1_ADDR,DataFrameBuf[11]);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
						}
						else Return_Error_Data(DATACODE_ERROR);
					}
					else if(DataFrameBuf[CTRL_CODE]==0x02 )//��������ͬʱ���ƶ�·�̵���
					{
						if ((DataFrameBuf[11]==0x00) || (DataFrameBuf[11]==0x7F) || (DataFrameBuf[11]==0x80))	//������ݵ���Ч��
						{
							Relay_CtrlFun(RELAY_5,(DataFrameBuf[10] && 0x01 == 0x01) ? RELAY_ON:RELAY_OFF);
							Relay_CtrlFun(RELAY_4,(DataFrameBuf[10] && 0x02 == 0x02) ? RELAY_ON:RELAY_OFF);
							Relay_CtrlFun(RELAY_3,(DataFrameBuf[10] && 0x04 == 0x04) ? RELAY_ON:RELAY_OFF);
							Relay_CtrlFun(RELAY_2,(DataFrameBuf[10] && 0x08 == 0x08) ? RELAY_ON:RELAY_OFF);
							Relay_CtrlFun(RELAY_1,(DataFrameBuf[10] && 0x10 == 0x10) ? RELAY_ON:RELAY_OFF);
							Relay_CtrlFun(RELAY_8,(DataFrameBuf[10] && 0x20 == 0x20) ? RELAY_ON:RELAY_OFF);
							Relay_CtrlFun(RELAY_9,(DataFrameBuf[10] && 0x40 == 0x40) ? RELAY_ON:RELAY_OFF);
							Relay_CtrlFun(RELAY_6,(DataFrameBuf[10] && 0x80 == 0x80) ? RELAY_ON:RELAY_OFF);
							Relay_CtrlFun(RELAY_7,(DataFrameBuf[11] && 0x80 == 0x80) ? RELAY_ON:RELAY_OFF);

							IapProgramByte(STATUS0_ADDR,DataFrameBuf[10]);
							IapProgramByte(STATUS1_ADDR,DataFrameBuf[11]);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
						}	
						else Return_Error_Data(DATACODE_ERROR);
					}						
					else if(DataFrameBuf[CTRL_CODE]==0x03 )//��ѯ״̬
					{
						DataFrameBuf[10]=IapReadByte(STATUS0_ADDR);
						DataFrameBuf[11]=IapReadByte(STATUS1_ADDR);
						DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
						SendDataFrame(MaxFrameLength,DataFrameBuf);
					}  	
					
					//���⹦�����ô���
					else if (DataFrameBuf[CTRL_CODE]==0x04)//����ϵͳ����
					{
						if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xE8)
						{
							IapProgramByte(BUAD_ADDR,0XE8);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//������ɺ�����ϵͳ
						}//����1200bps
						else if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xF4)
						{
							IapProgramByte(BUAD_ADDR,0XF4);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//������ɺ�����ϵͳ
						}//����2400bps
						else if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xFA)
						{
							IapProgramByte(BUAD_ADDR,0XFA);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//������ɺ�����ϵͳ
						}//����4800bps
						else if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xFD)
						{
							IapProgramByte(BUAD_ADDR,0XFD);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//������ɺ�����ϵͳ
						}//����9600bps
						else if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xFE)
						{
							IapProgramByte(BUAD_ADDR,0XFE);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//������ɺ�����ϵͳ
						}//����19200bps						
						else if (DataFrameBuf[10]==0x02 && DataFrameBuf[11]==0x00)
						{
							IapProgramByte(VERYFIY_ADDR,0X00);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//������ɺ�����ϵͳ
						}//����NONE
						else if (DataFrameBuf[10]==0x02 && DataFrameBuf[11]==0x01)
						{
							IapProgramByte(VERYFIY_ADDR,0X01);
							SendDataFrame(MaxFrameLength,DataFrameBuf);							
							ReBootCtrl=0;//������ɺ�����ϵͳ
						}//����EVEN
						else if (DataFrameBuf[10]==0x02 && DataFrameBuf[11]==0x02)
						{
							IapProgramByte(VERYFIY_ADDR,0X02);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//������ɺ�����ϵͳ
						}//����ODD
						else if (DataFrameBuf[10]==0x03 && DataFrameBuf[11]==0x00)
						{
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;
						}//����ϵͳ
						else if (DataFrameBuf[10]==0x03 && DataFrameBuf[11]==0x01)
						{
							DataFrameBuf[10]=IapReadByte(REBOOT0_ADDR);
							DataFrameBuf[11]=IapReadByte(REBOOT1_ADDR);
							DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
							SendDataFrame(MaxFrameLength,DataFrameBuf);							
						}//����������ѯ						
						else if (DataFrameBuf[10]==0x03 && DataFrameBuf[11]==0x02)
						{
							//����ڴ��м�¼������
							IapProgramByte(REBOOT0_ADDR,0X00);
							IapProgramByte(REBOOT1_ADDR,0X00);
							//��ȡ�ڴ��е�ǰ�����ݲ�����
							DataFrameBuf[10]=IapReadByte(REBOOT0_ADDR);
							DataFrameBuf[11]=IapReadByte(REBOOT1_ADDR);
							DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//����ϵͳ
						}//�����������
						else if (DataFrameBuf[10]==0x04 && DataFrameBuf[11]==0xCF)
						{
							DataFrameBuf[10]=IapReadByte(EERORCOUNT0_ADDR);
							DataFrameBuf[11]=IapReadByte(EERORCOUNT1_ADDR);
							DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
							SendDataFrame(MaxFrameLength,DataFrameBuf);							
						}//���������ѯ
						else if (DataFrameBuf[10]==0x04 && DataFrameBuf[11]==0xCC)
						{
							//����ڴ��м�¼������
							IapProgramByte(EERORCOUNT0_ADDR,0X00);
							IapProgramByte(EERORCOUNT1_ADDR,0X00);
							//��ȡ�ڴ��е�ǰ�����ݲ�����
							DataFrameBuf[10]=IapReadByte(EERORCOUNT0_ADDR);
							DataFrameBuf[11]=IapReadByte(EERORCOUNT1_ADDR);
							DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//����ϵͳ
						}//����������
						else if (DataFrameBuf[10]==0x05 && DataFrameBuf[11]==0x05)
						{
							unsigned char i;
							Sector_Erase(0x0200);
							for (i=0;i<8;i++) IapProgramByte(0x0300+i,InitParameter[i]);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//������ɺ�����ϵͳ
						}//�ָ���������
						else if (DataFrameBuf[10]==0x06 && DataFrameBuf[11]==0x06)
						{
							SendDataFrame(MaxFrameLength,DataFrameBuf);
						}//����Ӧ����ָ��
						else if (DataFrameBuf[10]==0x07 && DataFrameBuf[11]==0x07)
						{
							unsigned char i;
							for (i=0;i<8;i++) SendByteData(IapReadByte(0x0300+i));							
						}//��ȡEEPROM�еò���
						else Return_Error_Data(DATACODE_ERROR);//ϵͳ���ô���	
					}
					else Return_Error_Data(CTRLCODE_ERROR);//���������					
				}
				else Return_Error_Data(VERYCODE_ERROR);//У�������		
			}	
		}		
		pointer_buf485=0;//���ݽ�����ɺ���Կ�ʼ��������	
	}			
			
}	
#endif

	