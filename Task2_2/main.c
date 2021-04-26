#include <main.h>
int main(void)
{
	int lait[8]={0,0,0,0,0,0,0,0}
	//int mes[]={0xd1,0xf2,0xe3,0xf2,0xf3,0xf1,0x20,0xf1,e2,e5,f2,ee,e4,e8,ee,e4,e0,i,3d,lait[i]} переместить в нужное место
	InitUSART1();
	while(1)
	{
		while ((USART1->ISR & USART_ISR_RXNE) == 0) { }
		uint8_t d = USART1->RDR;
		while ((USART1->ISR & USART_ISR_TXE) == 0) { }
		USART1->TDR = d;
	}
}

void InitUSART1(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= 0x00280000;
	GPIOA->AFR[1] |= 0x00000110;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR10;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10_0;
	USART1->CR1 &= ~USART_CR1_UE;
	USART1->BRR=69;
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
	USART1->CR2 = 0;
	USART1->CR3 = 0;
	USART1->CR1 |= USART_CR1_UE;
}
