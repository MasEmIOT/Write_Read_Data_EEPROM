#define UART_BASE_ADDRESS  0x40004400

void UART_Init();
void UART_Send_Byte(char data);
void UART_Send_MultiByte(char* arr, int size);
void USART2_IRQHandler();
char UART_Receive_Byte();
void UART_Write_EEPROM();
void UART_Read_EEPROM();

	