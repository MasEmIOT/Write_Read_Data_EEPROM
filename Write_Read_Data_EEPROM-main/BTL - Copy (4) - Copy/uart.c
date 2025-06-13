#include<stdint.h>
#include"rcc.h"
#include"gpio.h"
#include"uart.h"
#include<stdlib.h>
#include<stdio.h>

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
