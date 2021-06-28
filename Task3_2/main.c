#include "main.h"

static uint8_t buf[6]={0,0,0,0,0,0};							 
static uint32_t iReadyTX; 
static int ent_nev_znac=0;
static uint8_t half_period_mas[6]={53, 48, 48, 48, 48, 0};
static uint32_t period=50000;
static uint8_t pack;
int main()
{
	
	
	
	__disable_irq();					
	uint32_t half_period;
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;													
	GPIOB->MODER|= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER8_0| GPIO_MODER_MODER1_0; 
	GPIOB->ODR|=0x100;																				//Разрешение работы светодиодов на стенде CТМ_01 с помощью установки логической "1" на выводе РВ.8									
	//---------------------------------------------
	iReadyTX = 0;																							//Сброс количества битовых пакетов, подготовленных для передачи на ПК																					//Сброс количества битовых пакетов, переданных на ПК через USART
	InitUSART1();																							//Инициализация модуля USART1
	NVIC->ISER[0] |= 0x08000000; 															//Разрешение в NVIC прерывания от модуля USART1 	
	__enable_irq();																						//Глобальное разрешение прерываний 																					//Инициализация переменной, хранящей половину периода переключения светодиода
	while(1){														//Определение степени коэффициента базовой задержки: К=2^n
		half_period=period;
		GPIOB->BSRR= 0x1;																				//Зажечь светодиод, подключенный к выводу РВ.О
		delay(half_period);																	//Задержка после включения светодиода
		GPIOB->BSRR=0x10000; 																		//Погасить светодиод, подключенный к выводу РВ.О
		delay (half_period);																	//Задержка после выключения светодиода 
	}
}

//Функция инициализации USART лабораторного комплекса
void InitUSART1(){
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;													//Включение тактирования USART1
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;														//Включение тактирования порта А
	
	//Настройка линий порта А: РА9(ТХ_1) - выход передатчика; PA10(RX_1) - вход приёмника
	GPIOA->MODER |= 0x00280000;																		//Перевести линии РА9 и РА10 в режим альтернативной функции
	GPIOA->AFR[1] |= 0x00000110;																	//Включить на линиях РА9 и РА10 альтернативную функцию AF1
	
	//Настройка линии передатчика Тх (РА9)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;														//Сбросить 9 бит GPIOA->OTYPER - переключение в режим push-pull для линии РА9 (активный выход) 
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;														//Отключение подтягивающих резисторов для линии РА9 
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9;											//Установка высокой скорости синхронизации линии РА9
	
	//Настройка линии приемника Rx (РА10)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;													//Сброс режима подтягивающих резисторов для линии РА10
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0;													//Включение подтягивающего резистора pull-up на входной линии РА10 (вход приемника Rx)
	
	//Конфигурирование USART
	USART1->CR1 &= ~USART_CR1_UE;																	//Запрещение работы модуля USART1 для изменения параметров его конфигурации
	USART1->BRR=69;																								/*Настройка делителя частоты, тактирующего USART и задающего скорость приема и передачи данных на уровне 115200 бит/с: 
																																	Частота тактирующего генератора = 8 МГц 
																																	Скорость обмена по USART - 115200 бит/с; коэффициент деления - 8000000 / 115200 - 69,4444(4); Округленное значение - 69*/
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE;										/*Разрешить работу приемника и передатчика USART. Остальные биты этого регистра сброшены, что обеспечивает: 
																																	количество бит данных в пакете 8;
																																	контроль четности - отключен; 
																																	прерывания по любым флагам USART - запрещены;
																																	состояние USART - отключен*/
	USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_TCIE; 						/*Разрешение (в модуле USART1) на выдачу сигнала прерывания при возникновении событий:
																																	прием кадра в буферный регистр; завершение передачи кадра */
	USART1->CR2 = 0;																							//Количество стоповых бит - 1
	USART1->CR3 = 0;																							//DMA1 - отключен
	USART1->CR1 |= USART_CR1_UE;																	//По завершении конфигурирования USART разрешить его работу (биту UE регистра CR1 присвоить 1)
}


void delay(uint32_t count)
{
	volatile uint32_t i;																			
	for (i=0;i<count;i++);																			
}

 
void USART1_IRQHandler(void)
{
	
	uint32_t namb=0;
	uint8_t old_per[36]={0xd0, 0xbf, 0xd0, 0xb5, 0xd1, 0x80, 0xd0, 0xb8, 0xd0, 0xbe, 0xd0, 0xb4, 0x20, 0xd0, 0xbc, 0xd0, 0xb8, 0xd0, 0xb3, 0xd0, 0xb0, 0xd0, 0xbd, 0xd0, 0xb8, 0xd1, 0x8f, 0x2c, 0x20, 0xd0, 0xbc, 0xd0, 0xba, 0xd1, 0x81, 0x3a};
	uint8_t new_value[36]={0xd0, 0x9d, 0xd0, 0xbe, 0xd0, 0xb2, 0xd0, 0xbe, 0xd0, 0xb5, 0x20, 0xd0, 0xb7, 0xd0, 0xbd, 0xd0, 0xb0, 0xd1, 0x87, 0xd0, 0xb5, 0xd0, 0xbd, 0xd0, 0xb8, 0xd0, 0xb5, 0x2c, 0x20, 0xd0, 0xbc, 0xd0, 0xba, 0xd1, 0x81, 0x3a};
	
	if (USART1->ISR & USART_ISR_RXNE) { 												
		pack=(uint8_t)USART1->RDR; 																
		 																	
		if(pack>=48&pack<=57&ent_nev_znac&iReadyTX+1<=6)
		{																					
			buf[(uint8_t)iReadyTX] = pack;
			while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
			USART1->TDR = buf[(uint8_t)iReadyTX];
			iReadyTX++;
		}																													
		if(pack==13)
		{																					
			if(ent_nev_znac)
			{
				namb=buf[0]-48;

				for (uint8_t i=1;i<iReadyTX;i++)
					{	
						namb=(namb*10)+(uint32_t)(buf[i]-48);
					}
				period=(uint32_t)namb;
				ent_nev_znac=0;
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = 0x0d;
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = 0x0a;
				for (uint8_t i=0;i<6;i++)
					{
						half_period_mas[i]=buf[i];
						buf[i]=0;
					}
				iReadyTX=0;
				
			}
			else
			{
				ent_nev_znac=1;
				for (uint8_t i=0;i<36;i++)
					{
						while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
						USART1->TDR = old_per[i];
					}
				for (uint8_t i=0;i<6;i++)
					{
						while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
						USART1->TDR = half_period_mas[i];
					}
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = 0x0d;
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = 0x0a;
				for (uint8_t i=0;i<36;i++)
					{
						while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
						USART1->TDR = new_value[i];
					}
			}	
		}														
		if(pack==0x7f&iReadyTX!=0)
		{																					
			buf[(uint8_t)iReadyTX--] = 0;
			while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
			USART1->TDR = 0x7f; 
		}																													
																															
	}

	if (USART1->ISR & USART_ISR_TC) {
			USART1->ICR=USART_ICR_TCCF;
			
		}
		
}
