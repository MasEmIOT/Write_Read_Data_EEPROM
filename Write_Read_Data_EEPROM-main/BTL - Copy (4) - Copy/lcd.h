#include<stdint.h>

void LCD_Write (uint8_t address, uint8_t *data, int size);
void LCD_Send_Command (char cmd);
void LCD_Sent_Data (char data);
void LCD_Clear (void);
void LCD_Put_Cur(int row, int col);
void LCD_Init (void);
void LCD_Sent_String (char *str);