#include "main.h"     //заголовочный файл с описанием подключаймых библеотечных модулей
//main обяззательная функция для исполнения кода пользователя  
int main(void)
{
	uint32_t half_period;//половина периода мигания светодиода
	uint32_t n;//Степень коифецента базовой задержки К=2^n
	uint32_t swreg;//переменная режима работы 0-светодиот "бежит" 1-поочередно загораются а потом гаснут
	uint32_t swrev;//переменная режимы работы с реверсом 0-реверс с право на лево 1- норма с лево на право
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;//Включение тактирования порта В: RCC_AHBENR_GPIOBEN=0х00040000
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 |//переключение линие с 0 по 8 порта В в режим "Output"
								GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 |
								GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
	GPIOB->OSPEEDR|=0X5555; //все порты на скорость в 10 мгц - 0b0101010101010101
	GPIOB->MODER&=~( GPIO_MODER_MODER12 | GPIO_MODER_MODER13 | GPIO_MODER_MODER14 | GPIO_MODER_MODER15 );//переключение линий с 12(SW4) по 15(SW1) порта В в режим "Input":
	GPIOB->ODR|=0x100;//

	half_period = 50000;
	while(1)
	{
		n=((GPIOB->IDR)&0x3000)>>12;
		swreg=((GPIOB->IDR)&0x8000)>>15;
		swrev=((GPIOB->IDR)&0x4000)>>14;
		if(swrev==1)
		{
			if(swreg==1)
			{
				GPIOB->BSRR|=0x1;//0
				delay(half_period<<n);
				GPIOB->BSRR|=0x2;//1
				delay(half_period<<n);
				GPIOB->BSRR|=0x4;//2
				delay(half_period<<n);
				GPIOB->BSRR|=0x80;//7
				delay(half_period<<n);
				GPIOB->BSRR|=0x40;//6
				delay(half_period<<n);
				GPIOB->BSRR|=0x20;//5
				delay(half_period<<n);
				GPIOB->BSRR|=0x10;//4
				delay(half_period<<n);
				GPIOB->BSRR|=0x8;//3
				delay(half_period<<n);
				GPIOB->BSRR|=0x10000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x20000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x40000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x800000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x400000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x200000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x100000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x80000;
				delay(half_period<<n);
			}
			else
			{
				GPIOB->BSRR|=0x1;
				delay(half_period<<n);
				GPIOB->BSRR|=0x10000;
				GPIOB->BSRR|=0x2;
				delay(half_period<<n);
				GPIOB->BSRR|=0x20000;
				GPIOB->BSRR|=0x4;
				delay(half_period<<n);
				GPIOB->BSRR|=0x40000;
				GPIOB->BSRR|=0x80;
				delay(half_period<<n);
				GPIOB->BSRR|=0x800000;
				GPIOB->BSRR|=0x40;
				delay(half_period<<n);
				GPIOB->BSRR|=0x400000;
				GPIOB->BSRR|=0x20;
				delay(half_period<<n);
				GPIOB->BSRR|=0x200000;
				GPIOB->BSRR|=0x10;
				delay(half_period<<n);
				GPIOB->BSRR|=0x100000;
				GPIOB->BSRR|=0x8;
				delay(half_period<<n);
				GPIOB->BSRR|=0x80000;
			}
		}
		else
		{
			if(swreg==1)
			{
				GPIOB->BSRR|=0x8;
				delay(half_period<<n);
				GPIOB->BSRR|=0x10;
				delay(half_period<<n);
				GPIOB->BSRR|=0x20;
				delay(half_period<<n);
				GPIOB->BSRR|=0x40;
				delay(half_period<<n);
				GPIOB->BSRR|=0x80;
				delay(half_period<<n);
				GPIOB->BSRR|=0x4;
				delay(half_period<<n);
				GPIOB->BSRR|=0x20;
				delay(half_period<<n);
				GPIOB->BSRR|=0x1;
				delay(half_period<<n);
				GPIOB->BSRR|=0x80000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x100000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x200000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x400000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x800000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x40000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x20000;
				delay(half_period<<n);
				GPIOB->BSRR|=0x10000;
				delay(half_period<<n);
			}
			else
			{
				GPIOB->BSRR|=0x8;
				delay(half_period<<n);
				GPIOB->BSRR|=0x80000;
				GPIOB->BSRR|=0x10;
				delay(half_period<<n);
				GPIOB->BSRR|=0x100000;
				GPIOB->BSRR|=0x20;
				delay(half_period<<n);
				GPIOB->BSRR|=0x200000;
				GPIOB->BSRR|=0x40;
				delay(half_period<<n);
				GPIOB->BSRR|=0x400000;
				GPIOB->BSRR|=0x80;
				delay(half_period<<n);
				GPIOB->BSRR|=0x800000;
				GPIOB->BSRR|=0x4;
				delay(half_period<<n);
				GPIOB->BSRR|=0x40000;
				GPIOB->BSRR|=0x2;
				delay(half_period<<n);
				GPIOB->BSRR|=0x20000;
				GPIOB->BSRR|=0x1;
				delay(half_period<<n);
				GPIOB->BSRR|=0x10000;
			}
		}
	}
}

void delay(uint32_t count)
{
for (uint32_t i=0;i<count;i++);
}
