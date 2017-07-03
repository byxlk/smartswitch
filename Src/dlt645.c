

#include <reg51.h>
#include <main.h>
#include <dlt645.h>
#include <interrupt.h>

extern uchar rs485_buf[MAX_485_LEN];
extern uchar pointer_send,send_len;
extern uchar oper_len,send_offset,chk_sum;
extern bit flag_send_data;

	uchar Get_Chksum(uchar *ps,uchar len)
	{
		uchar i,j=0;
		for(i=0;i<len;i++)
			j+=ps[i];

		return j;		
	}
	
	
/*	
	void Get_Send_Frame_Package(uchar len)
	{
		uchar i,j=0;
		
		rs485_buf[8]|=0x80;
		rs485_buf[9]=len;
		for(i=0;i<len;i++)
			rs485_buf[10+i]+=0x33;
			
		for(i=0;i<len+10;i++)
			j+=rs485_buf[i];				
		rs485_buf[i++]=j;
		rs485_buf[i]=0x16;
	}
*/
	
/*	
	void Send_Frame_To_485(void)
	{
		send_len=rs485_buf[9]+12;			
		pointer_send=1;
		TI=0;
		TB8=Get_Odd_Even_Bit(rs485_buf[0]);

		SBUF=rs485_buf[0];		
		REN_TEN=HIGH;
	}				
*/	
    void Send_Byte_To_485(uchar byt)
    {
     ES=1;
     EA=1;
     ACC=byt;
     if(P)
      TB8=0;
     else
      TB8=1;
     SBUF=byt;
     chk_sum+=byt;
    /* while(TI==1)
      TI=0;
     while(TI==1)
      TI=0;*/
    }

	void Send_Frame_To_485(uchar len,uchar head)
	{
		//uchar temp_i;
        //flag_send_data=1;
		//ES=1;
		//ACC=rs485_buf[0];                           	
		//oper_len=len;    			//len of sending data(L)
	//	send_offset=head;			//offset of sending buffer(spi_buf)
        send_len=len;
	//	pointer_send=1;
		chk_sum=0x00;
		REN_TEN=LOW;
        Send_Byte_To_485(rs485_buf[0]);

     /*   for(temp_i=head;temp_i<len;temp_i++)
           {if(temp_i==11)
             Send_Byte_To_485(chk_sum);
            else
             Send_Byte_To_485(rs485_buf[temp_i]);
           }
/*		rs485_buf[8]|=0x80;	
		send_len=len;			
		TI=0;
		if(!P)
         TB8=0;
        else
         TB8=1;//TB8=Get_Odd_Even_Bit(rs485_buf[0]);
		chk_sum=rs485_buf[0];*/
				
	//	SBUF=rs485_buf[0];		
	}
	
	
 









































































































































