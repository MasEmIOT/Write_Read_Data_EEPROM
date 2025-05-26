#include"rcc.h"
#include"gpio.h"
#include"timer.h"
#include"uart.h"
#include"eeprom.h"

int main(void)
{
	
	Clock_HSE_Init(); 
	TIM1_Init(); 
	UART_Init();
	Enable_Init();
	//Write_Number_EEPROM(101,2);
	//(1);
	//IO_Input_Init();
	//Read_Data(15);
	//int a = Read_Number_EEPROM();
	//char arr[10];
	//sprintf(arr,"%d",a);
	//Write_Character_EEPROM('C',10);
	//Read_Data(10);
	while(1)
	{
		//UART_Send_MultiByte(arr,strlen(arr));
		//TIM1_delay(10000);
		UART_Write_EEPROM();
		UART_Read_EEPROM();
	}
}
