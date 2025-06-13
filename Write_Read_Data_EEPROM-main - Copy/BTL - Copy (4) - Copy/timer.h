#define TIM1_BASE_ADDRESS  0x40012C00

void TIM1_Init();
void TIM1_UP_IRQHandler();
void TIM1_delay(int time);
