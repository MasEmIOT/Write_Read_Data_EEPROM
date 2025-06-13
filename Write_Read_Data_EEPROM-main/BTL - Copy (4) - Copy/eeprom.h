#include<stdint.h>
void Address_EEPROM(uint32_t address);
void Write_Number_EEPROM(uint8_t num, uint32_t address);
void Write_Character_EEPROM(char c, uint32_t address);
void Write_String_EEPROM(char* str, uint32_t start_address);
int Read_Number_EEPROM();
char Read_Character_EEPROM();
void Read_Data(uint32_t add);