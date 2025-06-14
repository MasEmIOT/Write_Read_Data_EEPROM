#include<stdint.h>
#include"rcc.h"
void Clock_HSE_Init()
{
	uint32_t* CR = (uint32_t*) (RCC_BASE_ADDRESS + 0x00);
	uint32_t* CFGR = (uint32_t*) (RCC_BASE_ADDRESS + 0x04);
	*CR |= (1<<16); //enable HSE
	while ((*CR << 17) & 1 == 0); //wait HSE ready
	*CFGR |= (0b01 <<0);
}