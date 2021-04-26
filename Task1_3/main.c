#include "main.h"

int main(void)
{
	uint32_t byf;
	int32_t i=0;
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;
	GPIOB->OSPEEDR|=0X5555; //все порты на скорость в 10 мгц - 0b0101010101010101
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |
								GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
								GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER9_0;
	GPIOB->MODER&=~( GPIO_MODER_MODER12 | GPIO_MODER_MODER13 | GPIO_MODER_MODER11 | GPIO_MODER_MODER10);
	GPIOB->ODR|=0x200;
	unsigned reg[3] = {0x23F, 0x206, 0x25B};
	unsigned namber[4] = {0, 0, 0, 0};
	while(1)
	{
		namber[0] = 0;
		namber[1] = 0;
		namber[2] = 0;
		namber[3] = 0;
		i=0;
		byf=(((GPIOB->IDR)&0x1000)>>12)+(((GPIOB->IDR)&0x2000)>>12)+4+(((GPIOB->IDR)&0x4000)>>11)+(((GPIOB->IDR)&0x8000)>>11);
		while(byf!=0)
		{
			namber[i]=byf%3;
			byf=byf/3;
			i=i+1;
		}
		for (int32_t s=3;s>=0;s--)
		{
				if(s<i)
				{
					GPIOB->ODR=reg[namber[s]];
					if(s==0)
					{
						GPIOB->ODR|=0x280;
					}
					delay(500000);
					GPIOB->ODR=0xffff0000;
					delay(50000);
				}
		}
	}
}
void delay(uint32_t count)
{
	for (uint32_t i=0;i<count;i++);
}
