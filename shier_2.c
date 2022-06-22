#include<reg51.h>
#include<iic.c>
#include<intrins.h>
sbit R1=P3^0;//??????(????????,????????????,??????)
sbit R2=P3^1;//??????
sbit R3=P3^2;//??????
sbit R4=P3^3;//??????



unsigned int count_f = 0;
unsigned char count_t = 0;
unsigned int dat_f = 0;
unsigned int dat_term = 0;
uchar S4_mode=9;
uchar S5_mode=4;
uchar i=0;
uchar j=0;
int voltage_rd1;
int dat_rd1;
int voltage_rb2;
int dat_rb2;
int voltage[];
int fre[];
void led_0();

void Scan_S4();
void Scan_S5();
void Init()
{
	R1=R2=R3=R4=1;
}


void Init_Timer()
{
	TH0 = 0xff;        
	TL0 = 0xff;
	
	TH1 = (65536 - 50000) / 256;        
	TL1 = (65536 - 50000) % 256;
	TMOD = 0x16;     
  	ET0 = 1;
  	ET1 = 1;
	EA = 1;
	TR0 = 1;
	TR1 = 1;
}
 
void Service_T0() interrupt 1
{
	count_f++;
}
 
void Service_T1() interrupt 3
{
  TH1 = (65536 - 50000) / 256;        
	TL1 = (65536 - 50000) % 256;
	count_t++;
	if(count_t == 20)
	{
		dat_f = count_f;
		count_f = 0;
		count_t = 0;
	}
}
void write_eeprom(unsigned char add,unsigned char date)//??eeprom??,?????,?????
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_SendByte(date);
	IIC_WaitAck();
	IIC_Stop();
}
unsigned char read_eeprom(unsigned char add)//?eeprom???,???????
{
	unsigned char temp;
	EA = 0;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();

	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_SendAck(1);
	IIC_WaitAck();
	IIC_Stop();
	EA = 1;
	return temp;
}

void display_f()
{
	ledlight(2);
	shumaguan_zimu(0,5);
	if(dat_f>9999)
		shumaguan_shuzi(3,dat_f/1000);
	if(dat_f>999)
		shumaguan_shuzi(4,dat_f/1000%10);
	if(dat_f>99)
		shumaguan_shuzi(5,dat_f/100%10);
	if(dat_f>9)
		shumaguan_shuzi(6,dat_f/10%10);
		
		shumaguan_shuzi(7,dat_f%10);
	
}
void display_term()
{
	ledlight(3);
	shumaguan_zimu(0,10);
	dat_term=1000000/dat_f;
	if(dat_f>9999)
		shumaguan_shuzi(3,dat_term/1000);
	if(dat_f>999)
		shumaguan_shuzi(4,dat_term/1000%10);
	if(dat_f>99)
		shumaguan_shuzi(5,dat_term/100%10);
	if(dat_f>9)
		shumaguan_shuzi(6,dat_term/10%10);
		
		shumaguan_shuzi(7,dat_term%10);
}
void display_RD1()
{
	ledlight(4);
	IIC_Start();						
	IIC_SendByte(0x90); 	
	IIC_WaitAck();  	
	IIC_SendByte(0x01);
	IIC_WaitAck();  						
	IIC_Stop(); 						
	
	IIC_Start();											
	IIC_SendByte(0x91); 			
	IIC_WaitAck(); 				
	dat_rd1 = IIC_RecByte();	 			
	IIC_WaitAck(); 						
	IIC_Stop(); 						

	
	voltage_rd1=dat_rd1*100/51;
	shumaguan_zimu(0,9);
	shumaguan_shuzi(1,10);
	shumaguan_shuzi(2,1);
	shumaguan_shuzi(6,voltage_rd1%100/10);
	shumaguan_shuzi(7,voltage_rd1%10);
	P2=(P2&0X1f)|0xC0;P0=weizhi[5];
	P2=(P2&0X1f)|0xFF;P0=shuzi[voltage_rd1/100]+0x80;
}
void display_Rb2()
{
	if(voltage_rb2>voltage[0])
	{
	P2=(P2&0X1f)|0x80;
	P0=0xee;
	}
	else ledlight(4);
	IIC_Start();						
	IIC_SendByte(0x90); 	
	IIC_WaitAck();  	
	IIC_SendByte(0x03);
	IIC_WaitAck();  						
	IIC_Stop(); 						
	
	IIC_Start();											
	IIC_SendByte(0x91); 			
	IIC_WaitAck(); 				
	dat_rb2 = IIC_RecByte();	 			
	IIC_WaitAck(); 						
	IIC_Stop(); 						

	
	voltage_rb2=dat_rb2*100/51;
	shumaguan_zimu(0,9);
	shumaguan_shuzi(1,10);
	shumaguan_shuzi(2,3);
	shumaguan_shuzi(6,voltage_rb2%100/10);
	shumaguan_shuzi(7,voltage_rb2%10);
	P2=(P2&0X1f)|0xC0;P0=weizhi[5];
	P2=(P2&0X1f)|0xFF;P0=shuzi[voltage_rb2/100]+0x80;
}
void Scan_S4()
{
	if(R4==0)
	{
		delay_ms(60);
		if(R4==0)
		{
			
			while(1)
			{
					
				display_term();
			
				if(R4==0)
				{
					delay_ms(60);
					if(R4==0)
					{
						
						while(1)
						{
						
							display_RD1();
							Scan_S5();
							
							if(R4==0) 
							{
								delay_ms(60);
								if(R4==0)
								{
								
									while(1)
									{
										display_f();
									
										if(R4==0)
										{
											delay_ms(60);
											if(R4==0)
											{
												
												while(1)
												{
													Scan_S4();
												}
											}
										}
										while(R4==0);
									}
									while(R4==0);
								}
							}
							while(R4==0);
						}
						
						while(R4==0);
					}
				}
				while(R4==0);
			}
			while(R4==0);
		}
	}
}
void Scan_S5()
{
	
	
	if(R3==0)
	{
		delay_ms(10);
	if(R3==0)
	{
		S5_mode++;
		while(R3==0);
	}
	}
	if(S5_mode%2==0)
	{
		display_RD1();
	}
	if(S5_mode%2==1)
	{
		while(1)
		{
			display_Rb2();
			if(R3==0)
			{
				delay_ms(10);
				if(R3==0)
				{
					break;
					while(R3==0);
				}
			}
		}
	}
}
void Scan_S6()
{
	
	if(R2==0)
	{
		delay_ms(50);
		if(R2==0)
		{
			voltage[i]=voltage_rb2;
			i++;
			dat_f=fre[j++];
			while(R2==0);
		}
}
}
void Scan_S7()
{
	int count=0;
	if(R1==0)
	{
		delay_ms(50);
		if(R1==0)
		{
			write_eeprom(0x03,dat_f);
		}
	}
	if(R1==0)
	{
		count++;
		if(count>500)
		{
			ledlight(8);
		
			if(R1==0)
			{
				count++;
				if(count>1000)
				{
					
					count=0;
				}
			}
		}
	}
}
void led_0()
{
	int voltage_c;
	voltage_c=read_eeprom(0x02);
	if(voltage_rb2>voltage_c)
	{
	
		
			P2=(P2&0X1f)|0x80;
			P0=0xee;
		
	}
	else ledlight(4);
}
void led_1()
{
	int dat_f_c;
	dat_f_c=read_eeprom(0x03);
	if(dat_f>dat_f_c) ledlight(1);
}
void main()
{
	allinit();
	Init_Timer();
	while(1)
	{
		Scan_S7();
		Scan_S6();
		Scan_S4();
		display_f();
	
	}
}
