

#include <reg51.h>
#include <main.h>
#include <rs485.h>

	

	//bit definition of property
	//.0=1 read and write command       .0=0 can't setup command 
	//.1=1 calibration command  			.1=0 not calibration command 
	//.2=1 soft modify						.2=0 not soft modify
	//.3=1 deuce as soft modifying 		.3=0 add as soft modifying
	//.4=1 phase of soft modifying		.4=0 not phase of soft modifying
  	//.5=1 inverse data sequence
  	
code struct TRead_Write_Attribute irregular_cmd_table[IRREGULAR_CMD_NUM]=
{
			{0x8011,0x60,0xc8,MAX_SPI_LEN,3,07,0x03},
			{0x8012,0x60,0xc9,MAX_SPI_LEN,3,31,0x03},
			{0x8013,0x60,0xca,MAX_SPI_LEN,3,55,0x03},
			
			{0x8021,0x60,0xc4,MAX_SPI_LEN,3,11,0x03},
			{0x8022,0x60,0xc5,MAX_SPI_LEN,3,35,0x03},
			{0x8023,0x60,0xc6,MAX_SPI_LEN,3,59,0x03},
			
			{0x8031,0x60,0xc0,MAX_SPI_LEN,3,15,0x03},
			{0x8032,0x60,0xc1,MAX_SPI_LEN,3,39,0x03},
			{0x8033,0x60,0xc2,MAX_SPI_LEN,3,63,0x03},
			
			{0x8041,0x60,0xd8,MAX_SPI_LEN,3,19,0x03}, 	//10
			{0x8042,0x60,0xd9,MAX_SPI_LEN,3,43,0x03},
			{0x8043,0x60,0xda,MAX_SPI_LEN,3,67,0x03},
			
			{0x8051,0x60,0xd4,MAX_SPI_LEN,3,23,0x03},
			{0x8052,0x60,0xd5,MAX_SPI_LEN,3,47,0x03},
			{0x8053,0x60,0xd6,MAX_SPI_LEN,3,71,0x03},
			
			{0x8061,0x60,0xd0,MAX_SPI_LEN,3,27,0x03},
			{0x8062,0x60,0xd1,MAX_SPI_LEN,3,51,0x03},
			{0x8063,0x60,0xd2,MAX_SPI_LEN,3,76,0x03},
			
			//active power modifying para.																				
			{0x8071,0x08,0xe0,1,1,118,0x05},
			{0x8072,0x08,0xe1,1,1,119,0x05},		//20
			{0x8073,0x08,0xe2,1,1,120,0x05},
			
			{0x8074,0x08,0xe0,1,1,118,0x0d},
			{0x8075,0x08,0xe1,1,1,119,0x0d},
			{0x8076,0x08,0xe2,1,1,120,0x0d},	
			
			//phase modifying para.
 			{0x8081,0x08,0xe8,1,1,124,0x15},
 			{0x8082,0x08,0xe9,1,1,125,0x15},
 			{0x8083,0x08,0xea,1,1,126,0x15},
 			
 			{0x8084,0x08,0xe8,1,1,124,0x1d},
 			{0x8085,0x08,0xe9,1,1,125,0x1d},
 			{0x8086,0x08,0xea,1,1,126,0x1d},		//30
 			
			//reactive power modifying para. 			
			{0x8091,0x08,0xe4,1,1,121,0x05},
			{0x8092,0x08,0xe5,1,1,122,0x05},
			{0x8093,0x08,0xe6,1,1,123,0x05},
			
			{0x8094,0x08,0xe4,1,1,121,0x0d},
			{0x8095,0x08,0xe5,1,1,122,0x0d},
			{0x8096,0x08,0xe6,1,1,123,0x0d},	
			
			//read frequency(A B C)
			{0xb601,0x08,0,4,4,72,0x20},
			{0xb602,0x08,0,4,4,76,0x20},
			{0xb603,0x08,0,4,4,80,0x20},	
			
			//read rms of valtage(A B C)			
			{0xb611,0x08,0,4,4,48,0x20},		//40
			{0xb612,0x08,0,4,4,52,0x20},
			{0xb613,0x08,0,4,4,56,0x20},	
			
			//read rms of current(A B C)			
			{0xb621,0x08,0,4,4,60,0x20},
			{0xb622,0x08,0,4,4,64,0x20},
			{0xb623,0x08,0,4,4,68,0x20},	
			
			//read instant active power(A B C)
			{0xb630,0x08,0,4,4,0,0x20},
 			{0xb631,0x08,0,4,4,4,0x20},
 			{0xb632,0x08,0,4,4,8,0x20},
 			{0xb633,0x08,0,4,4,12,0x20},	
 			
			//read instant reactive power(A B C)			 			
			{0xb640,0x08,0,4,4,16,0x20},  	//50
 			{0xb641,0x08,0,4,4,20,0x20},
 			{0xb642,0x08,0,4,4,24,0x20},
 			{0xb643,0x08,0,4,4,28,0x20},	
			
			//read power factor(A B C)
			{0xb650,0x08,0,4,4,84,0x20},
 			{0xb651,0x08,0,4,4,88,0x20},
 			{0xb652,0x08,0,4,4,92,0x20},
 			{0xb653,0x08,0,4,4,96,0x20},	
			
			//read real power (A B C)			 			
			{0xb680,0x08,0,4,4,32,0x20},
 			{0xb681,0x08,0,4,4,36,0x20},
 			{0xb682,0x08,0,4,4,40,0x20},		//60
 			{0xb683,0x08,0,4,4,44,0x20},	
	
			//meter pulse constant(active and reactive)									 			
 			{0xc030,0x08,0xb0,3,3,112,0x01},
			{0xc031,0x08,0xb1,3,3,115,0x01},				
			//meter voltage and current's range							 			
 			{0xe191,0x08,0x90,4,4,104,0x01},	
 			{0xe192,0x08,0xa0,4,4,108,0x01},		
			//read memory 					
 			{0xe195,0x60,0,MAX_SPI_LEN,MAX_SPI_LEN,0,0x00},
 			//read status
			{0xe196,0x08,0,4,4,100,0x20}
};	




							
	uchar Get_Iregular_Cmd_Atrib(uint cmd_type)
	{	
    	uchar max,min,mid;
	   	max=IRREGULAR_CMD_NUM;
	   	min=0;
	   	mid=IRREGULAR_CMD_NUM/2;
    			
	   	if(cmd_type==irregular_cmd_table[max-1].cmd_pc) 
			return (max-1);
    	if(cmd_type==irregular_cmd_table[min].cmd_pc) 
			return min;
	   	while(1)
		{	 	
    	   	if(cmd_type==irregular_cmd_table[mid].cmd_pc) return mid;
			else if(cmd_type>irregular_cmd_table[mid].cmd_pc)
			{
	         	min=mid;
	         	mid=(max+min)/2;
	      	}	
	      	else
	      	{
				max=mid;
				mid=(max+min)/2;
			}
			if((max-min)<=1)break;
		}
		if(cmd_type==irregular_cmd_table[max].cmd_pc) return max;
		else if(cmd_type==irregular_cmd_table[min].cmd_pc)return min;
		else return 0xff;
	}







	
	
 






























































































































