#include<stdint.h>
#include"rcc.h"
void Clock_HSE_Init() // use hse as system clock
{
	uint32_t* CR = (uint32_t*) (RCC_BASE_ADDRESS + 0x00);
	uint32_t* CFGR = (uint32_t*) (RCC_BASE_ADDRESS + 0x04);
	*CR |= (1<<16); //enable HSE
	while ((*CR >> 17) & 1 == 0); //wait HSE ready
	*CFGR |= (0b01 <<0);
}

void Clock_PLL_Init()
{
	uint32_t* CR = (uint32_t*) (RCC_BASE_ADDRESS + 0x00);
	uint32_t* CFGR = (uint32_t*) (RCC_BASE_ADDRESS + 0x04);
	*CR |= (1<<16) | (1<<24);//enable HSE, PLL
	while ((*CR >> 17) & 1 == 0); //wait HSE ready
	while ((*CR >> 25) & 1 == 0); //wait PLL ready
	*CFGR |= (1<<16); //set HSE as PLL source
	*CFGR |= (0b0111 << 18); // PLLMul x9
	*CFGR |= (0b10 << 0); // PLL selected as system clock
  *CFGR |= (0b100 << 8); // APB1 prescaler /2
}