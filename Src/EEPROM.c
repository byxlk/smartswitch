
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


//	��������STCϵ�е�����EEPROM��д����

#include "config.h"
#include "eeprom.h"


//========================================================================
// ����: void	ISP_Disable(void)
// ����: ��ֹ����ISP/IAP.
// ����: non.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void	DisableEEPROM(void)
{
	ISP_CONTR = 0;			//��ֹISP/IAP����
	ISP_CMD   = 0;			//ȥ��ISP/IAP����
	ISP_TRIG  = 0;			//��ֹISP/IAP�����󴥷�
	ISP_ADDRH = 0xff;		//��0��ַ���ֽ�
	ISP_ADDRL = 0xff;		//��0��ַ���ֽڣ�ָ���EEPROM������ֹ�����
}

//========================================================================
// ����: void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
// ����: ��ָ��EEPROM�׵�ַ����n���ֽڷ�ָ���Ļ���.
// ����: EE_address:  ����EEPROM���׵�ַ.
//       DataAddress: �������ݷŻ�����׵�ַ.
//       number:      �������ֽڳ���.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================

void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
{
	EA = 0;		//��ֹ�ж�
	ISP_CONTR = (ISP_EN + ISP_WAIT_FREQUENCY);	//���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	do
	{
		ISP_READ();							//������
		ISP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		ISP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		ISP_TRIG();							//����5AH������A5H��ISP/IAP�����Ĵ�����ÿ�ζ���Ҫ���
											//����A5H��ISP/IAP������������������
											//CPU�ȴ�IAP��ɺ󣬲Ż����ִ�г���
		_nop_();
		*DataAddress = ISP_DATA;			//��������������
		EE_address++;
		DataAddress++;
	}while(--number);

	DisableEEPROM();
	EA = 1;		//���������ж�
}


/******************** ������������ *****************/
//========================================================================
// ����: void EEPROM_SectorErase(u16 EE_address)
// ����: ��ָ����ַ��EEPROM��������.
// ����: EE_address:  Ҫ����������EEPROM�ĵ�ַ.
// ����: non.
// �汾: V1.0, 2013-5-10
//========================================================================
void EEPROM_SectorErase(u16 EE_address)
{
	EA = 0;		//��ֹ�ж�
											//ֻ������������û���ֽڲ�����512�ֽ�/������
											//����������һ���ֽڵ�ַ����������ַ��
	ISP_ADDRH = EE_address / 256;			//��������ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
	ISP_ADDRL = EE_address % 256;			//��������ַ���ֽ�
	ISP_CONTR = (ISP_EN + ISP_WAIT_FREQUENCY);	//���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	ISP_ERASE();							//��������������
	ISP_TRIG();
	_nop_();
	DisableEEPROM();
	EA = 1;		//���������ж�
}

//========================================================================
// ����: void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
// ����: �ѻ����n���ֽ�д��ָ���׵�ַ��EEPROM.
// ����: EE_address:  д��EEPROM���׵�ַ.
//       DataAddress: д��Դ���ݵĻ�����׵�ַ.
//       number:      д����ֽڳ���.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
{
	EA = 0;		//��ֹ�ж�

	ISP_CONTR = (ISP_EN + ISP_WAIT_FREQUENCY);	//���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	do
	{
		ISP_WRITE();						//д����
		ISP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		ISP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		ISP_DATA  = *DataAddress;			//�����ݵ�ISP_DATA��ֻ�����ݸı�ʱ����������
		ISP_TRIG();
		_nop_();
		EE_address++;
		DataAddress++;
	}while(--number);

	DisableEEPROM();
	EA = 1;		//���������ж�
}

/*
u8 EEPROM_read_Compare(u16 EE_address,u8 *DataAddress,u16 number)
{
	u8	j;
	j = 0;
	EA = 0;		//��ֹ�ж�
	ISP_CONTR = (ISP_EN + ISP_WAIT_FREQUENCY);	//���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	ISP_READ();									//���ֽڶ���������ı�ʱ����������������
	do
	{
		ISP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		ISP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		ISP_TRIG();							//����5AH������A5H��ISP/IAP�����Ĵ�����ÿ�ζ���Ҫ���
											//����A5H��ISP/IAP������������������
											//CPU�ȴ�IAP��ɺ󣬲Ż����ִ�г���
		_nop_();
		if(*DataAddress != ISP_DATA)		//������������Դ���ݱȽ�
		{
			j = 1;
			break;
		}
		EE_address++;
		DataAddress++;
	}while(--number);

	DisableEEPROM();
	EA = 1;		//���������ж�
	return j;
}
*/