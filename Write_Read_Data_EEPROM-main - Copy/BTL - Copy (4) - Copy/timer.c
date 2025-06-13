#include<stdint.h>
#include"timer.h"
#include"rcc.h"

void TIM1_Init()
{
	uint32_t* RCC_APB2ENR = (uint32_t*) (RCC_BASE_ADDRESS + 0x18);
	*RCC_APB2ENR |= (1<<11);
	uint32_t* TIM1_PSC  = (uint32_t*) (TIM1_BASE_ADDRESS + 0x28);
	uint32_t* TIM1_ARR  = (uint32_t*) (TIM1_BASE_ADDRESS + 0x2C);
	uint16_t* TIM1_CR1  = (uint16_t*) (TIM1_BASE_ADDRESS + 0x00);
	uint16_t* TIM1_DIER = (uint16_t*) (TIM1_BASE_ADDRESS + 0x0C);
	uint32_t* ISER0 = (uint32_t*) 0xe000e100;
	*TIM1_PSC = 72 - 1;
	*TIM1_ARR = 0xffff - 1; // max value the counter can count (16bit ~ 0xffff), after this the timer will overflow and our time delay will be messed up.
	*TIM1_CR1 |= (1<<0);
	*ISER0 |= (1<<25);
	*TIM1_DIER |= (1<<0);
}
int timer_cnt = 0;

void TIM1_UP_IRQHandler()
{
	timer_cnt++;
	uint16_t* TIM1_SR = (uint16_t*) (TIM1_BASE_ADDRESS + 0x10);
	*TIM1_SR &= ~(1<<0);
}

void TIM1_delay(int time) // TIM1_delay(1) = 100us
{
	timer_cnt = 0;
	//uint32_t* TIM1_ARR  = (uint32_t*) (TIM1_BASE_ADDRESS + 0x2C);
	//*TIM1_ARR = ms - 1;
	while(timer_cnt < time);
}