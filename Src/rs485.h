                                                                                          


	#define IRREGULAR_CMD_NUM	67	


	struct TRead_Write_Attribute{
		uint cmd_pc;
		uchar cmd_spi_read;
		uchar cmd_spi_write;
		uchar max_len;
		uchar oper_len;
		uchar offset;
		uchar property;		
	};	 
//.0=1 read and write command .1=1 calibration command  .2=1 soft modify .3=1(sub,=0 add).4=1(phase soft modify)
//.5=1 inverse data sequence
	
	uchar Get_Iregular_Cmd_Atrib(uint);	











