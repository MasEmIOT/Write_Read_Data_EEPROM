#include<stdint.h>
#define I2C_BASE_ADDRESS 0x40005400

void I2C_Init();
void I2C_Start();
void I2C_Write(uint8_t data);
void I2C_Address(uint8_t address);
void I2C_Stop();
void I2C_WriteMultiByte(uint8_t* data, uint8_t size);