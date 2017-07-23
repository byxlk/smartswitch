void DelayFun(unsigned int delay)
{
    while(delay>0) delay--;
}

void IAP_Disable()
{
    //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    IAP_CONTR = 0;      //关闭IAP 功能
    IAP_CMD   = CMD_IDLE;      //清命令寄存器,使命令寄存器无命令,此句可不用
    IAP_TRIG  = 0;      //清命令触发寄存器,使命令触发寄存器无触发,此句可不用
    IAP_ADDRH = 0x80;
    IAP_ADDRL = 0;
}

//读一字节，调用前需打开IAP 功能，入口:DPTR = 字节地址，返回:A = 读出字节
unsigned char IapReadByte(unsigned int addr)
{
    IAP_ADDRL = addr;    //设置目标单元地址的高8 位地址
    IAP_ADDRH = addr>>8;    //设置目标单元地址的低8 位地址
 
	//EA=0;
	IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = CMD_READ;                 //IAP/ISP/EEPROM 字节读命令
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
   
	IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    return (IAP_DATA);
}

//擦除扇区, 入口:DPTR = 扇区地址
void Sector_Erase(unsigned int addr)
{
    IAP_ADDRL = addr;    //设置目标单元地址的高8 位地址
    IAP_ADDRH = addr>>8;    //设置目标单元地址的低8 位地址

   	IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = CMD_ERASE;                 //IAP/ISP/EEPROM 扇区擦除命令
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
	_nop_();
   	 
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}
//void IapProgramByte(unsigned int addr, unsigned char dat)
void ProgramByteData(unsigned int addr, unsigned char dat)
{
	IAP_ADDRL = addr;    //设置目标单元地址的高8 位地址
    IAP_ADDRH = addr>>8;    //设置目标单元地址的低8 位地址
	
	IAP_CONTR = ENABLE_ISP;         //打开 IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = CMD_PROGRAM;                 //IAP/ISP/EEPROM 字节编程命令
    IAP_DATA = dat;                  //要编程的数据先送进IAP_DATA 寄存器
   
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    	
	IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

//字节编程，调用前需打开IAP 功能，入口:DPTR = 字节地址, A= 须编程字节的数据
void IapProgramByte(unsigned int addr, unsigned char dat)
{
	unsigned char i;
	unsigned char TempDataArray[6]={0};

	for (i=0;i<6;i++) TempDataArray[i]=IapReadByte(0x0300+i);
	
    if(addr<0x0200) 	Sector_Erase(0);//写数据前腰先擦除
	else  Sector_Erase(0x0200);
	
	switch (addr)
    {
		case 0x0300: TempDataArray[0]=dat;break;
		case 0x0301: TempDataArray[1]=dat;break;
		case 0x0302: TempDataArray[2]=dat;break;
		case 0x0303: TempDataArray[3]=dat;break;
		case 0x0304: TempDataArray[4]=dat;break;
		case 0x0305: TempDataArray[5]=dat;break;
		default: break;
	}

	for (i=0;i<6;i++) ProgramByteData(0x0300+i,TempDataArray[i]);	
}