#include "rcc.h"
#include<stdint.h>
#include "gpio.h"
#include "i2c.h"
uint32_t* I2C_DR = (uint32_t*) (I2C_BASE_ADDRESS + 0x10);
uint32_t* I2C_SR1 = (uint32_t*) (I2C_BASE_ADDRESS + 0x14);
uint32_t* I2C_SR2 = (uint32_t*) (I2C_BASE_ADDRESS + 0x18);
uint32_t* I2C_CR1 = (uint32_t*) (I2C_BASE_ADDRESS + 0x00);
uint32_t* AFIO_MAPR = (uint32_t*) (AFIO_BASE_ADDRESS + 0x04);
void I2C_Init()
{
	uint32_t* RCC_APB1ENR = (uint32_t*) (RCC_BASE_ADDRESS + 0x1C);
	*RCC_APB1ENR |= (1<<21); 
	uint32_t* RCC_APB2ENR = (uint32_t*) (RCC_BASE_ADDRESS + 0x18);
	*RCC_APB2ENR |= (1<<3);
	uint32_t* GPIOB_CRL = (uint32_t*) (GPIOB_BASE_ADDRESS + 0x00);
	*GPIOB_CRL &= ~(0xff << 24);
	*GPIOB_CRL |= (0xDD << 24); //enable PB7,PB6 as alternative function output open-drain mode
	*AFIO_MAPR &= ~(1<<1); // no remap => i2c scl: PB6, sda: PB7
	
	*I2C_CR1 |= (1<<15);//reset I2C
	*I2C_CR1 &= ~(1<<15);
	*I2C_CR1 &= ~(1<<0); //disable I2C before setup I2C
	uint32_t* I2C_CR2 = (uint32_t*) (I2C_BASE_ADDRESS + 0x04);
	*I2C_CR2 |= 16; // Xung clock 16MHz
	uint32_t* I2C_CCR = (uint32_t*) (I2C_BASE_ADDRESS + 0x1C);
	*I2C_CCR = 80; // t_high = 1/200000 = 1/16000000 * 80
	uint32_t* I2C_TRISE = (uint32_t*) (I2C_BASE_ADDRESS + 0x20);
	*I2C_TRISE = 17;
	*I2C_CR1 |= (1<<0); //enable I2C
}

void I2C_Start()
{
	*I2C_CR1 |= (1<<8); //Generate start
	uint32_t* I2C_SR1 = (uint32_t*) (I2C_BASE_ADDRESS + 0x14);
	while(!(*I2C_SR1 & (1<<0))); // wait SB bit set
}

void I2C_Write(uint8_t data)
{
	while(!(*I2C_SR1 & (1<<7))); //wait bit TxE set
	*I2C_DR = data;
	while (!(*I2C_SR1 & (1<<2))); // BTF = 1 => byte transfer succeed
}

void I2C_Address(uint8_t address)
{
	*I2C_DR = address;
	while (!(*I2C_SR1 & (1<<1))); // wait ADDR bit set
	uint8_t temp = *I2C_SR1 | *I2C_SR2; // reading SR1 register and reading SR2 to clear bit
}

void I2C_Stop()
{
	*I2C_CR1 |= (1<<9);
}

void I2C_WriteMultiByte(uint8_t* data, uint8_t size)
{
	while(!(*I2C_SR1 & (1<<7))); //wait bit TxE set
	while(size)
	{
		while(!(*I2C_SR1 & (1<<7))); //wait bit TxE set
		*I2C_DR = (volatile uint32_t)*data++;
		size--;
	}
	while (!(*I2C_SR1 & (1<<2))); // BTF = 1 => byte transfer succeed
}