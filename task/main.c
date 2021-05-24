#include "main.h"     //заголовочный файл с описанием подключаймых библеотечных модулей
//main обяззательная функция для исполнения кода пользователя               
int main(void)
{	
	uint32_t half_period;//половина периода мигания светодиода
	uint32_t n;//Степень коифецента базовой задержки К=2^n
	RCC->AHBENR|=RCC_AHBENR_GPIOBEN;//Включение тактирования порта В: RCC_AHBENR_GPIOBEN=0х00040000
	GPIOB->MODER|=GPIO_MODER_MODER0_0 | GPIO_MODER_MODER8_0;//переключение линие 0 и 8 порта В в режим "Output"GPIO_MODER_MODER0_0= 0х1;GPIO_MODER_MODER8_0=0х10000
	GPIOB->MODER&=~( GPIO_MODER_MODER12 | GPIO_MODER_MODER13);//переключение линий 12(SW4) и 13(SW3) порта В в режим "Input":
	GPIOB->ODR|=0x100;//Разрешения работы светодиодов на стенде
	half_period = 50000;//Базовая задержка
	while(1)//Реализация процеса мигания светодиода
	{
		n=((GPIOB->IDR)&0x3000)>>12;//определяем степень задержки
		GPIOB->BSRR=0x1;//зажегаем светодиот
		delay(half_period<<n);//задержка
		GPIOB->BSRR=0x10000;//выключение Светодиота
		delay(half_period<<n);//задержка
	}
}
void delay(uint32_t count)//функция задержки : Count-сколько ждать один элемент примерно 2.5мкс
{
	for (uint32_t i=0;i<count;i++);//выполнения пустого цикла для задержки
}
