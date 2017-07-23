#include "dlt645.h"
#include "config.h"

// DLT645 Frame EOF/BOF Flag
#define DLT645_FRAME_BOF   0x68
#define DLT645_FRAME_EOF   0x16


//求一个数组的校验和
unsigned char Get_Chksum(unsigned char *ps,unsigned char length)
{
	unsigned char i = 0;
    unsigned char checkSum = 0;
 
	for(i = 0; i < length; i++) 
        checkSum += ps[i];
 
	return checkSum;		
}

#if 0
//地址核对
bit Compare_Device_ID(void)
{
	unsigned char i;
	//获得设备的地址
	i	=	P3;
//    DeviceAddr[5]=((i >> 2) & 0x0F);
	
//	for(i=0;i<DEVICE_ID_LEN;i++)
	{
		//地址只要任何一个字节不对就退出
		if(DataFrameBuf[i+1] != DeviceAddr[i])	break;
	}
	if(i==DEVICE_ID_LEN) return 1;//地址比对正确
	else return 0;//地址比对不正确
}

/**********************************************
函数名：Relay_CtrlFun（）	继电器控制函数
参数一：RelayNumber 		继电器编号 1——9,ALL
参数二：RelayStatusCtrl		继电器状态 1：吸合，0：断开
功能：控制制定编号的继电器打开或者关闭，继电器默认状态为吸合
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
	//错误计数处理
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
	
	//返回数据处理
	switch (ErrorCode)
	{
		case CTRLCODE_ERROR:	//控制码错误
			DataFrameBuf[CTRL_CODE]=0xEE;
			DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
			break;
			
		case DATACODE_ERROR:	//数据域错误
			DataFrameBuf[10]=0xEE;
			DataFrameBuf[11]=0xEE;
			DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
			break;
			
		case VERYCODE_ERROR:	//校验码错误
			DataFrameBuf[CTRL_CODE]=0xEE;
			DataFrameBuf[10]=0xEE;
			DataFrameBuf[11]=0xEE;
			DataFrameBuf[12]=0xEE;
			break;
			
		default:break;
	}	
	SendDataFrame(MaxFrameLength,DataFrameBuf); 
}

//数据帧处理
void DataFrameProcess(void)
{
	//接收的数据个数大于等于指令的个数14，否则接收的数据不全，数据帧丢掉不处理
	if(pointer_buf485==MaxFrameLength)
	{		//SendByteData(0x18);
		//ES=0;				//处理解析数据帧的时候禁止接收数据		
		//判断第一和第二起始符68h以及结束符16h
		if((DataFrameBuf[0]==0x66) && (DataFrameBuf[7]==0x68) && 
							(DataFrameBuf[DATA_LENGTH]==0x02) && (DataFrameBuf[13]==0x16))
		{
			//判断设备的地址与本机地址是否相同
			if(Compare_Device_ID())
			{
				//判断校验码
				if(Get_Chksum(DataFrameBuf,VERYDAT_NUM)==DataFrameBuf[12])
				{
					if(DataFrameBuf[CTRL_CODE]==0x01) //全部控制
					{
						if (DataFrameBuf[10]==0x00 && DataFrameBuf[11]==0X7F)	
						{
							Relay_CtrlFun(RELAY_ALL,RELAY_OFF);//控制继电器动作
							
							//把继电器状态写入存储器
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
					else if(DataFrameBuf[CTRL_CODE]==0x02 )//单独或者同时控制多路继电器
					{
						if ((DataFrameBuf[11]==0x00) || (DataFrameBuf[11]==0x7F) || (DataFrameBuf[11]==0x80))	//检测数据的有效性
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
					else if(DataFrameBuf[CTRL_CODE]==0x03 )//查询状态
					{
						DataFrameBuf[10]=IapReadByte(STATUS0_ADDR);
						DataFrameBuf[11]=IapReadByte(STATUS1_ADDR);
						DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
						SendDataFrame(MaxFrameLength,DataFrameBuf);
					}  	
					
					//特殊功能设置代码
					else if (DataFrameBuf[CTRL_CODE]==0x04)//设置系统参数
					{
						if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xE8)
						{
							IapProgramByte(BUAD_ADDR,0XE8);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//设置完成后重启系统
						}//设置1200bps
						else if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xF4)
						{
							IapProgramByte(BUAD_ADDR,0XF4);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//设置完成后重启系统
						}//设置2400bps
						else if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xFA)
						{
							IapProgramByte(BUAD_ADDR,0XFA);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//设置完成后重启系统
						}//设置4800bps
						else if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xFD)
						{
							IapProgramByte(BUAD_ADDR,0XFD);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//设置完成后重启系统
						}//设置9600bps
						else if (DataFrameBuf[10]==0x01 && DataFrameBuf[11]==0xFE)
						{
							IapProgramByte(BUAD_ADDR,0XFE);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//设置完成后重启系统
						}//设置19200bps						
						else if (DataFrameBuf[10]==0x02 && DataFrameBuf[11]==0x00)
						{
							IapProgramByte(VERYFIY_ADDR,0X00);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//设置完成后重启系统
						}//设置NONE
						else if (DataFrameBuf[10]==0x02 && DataFrameBuf[11]==0x01)
						{
							IapProgramByte(VERYFIY_ADDR,0X01);
							SendDataFrame(MaxFrameLength,DataFrameBuf);							
							ReBootCtrl=0;//设置完成后重启系统
						}//设置EVEN
						else if (DataFrameBuf[10]==0x02 && DataFrameBuf[11]==0x02)
						{
							IapProgramByte(VERYFIY_ADDR,0X02);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//设置完成后重启系统
						}//设置ODD
						else if (DataFrameBuf[10]==0x03 && DataFrameBuf[11]==0x00)
						{
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;
						}//重启系统
						else if (DataFrameBuf[10]==0x03 && DataFrameBuf[11]==0x01)
						{
							DataFrameBuf[10]=IapReadByte(REBOOT0_ADDR);
							DataFrameBuf[11]=IapReadByte(REBOOT1_ADDR);
							DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
							SendDataFrame(MaxFrameLength,DataFrameBuf);							
						}//重启计数查询						
						else if (DataFrameBuf[10]==0x03 && DataFrameBuf[11]==0x02)
						{
							//清除内存中记录的数据
							IapProgramByte(REBOOT0_ADDR,0X00);
							IapProgramByte(REBOOT1_ADDR,0X00);
							//读取内存中当前的数据并发送
							DataFrameBuf[10]=IapReadByte(REBOOT0_ADDR);
							DataFrameBuf[11]=IapReadByte(REBOOT1_ADDR);
							DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//重启系统
						}//清除重启计数
						else if (DataFrameBuf[10]==0x04 && DataFrameBuf[11]==0xCF)
						{
							DataFrameBuf[10]=IapReadByte(EERORCOUNT0_ADDR);
							DataFrameBuf[11]=IapReadByte(EERORCOUNT1_ADDR);
							DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
							SendDataFrame(MaxFrameLength,DataFrameBuf);							
						}//错误计数查询
						else if (DataFrameBuf[10]==0x04 && DataFrameBuf[11]==0xCC)
						{
							//清除内存中记录的数据
							IapProgramByte(EERORCOUNT0_ADDR,0X00);
							IapProgramByte(EERORCOUNT1_ADDR,0X00);
							//读取内存中当前的数据并发送
							DataFrameBuf[10]=IapReadByte(EERORCOUNT0_ADDR);
							DataFrameBuf[11]=IapReadByte(EERORCOUNT1_ADDR);
							DataFrameBuf[12]=Get_Chksum(DataFrameBuf,VERYDAT_NUM);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//重启系统
						}//清除错误计数
						else if (DataFrameBuf[10]==0x05 && DataFrameBuf[11]==0x05)
						{
							unsigned char i;
							Sector_Erase(0x0200);
							for (i=0;i<8;i++) IapProgramByte(0x0300+i,InitParameter[i]);
							SendDataFrame(MaxFrameLength,DataFrameBuf);
							ReBootCtrl=0;//设置完成后重启系统
						}//恢复出厂设置
						else if (DataFrameBuf[10]==0x06 && DataFrameBuf[11]==0x06)
						{
							SendDataFrame(MaxFrameLength,DataFrameBuf);
						}//自适应测试指令
						else if (DataFrameBuf[10]==0x07 && DataFrameBuf[11]==0x07)
						{
							unsigned char i;
							for (i=0;i<8;i++) SendByteData(IapReadByte(0x0300+i));							
						}//读取EEPROM中得参数
						else Return_Error_Data(DATACODE_ERROR);//系统设置错误	
					}
					else Return_Error_Data(CTRLCODE_ERROR);//控制码错误					
				}
				else Return_Error_Data(VERYCODE_ERROR);//校验码出错		
			}	
		}		
		pointer_buf485=0;//数据解码完成后可以开始接受数据	
	}			
			
}	
#endif

	