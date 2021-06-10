#include "main.h"

static uint8_t flag=0x0000;
static int32_t a=8,sum=8;
static uint8_t i=1;
int main()
{
	__disable_irq();
	int8_t q=-5;
	InitUSART1();																						
	NVIC->ISER[0] |= 0x08000000; 													
	__enable_irq();									
	while(i<=36)
	{													
		
		a=a*q;		
		sum=sum*a;
		i++;
		number_out(a);
		number_out(i);
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
	volatile uint32_t j;																			
	for (j=0;j<count;j++);																			
}

void debug(void)
{
	uint8_t mes_a[2]={0x61,0x3d};
	uint8_t mes_n[3]={0x2c,0x6e,0x3d};
	uint8_t mes_q[3]={0x2a,0x2d,0x35};
	uint8_t mes_s[3]={0x2c,0x73,0x3d};
	while((flag&0x1000)>>3==1)
	{
		if((flag&0x100)>>2==1)//i
		{																			
			for(uint8_t v=0;v<2;v++)
			{	
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = mes_a[v];
			}
			number_out(a);
			for(uint8_t v=0;v<2;v++)
			{	
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = mes_n[v];
			}
			number_out(i);
		}																												
		if((flag&0x10)>>1==1)//e
		{																					
			for(uint8_t v=0;v<2;v++)
			{	
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = mes_a[v];
			}
			number_out(a);	
			while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
			USART1->TDR = 0x3d;
			number_out(a/-5);
			for(uint8_t v=0;v<2;v++)
			{	
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = mes_q[v];
			}
		}														
		if((flag&0x1)==1)//s
		{																					
			for(uint8_t v=0;v<2;v++)
			{	
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = mes_a[v];
			}
			number_out(a);
			for(uint8_t v=0;v<2;v++)
			{	
				while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
				USART1->TDR = mes_s[v];
			}
			number_out(sum);
		}
	}
}

void USART1_IRQHandler(void)
{
		uint8_t pack;
		if (USART1->ISR & USART_ISR_RXNE) { 												//Если в регистре состояний USART1 установлен флаг "RXNE", то
		pack=(uint8_t)USART1->RDR; 																//Чтение принятого битового пакета из буферного регистра приемника USART1 
		 														
		if(pack==0x69)//i
		{																			
			flag|=0x100;
		}																												
		if(pack==0x65)//e
		{																					
				flag|=0x10;
		}														
		if(pack==0x73)//s
		{																					
			flag|=0x1;
		}
		if(pack==0x66 && ((flag&0x1000)>>3)==0)//f
		{																				
			flag|=0x1000;
		}
		else if(pack==0x66 && ((flag&0x1000)>>3)==1)//f
		{
			flag&=0x0;
		}
																															
	}
	if (USART1->ISR & USART_ISR_TC) 
		{
			USART1->ICR=USART_ICR_TCCF;
		}		
}

void number_out(int32_t num)
{
	int32_t con=48;
	int32_t ten=10;
	uint8_t flag_0=0;
	if(num<0)
	{
		while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
		USART1->TDR = 0x2d;
		num=num*(-1);
	}
	int32_t mes[30];
	for(uint8_t k=29;num!=0;k--)
	{
		mes[k]=num%ten+con;
		num=num/ten;
	}	
	for(uint8_t k=0;k<30;k++)
	{
		if((!(mes[k]==0x30))&&(!flag_0))
		{
			flag_0=1;
			while ((USART1->ISR & USART_ISR_TXE) == 0) {} 						
			USART1->TDR = (uint8_t)mes[k];
		}		
	}
}
