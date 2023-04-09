#include <stdint.h>
#include "stm32f4xx.h"


int main(void)
{
	//ENABLE THE CLOCK ACCESS FOR GPIOA
	RCC->AHB1ENR |= 0X01;
	//ENABLE THE CLOCK ACCESS FOR TIM2CH1
	RCC->APB1ENR |= 0X01;
	//SET THE MODE REGISTER
	GPIOA->MODER |= 0X00000800;
	//AFR
	GPIOA->AFR[0] |= 0X00100000;

	//TIMER REGISTER
	TIM2->PSC = 10-1; //DIVIDE THE TIMER CLOCK BY 10
	TIM2->ARR =  26667-1;
	TIM2->CNT = 0; //INITIAL COUNT IS ZERO
	TIM2->CCMR1 = 0X0060; //ENABLE PWM MODE
	TIM2->CCER = 1; //ENABLE PWM OF CHANNEL 1
	TIM2->CCR1 = 8889-1;
	TIM2->CR1 = 1; //TO ENABLE THE TIMER

	while(1){

	}


    /* Loop forever */
	for(;;);
}