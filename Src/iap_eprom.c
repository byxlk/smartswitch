void DelayFun(unsigned int delay)
{
    while(delay>0) delay--;
}

void IAP_Disable()
{
    //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    IAP_CONTR = 0;      //�ر�IAP ����
    IAP_CMD   = CMD_IDLE;      //������Ĵ���,ʹ����Ĵ���������,�˾�ɲ���
    IAP_TRIG  = 0;      //��������Ĵ���,ʹ������Ĵ����޴���,�˾�ɲ���
    IAP_ADDRH = 0x80;
    IAP_ADDRL = 0;
}

//��һ�ֽڣ�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ������:A = �����ֽ�
unsigned char IapReadByte(unsigned int addr)
{
    IAP_ADDRL = addr;    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRH = addr>>8;    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ
 
	//EA=0;
	IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = CMD_READ;                 //IAP/ISP/EEPROM �ֽڶ�����
    IAP_TRIG = 0x5A;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = 0xA5;   //����A5h ��ISP/IAP ����������������
    _nop_();
   
	IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    return (IAP_DATA);
}

//��������, ���:DPTR = ������ַ
void Sector_Erase(unsigned int addr)
{
    IAP_ADDRL = addr;    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRH = addr>>8;    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

   	IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = CMD_ERASE;                 //IAP/ISP/EEPROM ������������
    IAP_TRIG = 0x5A;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = 0xA5;   //����A5h ��ISP/IAP ����������������
	_nop_();
   	 
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}
//void IapProgramByte(unsigned int addr, unsigned char dat)
void ProgramByteData(unsigned int addr, unsigned char dat)
{
	IAP_ADDRL = addr;    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRH = addr>>8;    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ
	
	IAP_CONTR = ENABLE_ISP;         //�� IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = CMD_PROGRAM;                 //IAP/ISP/EEPROM �ֽڱ������
    IAP_DATA = dat;                  //Ҫ��̵��������ͽ�IAP_DATA �Ĵ���
   
    IAP_TRIG = 0x5A;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = 0xA5;   //����A5h ��ISP/IAP ����������������
    _nop_();
    	
	IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

//�ֽڱ�̣�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ, A= �����ֽڵ�����
void IapProgramByte(unsigned int addr, unsigned char dat)
{
	unsigned char i;
	unsigned char TempDataArray[6]={0};

	for (i=0;i<6;i++) TempDataArray[i]=IapReadByte(0x0300+i);
	
    if(addr<0x0200) 	Sector_Erase(0);//д����ǰ���Ȳ���
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