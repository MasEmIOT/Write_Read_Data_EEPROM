
#include"i2c.h"
#include"lcd.h"
#include"timer.h"
#define LCD_ADDRESS 0x4E

void LCD_Write (uint8_t address, uint8_t *data, int size)
{
	I2C_Start();
	I2C_Address(address);
	for (int i=0; i<size; i++) 
	{
		I2C_Write (*data++);
	}
	I2C_Stop ();
}
void LCD_Send_Command (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);//lay 4 bit cao
	data_l = ((cmd<<4)&0xf0);//lay 4 bit thap
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	LCD_Write (LCD_ADDRESS,(uint8_t *) data_t, 4);
}
void LCD_Sent_Data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	LCD_Write (LCD_ADDRESS,(uint8_t *) data_t, 4);
}
void LCD_Clear (void)
{
	LCD_Send_Command (0x80);
	for (int i=0; i<70; i++)
	{
		LCD_Sent_Data (' ');
	}
}

void LCD_Put_Cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    LCD_Send_Command (col);
}


void LCD_Init (void)
{
	// 4 bit initialisation
	TIM1_delay(500);  // wait for >40ms
	LCD_Send_Command (0x30);
	TIM1_delay(50);  // wait for >4.1ms
	LCD_Send_Command (0x30);
	TIM1_delay(2);  // wait for 200us
	LCD_Send_Command (0x30);
	TIM1_delay(100);
	LCD_Send_Command (0x20);  // 4bit mode
	TIM1_delay(100);

  // dislay initialisation
	LCD_Send_Command (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	TIM1_delay(10);
	LCD_Send_Command (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	TIM1_delay(10);
	LCD_Send_Command (0x01);  // clear display
	TIM1_delay(10);
	TIM1_delay(10);
	LCD_Send_Command (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	TIM1_delay(10);
	LCD_Send_Command (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void LCD_Sent_String (char *str)
{
	while (*str) LCD_Sent_Data (*str++);
}
