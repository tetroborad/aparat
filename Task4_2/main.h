#include "stm32f0xx.h"						//	Подключение библиотеки с моделью stm32f0xx
void InitPortB(void);							//	Декларация функции инициализации порта B
void InitTimer6(void);						//	Декларация функции инициализации таймера
void TIM6_DAC_IRQHandler(void);		
