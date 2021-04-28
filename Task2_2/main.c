#include <main.h>
int main(void)
{
	int lait[8]={0,0,0,0,0,0,0,0};
	int s=0x10000;
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |
								GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
								GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
	InitUSART1();
	while(1)
	{
		while ((USART1->ISR & USART_ISR_RXNE) == 0) { };
		uint8_t d = USART1->RDR;
		if(d==0x31)
		{
			if(lait[0]==0)
			{
				lait[0]=1;
				GPIOB->BSRR|=0x1;
				printed(1,lait[0]);
			}
			else
			{
				lait[0]=0;
				GPIOB->BSRR|=0x10000;
				printed(1,lait[0]);
			}
		}
		if(d==0x32)
		{
			if(lait[1]==0)
			{
				lait[1]=1;
				GPIOB->BSRR|=0x2;
				printed(2,lait[1]);
			}
			else
			{
				lait[1]=0;
				GPIOB->BSRR|=0x20000;
				printed(2,lait[1]);
			}
		}
		if(d==0x33)
		{
			if(lait[2]==0)
			{
				lait[2]=1;
				GPIOB->BSRR|=0x4;
				printed(3,lait[2]);
			}
			else
			{
				lait[2]=0;
				GPIOB->BSRR|=0x40000;
				printed(3,lait[2]);
			}
		}
		if(d==0x34)
		{
			if(lait[3]==0)
			{
				lait[3]=1;
				GPIOB->BSRR|=0x80;
				printed(4,lait[3]);
			}
			else
			{
				lait[3]=0;
				GPIOB->BSRR|=0x800000;
				printed(4,lait[3]);
			}
		}
		if(d==0x35)
		{
			if(lait[4]==0)
			{
				lait[4]=1;
				GPIOB->BSRR|=0x40;
				printed(5,lait[4]);
			}
			else
			{
				lait[4]=0;
				GPIOB->BSRR|=0x400000;
				printed(5,lait[4]);
			}
		}
		if(d==0x36)
		{
			if(lait[5]==0)
			{
				lait[5]=1;
				GPIOB->BSRR|=0x20;
				printed(6,lait[5]);
			}
			elsу
			{
				lait[5]=0;
				GPIOB->BSRR|=0x200000;
				printed(6,lait[5]);
			}
		}
		if(d==0x37)
		{
			if(lait[6]==0)
			{
				lait[6]=1;
				GPIOB->BSRR|=0x10;
				printed(7,lait[6]);
			}
			else
			{
				lait[6]=0;
				GPIOB->BSRR|=0x100000;
				printed(7,lait[6]);
			}
		}
		if(d==0x38)
		{
			if(lait[7]==0)
			{
				lait[7]=1;
				GPIOB->BSRR|=0x8;
				printed(8,lait[7]);
			}
			else
			{
				lait[7]=0;
				GPIOB->BSRR|=0x80000;
				printed(8,lait[7]);
			}
		}
		if(d==0x39)
		{
			for(int i=0;i<8;i++)
			{
				printed(i+1,lait[1]);
			}
		}
		if(d==0x30)
		{
			s=0x10000;
			for(int i=0;i<8;i++)
			{
				lait[i]=0;
				GPIOB->BSRR|=s;
				s=s<<1;
				printed(i+1,lait[1]);
			}
		}
		
	}
}
void printed(int i, int v)
{
	while ((USART1->ISR & USART_ISR_TXE) == 0) { };
	uint8_t mesand[20]={0xd1,0xf2,0xe3,0xf2,0xf3,0xf1,0x20,0xf1,0xe2,0xe5,0xf2,0xee,0xe4,0xe8,0xee,0xe4,0xe0,i,0x3d,v};
	for(int i=0;i<20;i++)
	{
		USART1->TDR = mesand[i];
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
