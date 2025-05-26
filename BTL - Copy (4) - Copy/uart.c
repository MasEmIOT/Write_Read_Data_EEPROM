#include<stdint.h>
#include"rcc.h"
#include"gpio.h"
#include"uart.h"
#include<string.h>
#include"eeprom.h"
#include<stdlib.h>
#include<stdio.h>
char rx_buf[128]; // vung nho kich thuoc 8 byte
char _address[128];
char _number[128];
int rx_index = 0;
int counter = 0;

char readnumber[10];


char _string[128];
char _character;
char _stringread[50];
char _length[128];
void UART_Init()
{
	//AFIO
	uint32_t* RCC_APB2ENR = (uint32_t*) (RCC_BASE_ADDRESS + 0x18);
	uint32_t* RCC_APB1ENR = (uint32_t*) (RCC_BASE_ADDRESS + 0x1C);
	*RCC_APB2ENR |= (1<<2); // enable gpioA 
	*RCC_APB1ENR |= (1<<17); //enable uart2
	uint32_t* GPIOA_CRL = (uint32_t*) (GPIOA_BASE_ADDRESS + 0x00);
	*GPIOA_CRL &= ~(0xff << 8);
	*GPIOA_CRL |= (0x49 << 8); // enable PA2 as alternative function output, PA3 input floating
	
	uint32_t* AFIO_MAPR = (uint32_t*) (AFIO_BASE_ADDRESS + 0x04);
	*AFIO_MAPR &= ~(1<<3);
	//UART config
	uint32_t* UART2_BRR = (uint32_t*) (UART_BASE_ADDRESS + 0x08);
	//*UART2_BRR = (468<<4) | (12); //baurate 4800 
	*UART2_BRR = (234<<4) | (6); //baudrate 9600
	uint32_t* UART2_CR1 = (uint32_t*) (UART_BASE_ADDRESS + 0x0C);
	*UART2_CR1 |= (1<<2) | (1<<3) | (1<<13) | (1<<5); // enable UE,RE,TE

	//NVIC config
	uint32_t* ISER1 = (uint32_t*) 0xe000e104; // 0xe000e100 - NVIC_ISER0
	*ISER1 |= (1<<(38-32)); //UART2 Interrupt at position 38, ISER1 begin at position 32
	
}
 void UART_Send_Byte(char data)
 {
	 uint32_t* UART_DR = (uint32_t*) (UART_BASE_ADDRESS + 0x04);
	 uint32_t* UART_SR = (uint32_t*) (UART_BASE_ADDRESS + 0x00);
	 while(!(*UART_SR & (0x0080))); // check TxE (TX buffer register), if empty => write  
	 *UART_DR = data;
	 while(((*UART_SR >> 6) & 1) != 0 );
 }
 void UART_Send_MultiByte(char* arr, int size)
 {
	 for(int i = 0; i < size; i++)
	 {
		UART_Send_Byte(arr[i]);
	 }
	 //UART_Send_Byte('\n');
 }
 void USART2_IRQHandler()
 {
	 uint32_t* UART_DR = (uint32_t*) (UART_BASE_ADDRESS + 0x04);
	 rx_buf[rx_index] = *UART_DR;
	 //UART_Send_Byte(rx_buf[rx_index]);
	 rx_index++;
	 //counter++;
	 //UART_Write_EEPROM();
	 //UART_Read_EEPROM();
 }

 char UART_Receive_Byte()
 {
	 uint32_t* UART_DR = (uint32_t*) (UART_BASE_ADDRESS + 0x04);
	 uint32_t* UART_SR = (uint32_t*) (UART_BASE_ADDRESS + 0x00);
	 while(!(*UART_SR & (0x0020)));
	 return *UART_DR;
	 
 }
 
