
#include"eeprom.h"
#include"rcc.h"
#include"gpio.h"
#include"timer.h"
#include<string.h>
uint32_t end_address[128];
int _numberread = 0;
char _characterread;
void Address_EEPROM(uint32_t address)
 {
	 Address_Init();
	 GPIO_Write_Pin(GPIOA,0,(address>>0)& (1));
	 GPIO_Write_Pin(GPIOA,1,(address>>1)& (1));
	 GPIO_Write_Pin(GPIOB,12,(address>>2)& (1));
	 GPIO_Write_Pin(GPIOB,13,(address>>3)& (1));
	 GPIO_Write_Pin(GPIOA,4,(address>>4)& (1));
	 GPIO_Write_Pin(GPIOA,5,(address>>5)& (1));
	 GPIO_Write_Pin(GPIOA,6,(address>>6)& (1));
	 GPIO_Write_Pin(GPIOA,7,(address>>7)& (1));
	 GPIO_Write_Pin(GPIOB,11,(address>>8)& (1));
 }
 
 
 void Write_Number_EEPROM(uint8_t num, uint32_t address)
 {
	 IO_Output_Init();
	 GPIO_Write_Pin(GPIOB,10,(num>>0)& (1));
	 GPIO_Write_Pin(GPIOB,1,(num>>1)& (1));
	 GPIO_Write_Pin(GPIOB,0,(num>>2)& (1));
	 GPIO_Write_Pin(GPIOA,8,(num>>3)& (1));
	 GPIO_Write_Pin(GPIOB,15,(num>>4)& (1));
	 GPIO_Write_Pin(GPIOB,14,(num>>5)& (1));
	 GPIO_Write_Pin(GPIOA,9,(num>>6)& (1));
	 GPIO_Write_Pin(GPIOA,10,(num>>7)& (1));
	 Address_EEPROM(address);
	 TIM1_delay(10);
	 GPIO_Write_Pin(GPIOA,11,1); //OE High
	 GPIO_Write_Pin(GPIOA,12,1); //WE High
	 TIM1_delay(10); //1000us
	 GPIO_Write_Pin(GPIOA,12,0); //WE Low
	 TIM1_delay(5);
   GPIO_Write_Pin(GPIOA,12,1); 
	 TIM1_delay(10);
 }
void Write_Character_EEPROM(char c, uint32_t address)
{
	IO_Output_Init();
	GPIO_Write_Pin(GPIOB,10,(c>>0)& (1));
	 GPIO_Write_Pin(GPIOB,1,(c>>1)& (1));
	 GPIO_Write_Pin(GPIOB,0,(c>>2)& (1));
	 GPIO_Write_Pin(GPIOA,8,(c>>3)& (1));
	 GPIO_Write_Pin(GPIOB,15,(c>>4)& (1));
	 GPIO_Write_Pin(GPIOB,14,(c>>5)& (1));
	 GPIO_Write_Pin(GPIOA,9,(c>>6)& (1));
	 GPIO_Write_Pin(GPIOA,10,(c>>7)& (1));
	 Address_EEPROM(address);
	 TIM1_delay(10);
	 GPIO_Write_Pin(GPIOA,11,1); //OE High
	 GPIO_Write_Pin(GPIOA,12,1); //WE High
	 TIM1_delay(10); //1000us
	 GPIO_Write_Pin(GPIOA,12,0); //WE Low
	 TIM1_delay(5);
   GPIO_Write_Pin(GPIOA,12,1); 
	 TIM1_delay(10);
}
void Write_String_EEPROM(char* str, uint32_t start_address)
{
	IO_Output_Init();
	for(int i = 0; i < strlen(str); i++)
	{
		Write_Character_EEPROM(str[i], start_address);
		start_address++;
	}
	end_address[start_address] = start_address;
}
int Read_Number_EEPROM()
{
	_numberread = (GPIO_Read_Pin(GPIOB,10) & 1)*1 + (GPIO_Read_Pin(GPIOB,1) & 1)*2 + ((GPIO_Read_Pin(GPIOB,0) & 1)*4) + ((GPIO_Read_Pin(GPIOA,8) & 1)*8) + ((GPIO_Read_Pin(GPIOB,15) & 1)*16) + ((GPIO_Read_Pin(GPIOB,14) & 1)*32) + ((GPIO_Read_Pin(GPIOA,9) & 1)*64) + ((GPIO_Read_Pin(GPIOA,10) & 1)*128);
	return _numberread;
}
char Read_Character_EEPROM()
{
	IO_Input_Init();
	_characterread = (GPIO_Read_Pin(GPIOB,10) & 1)*1 + (GPIO_Read_Pin(GPIOB,1) & 1)*2 + ((GPIO_Read_Pin(GPIOB,0) & 1)*4) + ((GPIO_Read_Pin(GPIOA,8) & 1)*8) + ((GPIO_Read_Pin(GPIOB,15) & 1)*16) + ((GPIO_Read_Pin(GPIOB,14) & 1)*32) + ((GPIO_Read_Pin(GPIOA,9) & 1)*64) + ((GPIO_Read_Pin(GPIOA,10) & 1)*128);
	return _characterread;
}
void Read_Data(uint32_t add)
{
	IO_Input_Init();
	Address_EEPROM(add);
	GPIO_Write_Pin(GPIOA,12,1); //WE High
	GPIO_Write_Pin(GPIOA,11,0); //OE LOW
}
