#include"gpio.h"
#include<stdint.h>
#include"rcc.h"
void IO_Output_Init()
{
	uint32_t* RCC_APB2ENR = (uint32_t*) (RCC_BASE_ADDRESS + 0x18);
	*RCC_APB2ENR |= (1<<2) | (1<<3); //IOA,B Enable
	uint32_t* GPIOA_CRH = (uint32_t*) (GPIOA_BASE_ADDRESS + 0x04);
	*GPIOA_CRH &= ~(0xfff);
	*GPIOA_CRH |= (0x111); //PA8,9,10
	uint32_t* GPIOB_CRL = (uint32_t*) (GPIOB_BASE_ADDRESS + 0x00);
	*GPIOB_CRL &= ~(0xff); 
	*GPIOB_CRL |= (0x11); // PB1, PB0
	uint32_t* GPIOB_CRH = (uint32_t*) (GPIOB_BASE_ADDRESS + 0x04);
	*GPIOB_CRH &= ~(0xff000f00);
	*GPIOB_CRH |= (0x11000100 << 0); //PB10,14,15
}

void IO_Input_Init()
{
	uint32_t* RCC_APB2ENR = (uint32_t*) (RCC_BASE_ADDRESS + 0x18);
	*RCC_APB2ENR |= (1<<2) | (1<<3); //IOA,B Enable
	uint32_t* GPIOA_CRH = (uint32_t*) (GPIOA_BASE_ADDRESS + 0x04);
	*GPIOA_CRH &= ~(0xfff);
	*GPIOA_CRH |= (0x444); //PA8,9,10
	uint32_t* GPIOB_CRL = (uint32_t*) (GPIOB_BASE_ADDRESS + 0x00);
	*GPIOB_CRL &= ~(0xff); 
	*GPIOB_CRL |= (0x44); // PB1, PB0
	uint32_t* GPIOB_CRH = (uint32_t*) (GPIOB_BASE_ADDRESS + 0x04);
	*GPIOB_CRH &= ~(0xff000f00);
	*GPIOB_CRH |= (0x44000400 << 0); //PB10,14,15
}
void Address_Init()
{
	uint32_t* RCC_APB2ENR = (uint32_t*) (RCC_BASE_ADDRESS + 0x18);
	*RCC_APB2ENR |= (1<<2) | (1<<3); //IOA,B Enable
	uint32_t* GPIOA_CRL = (uint32_t*) (GPIOA_BASE_ADDRESS + 0x00);
	*GPIOA_CRL &= ~(0xffff00ff); 
	*GPIOA_CRL |= (0x11110011); //PA0,1,4,5,6,7 Output push pull
	uint32_t* GPIOB_CRH = (uint32_t*) (GPIOB_BASE_ADDRESS + 0x04);
	*GPIOB_CRH &= ~(0xfff << 12);
	*GPIOB_CRH |= (0x111 << 12); //PB11,12,13 Output push pull
}

void Enable_Init()
{
	uint32_t* RCC_APB2ENR = (uint32_t*) (RCC_BASE_ADDRESS + 0x18);
	*RCC_APB2ENR |= (1<<2); //IOA Enable
	uint32_t* GPIOA_CRH = (uint32_t*) (GPIOA_BASE_ADDRESS + 0x04);
	*GPIOA_CRH &= ~(0xf00ff << 12);
	*GPIOA_CRH |= (0x10011 << 12); //PA11,12,15
}
void GPIO_Write_Pin(GPIO_Port port, unsigned short pin, unsigned short state)
{
	if(port == GPIOA)
	{
		uint32_t* GPIOA_ODR = (uint32_t*) (GPIOA_BASE_ADDRESS + 0x0C);
		if(state == 1)
		*GPIOA_ODR |= (1<<pin);
		else
			*GPIOA_ODR &= ~(1<<pin);
	}
	else if(port == GPIOB)
	{
		uint32_t* GPIOB_ODR = (uint32_t*) (GPIOB_BASE_ADDRESS + 0x0C);
		if(state == 1)
			*GPIOB_ODR |= (1<<pin);
		else
			*GPIOB_ODR &= ~(1<<pin);
	}
}	


int GPIO_Read_Pin(GPIO_Port port, unsigned short pin)
{
	if(port == GPIOA)
		{
			uint32_t* GPIOA_IDR = (uint32_t*) (GPIOA_BASE_ADDRESS + 0x08);
			return ((*GPIOA_IDR>>pin) & (1));
		}
	else if(port == GPIOB)
		{
			uint32_t* GPIOB_IDR = (uint32_t*) (GPIOB_BASE_ADDRESS + 0x08);
			return ((*GPIOB_IDR>>pin) & (1));
		}
}