//------------------------------------------------------------------------



 
void UART_Write_EEPROM()
{
	if(strcmp(rx_buf,"Write number") == 0)
	{
		//IO_Output_Init();
		UART_Send_Byte('\n');
		UART_Send_MultiByte("Write number: ",14);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='.');
		//strcpy(_number,rx_buf[strlen(rx_buf)-1]);
		//UART_Send_MultiByte(_number,strlen(_number));
		for(int i = 0; i<rx_index-1;i++)
		{
			_number[i] = rx_buf[i];
		}
		UART_Send_MultiByte(_number,strlen(_number));
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='*');
		//strcpy(_address,rx_buf);
		//UART_Send_MultiByte(_address,strlen(_address));
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		UART_Send_MultiByte(_address,strlen(_address));
		Write_Number_EEPROM(atoi(_number),atoi(_address));	
		UART_Send_Byte('\n');
		rx_index = 0;
		rx_buf[rx_index] = '\0';
	}
	
	if(strcmp(rx_buf,"Write character") == 0)
	{
		//IO_Output_Init();
		UART_Send_Byte('\n');
		UART_Send_MultiByte("Write character: ",17);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_index==0);
		_character = rx_buf[0];
		UART_Send_Byte(_character);
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='*');
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		UART_Send_MultiByte(_address,strlen(_address));
		Write_Character_EEPROM(_character,atoi(_address));	
		UART_Send_Byte('\n');
		rx_index = 0;
		rx_buf[rx_index] = '\0';
	}
	
	if(strcmp(rx_buf,"Write string") == 0)
	{
		//IO_Output_Init();
		UART_Send_Byte('\n');
		UART_Send_MultiByte("Write string: ",14);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='.');
		for(int i = 0; i<rx_index;i++)
		{
			_string[i] = rx_buf[i];
		}
		UART_Send_MultiByte(_string,strlen(_string)-1);
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='*');
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		UART_Send_MultiByte(_address,strlen(_address));
		Write_String_EEPROM(_string,atoi(_address));	
		UART_Send_Byte('\n');
		rx_index = 0;
		rx_buf[rx_index] = '\0';
	}
}
void UART_Read_EEPROM()
{
	if(strcmp(rx_buf,"Read number") == 0)
	{
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='*');
		//UART_Send_MultiByte(_address,strlen(_address));
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		UART_Send_MultiByte(_address,strlen(_address));
		Read_Data(atoi(_address));

		UART_Send_Byte('\n');
		rx_index = 0;
		int a = Read_Number_EEPROM();
		sprintf(readnumber,"%d",a);
		UART_Send_MultiByte(readnumber,strlen(readnumber));
		UART_Send_Byte('\n');
		//while(rx_index == 0);
	}
	
	if(strcmp(rx_buf,"Read character") == 0)
	{
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='*');
		//UART_Send_MultiByte(_address,strlen(_address));
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		UART_Send_MultiByte(_address,strlen(_address));
		Read_Data(atoi(_address));

		UART_Send_Byte('\n');
		rx_index = 0;
		_character = Read_Character_EEPROM();
		UART_Send_Byte(_character);
		UART_Send_Byte('\n');
		//while(rx_index == 0);
	}
	
		if(strcmp(rx_buf,"Read string") == 0)
	{
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='*');
		//TIM1_delay(3000);
		//UART_Send_MultiByte(_address,strlen(_address));
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		UART_Send_MultiByte(_address,strlen(_address));
		
		UART_Send_Byte('\n');
		
		UART_Send_MultiByte("Length: ",8);
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='.');
		for(int i = 0; i<rx_index-1;i++)
		{
			_length[i] = rx_buf[i];
		}
		UART_Send_MultiByte(_length,strlen(_length));
		rx_index = 0;
		UART_Send_Byte('\n');
    //int length = atoi(_length);
	
		for(int i = 0; i < atoi(_length); i++)
		{
			//Read_Data(atoi(_address)+i);
			Read_Data(atoi(_address)+i);
			_character = Read_Character_EEPROM();
			_stringread[i] = _character;
			//UART_Send_Byte(_stringread[i]);
		}
		UART_Send_MultiByte(_stringread,strlen(_stringread));
		UART_Send_Byte('\n');
		//while(rx_index == 0);
	}
	
}
