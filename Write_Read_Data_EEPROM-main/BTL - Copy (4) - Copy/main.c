#include"rcc.h"
#include"gpio.h"
#include"timer.h"
#include"uart.h"
#include"eeprom.h"
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"lcd.h"
#include"i2c.h"
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


 char UART_Receive_Byte()
 {
	 uint32_t* UART_DR = (uint32_t*) (UART_BASE_ADDRESS + 0x04);
	 uint32_t* UART_SR = (uint32_t*) (UART_BASE_ADDRESS + 0x00);
	 while(!(*UART_SR & (0x0020)));
	 return *UART_DR;
	 
 }
 
//------------------------------------------------------------------------
 void USART2_IRQHandler()
 {
	 uint32_t* UART_DR = (uint32_t*) (UART_BASE_ADDRESS + 0x04);
	 rx_buf[rx_index] = *UART_DR;
	 //UART_Send_Byte(rx_buf[rx_index]);
	 rx_index++;
	 //counter++;

}


 
void UART_ReadWrite_EEPROM()
{
	if(strcmp(rx_buf,"Write number") == 0)
	{
		//IO_Output_Init();
		UART_Send_Byte('\n');
		UART_Send_MultiByte("Write number: ",14);
		LCD_Put_Cur(0,0);
		LCD_Sent_String("Write number: ");
		rx_index = 0;
		strcpy(rx_buf,"");
		TIM1_delay(10000);
		while(rx_buf[rx_index]!='.');
		//strcpy(_number,rx_buf[strlen(rx_buf)-1]);
		//UART_Send_MultiByte(_number,strlen(_number));
		for(int i = 0; i<rx_index-1;i++)
		{
			_number[i] = rx_buf[i];
		}
		LCD_Sent_String(_number);
		LCD_Put_Cur(1,0);
		UART_Send_MultiByte(_number,strlen(_number));
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		LCD_Sent_String("At address: ");
		rx_index = 0;
		strcpy(rx_buf,"");
		TIM1_delay(10000);
		while(rx_buf[rx_index]!='*');
		//strcpy(_address,rx_buf);
		//UART_Send_MultiByte(_address,strlen(_address));
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		LCD_Sent_String(_address);
		UART_Send_MultiByte(_address,strlen(_address));
		Write_Number_EEPROM(atoi(_number),atoi(_address));	
		UART_Send_Byte('\n');
		rx_index = 0;
		strcpy(rx_buf,"");
		//rx_buf[rx_index] = '\0';
	}
	
	if(strcmp(rx_buf,"Write character") == 0)
	{
		//IO_Output_Init();
		UART_Send_Byte('\n');
		UART_Send_MultiByte("Write character: ",17);
		LCD_Put_Cur(0,0);
		LCD_Sent_String("Write character: ");
		rx_index = 0;
		strcpy(rx_buf,"");
		TIM1_delay(10000);
		while(rx_index==0);
		_character = rx_buf[0];
		UART_Send_Byte(_character);
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		LCD_Put_Cur(1,0);
		LCD_Sent_String("At address: ");
		rx_index = 0;
		strcpy(rx_buf,"");
		TIM1_delay(10000);
		while(rx_buf[rx_index]!='*');
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		LCD_Sent_String(_address);
		UART_Send_MultiByte(_address,strlen(_address));
		Write_Character_EEPROM(_character,atoi(_address));	
		UART_Send_Byte('\n');
		rx_index = 0;
		strcpy(rx_buf,"");
		//rx_buf[rx_index] = '\0';
	}
	
	if(strcmp(rx_buf,"Write string") == 0)
	{
		//IO_Output_Init();
		UART_Send_Byte('\n');
		UART_Send_MultiByte("Write string: ",14);
		LCD_Put_Cur(0,0);
		LCD_Sent_String("Write string: ");
		rx_index = 0;
		strcpy(rx_buf,"");
		TIM1_delay(10000);
		while(rx_buf[rx_index]!='.');
		for(int i = 0; i<rx_index;i++)
		{
			_string[i] = rx_buf[i];
		}
		LCD_Put_Cur(1,0);
		LCD_Sent_String(_string);
		UART_Send_MultiByte(_string,strlen(_string)-1);
		LCD_Put_Cur(0,0);
		LCD_Sent_String("At address: ");
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		rx_index = 0;
		strcpy(rx_buf,"");
		TIM1_delay(10000);
		while(rx_buf[rx_index]!='*');
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		LCD_Sent_String(_address);
		UART_Send_MultiByte(_address,strlen(_address));
		Write_String_EEPROM(_string,atoi(_address));	
		UART_Send_Byte('\n');
		rx_index = 0;
		strcpy(rx_buf,"");
	}
	
	if(strcmp(rx_buf,"Read number") == 0)
	{
		UART_Send_Byte('\n');
		UART_Send_MultiByte("Read number",11);
		LCD_Put_Cur(0,0);
		LCD_Sent_String("Read number");
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		LCD_Put_Cur(1,0);
		LCD_Sent_String("At address: ");
		rx_index = 0;
		strcpy(rx_buf,"");
		TIM1_delay(10000);
		while(rx_buf[rx_index]!='*');
		//UART_Send_MultiByte(_address,strlen(_address));
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		LCD_Sent_String(_address);
		UART_Send_MultiByte(_address,strlen(_address));
		Read_Data(atoi(_address));

		UART_Send_Byte('\n');
		rx_index = 0;
		int a = Read_Number_EEPROM();
		sprintf(readnumber,"%d",a);
		LCD_Clear();
		LCD_Put_Cur(0,0);
		LCD_Sent_String(readnumber);
		UART_Send_MultiByte(readnumber,strlen(readnumber));
		UART_Send_Byte('\n');
		strcpy(rx_buf,"");
		//while(rx_index == 0);
	}
	
	if(strcmp(rx_buf,"Read character") == 0)
	{
		UART_Send_Byte('\n');
		UART_Send_MultiByte("Read character",14);
		LCD_Put_Cur(0,0);
		LCD_Sent_String("Read character");
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		LCD_Put_Cur(1,0);
		LCD_Sent_String("At address: ");
		rx_index = 0;
		strcpy(rx_buf,"");
		TIM1_delay(10000);
		while(rx_buf[rx_index]!='*');
		//UART_Send_MultiByte(_address,strlen(_address));
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		LCD_Sent_String(_address);
		UART_Send_MultiByte(_address,strlen(_address));
		Read_Data(atoi(_address));

		UART_Send_Byte('\n');
		rx_index = 0;
		_character = Read_Character_EEPROM();
		LCD_Clear();
		LCD_Put_Cur(0,0);
		LCD_Sent_Data(_character);
		UART_Send_Byte(_character);
		UART_Send_Byte('\n');
		strcpy(rx_buf,"");
		//while(rx_index == 0);
	}
	
	if(strcmp(rx_buf,"Read string") == 0)
	{
		UART_Send_Byte('\n');
		UART_Send_MultiByte("Read string",11);
		LCD_Put_Cur(0,0);
		LCD_Sent_String("Read string");
		UART_Send_Byte('\n');
		UART_Send_MultiByte("At address: ",12);
		LCD_Put_Cur(1,0);
		LCD_Sent_String("At address: ");
		rx_index = 0;
		strcpy(rx_buf,"");
		TIM1_delay(10000);
		while(rx_buf[rx_index]!='*');
		//TIM1_delay(3000);
		//UART_Send_MultiByte(_address,strlen(_address));
		for(int i = 0; i<rx_index-1;i++)
		{
			_address[i] = rx_buf[i];
		}
		LCD_Sent_String(_address);
		UART_Send_MultiByte(_address,strlen(_address));
		
		UART_Send_Byte('\n');
		
		UART_Send_MultiByte("Length: ",8);
		LCD_Clear();
		LCD_Put_Cur(0,0);
		LCD_Sent_String("Length: ");
		rx_index = 0;
		strcpy(rx_buf,"");
		while(rx_buf[rx_index]!='.');
		for(int i = 0; i<rx_index-1;i++)
		{
			_length[i] = rx_buf[i];
		}
		LCD_Clear();
		LCD_Sent_String(_length);
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
		LCD_Put_Cur(1,0);
		LCD_Sent_String(_stringread);
		UART_Send_MultiByte(_stringread,strlen(_stringread));
		UART_Send_Byte('\n');
		strcpy(rx_buf,"");
		//while(rx_index == 0);
	}
	
}
int main(void)
{
	
	Clock_HSE_Init(); 
	TIM1_Init(); 
	UART_Init();
	Enable_Init();
	I2C_Init();
	//IO_Output_Init();
  //Write_Number_EEPROM(30,6);
	//(1);
	//IO_Input_Init();
	//Read_Data(15);
	//int a = Read_Number_EEPROM();
	//char arr[10];
	//sprintf(arr,"%d",a);
	//Write_Character_EEPROM('C',10);
	//Read_Data(6);
	LCD_Init();
	LCD_Put_Cur(0,0);
  LCD_Sent_String("WELCOME");
	while(1)
	{
		//UART_Send_MultiByte("hello world",11);
		//UART_Send_MultiByte(arr,strlen(arr));
		//GPIO_Write_Pin(GPIOA,9,1);
    //TIM1_delay(10000);
		//GPIO_Write_Pin(GPIOA,9,0);
		//TIM1_delay(10000);
		//UART_Write_EEPROM();
		//UART_Read_EEPROM();
		UART_ReadWrite_EEPROM();
	}
}
