
#define GPIOA_BASE_ADDRESS 0x40010800
#define GPIOB_BASE_ADDRESS 0x40010C00
#define AFIO_BASE_ADDRESS  0x40010000



typedef enum
{
	PB10 = 0, PB1 = 1, PB0 = 2, PA8 = 3, PB15 = 4, PB14 = 5, PA9 = 6, PA10 = 7
}IO_num;
typedef enum
{
	GPIOA, GPIOB
}GPIO_Port;

void Clock_HSE_Init();
void IO_Output_Init();
void IO_Input_Init();
void GPIO_Write_Pin(GPIO_Port port, unsigned short pin, unsigned short state);
int GPIO_Read_Pin(GPIO_Port port, unsigned short pin);
void Enable_Init();
void Address_Init();