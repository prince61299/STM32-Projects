#include <stdint.h>
#include "stm32f4xx.h"

//define the macros
#define GPIOAEN (1U<<0)
#define SPI1EN (1U<<12)

void spi_gpio_init(void);
void spi_config(void);
void cs_enable(void);
void cs_disable(void);

void cs_enable(void){
	GPIOA->ODR &= ~(1U<<9); //SET 0
}

void cs_disable(void){
	GPIOA->ODR |= (1U<<9); //SET 1
}

void spi_config(void){

	//TO ENABLE THE CLOCK AXIX OF GPIOA
	RCC->APB2ENR |= SPI1EN;

	//set the FPCL4CK 5
	SPI1->CR1 |= (1U<<3); //SET 1
	SPI1->CR1 &= ~(1U<<4); //SET 0
	SPI1->CR1 &= ~(1U<<5); //SET 0

	//SET CPOL TO 1 CPHA TO 1
	SPI1->CR1 |= (1U<<0); //SET 1
	SPI1->CR1 |= (1U<<1); //SET 1

	//CONFIGURE FOR FULL DUPLEX
	SPI1->CR1 &= ~(1U<<10); //SET 0

	//SET AS MASTER
	SPI1->CR1 |= (1U<<2); //SET 1

	//SET MSB TRANSMITTED FIRST
	SPI1->CR1 &= ~(1U<<7); //SET 0

	//CONFIGURE FOR 8 BIT DATA MODE
	SPI1->CR1 &= ~(1U<<11); //SET 0

	//SELECT SOFTWARE SLAVE MANAGMENT
	SPI1->CR1 |= (1U<<8); //SET 1
	SPI1->CR1 |= (1U<<9); //SET 1

	//SPI ENABLE
	SPI1->CR1 |= (1U<<6); //SET 1
}

void spi_gpio_init(void) {

	//TO ENABLE THE CLOCK AXIX OF GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//TO ENABLE THE CLOCK AXIX OF GPIOA
	RCC->APB2ENR |= SPI1EN;

	//SET PA5, PA6, and PA7 for Alternate Function
	GPIOA ->MODER &= ~(1U<<10); //SET ZERO
	GPIOA ->MODER |= (1U<<11); //SET ONE
	GPIOA ->MODER &= ~(1U<<12); //SET ZERO
	GPIOA ->MODER |= (1U<<13); //SET ONE
	GPIOA ->MODER &= ~(1U<<14); //SET ZERO
	GPIOA ->MODER |= (1U<<15); //SET ONE

	//SET PA9 FOR CHIP SELECT GPIO FOR OUTPUT
	GPIOA ->MODER |= (1U<<18); //SET ONE
	GPIOA ->MODER &= ~(1U<<19); //SET ZERO

	//SET AFR
	GPIOA->AFR[0] |= (1U<<20);
	GPIOA->AFR[0] &= ~(1U<<21);
	GPIOA->AFR[0] |= (1U<<22);
	GPIOA->AFR[0] &= ~(1U<<23);
	GPIOA->AFR[0] |= (1U<<24);
	GPIOA->AFR[0] &= ~(1U<<25);
	GPIOA->AFR[0] |= (1U<<26);
	GPIOA->AFR[0] &= ~(1U<<27);
	GPIOA->AFR[0] |= (1U<<28);
	GPIOA->AFR[0] &= ~(1U<<29);
	GPIOA->AFR[0] |= (1U<<30);
	GPIOA->AFR[0] &= ~(1U<<31);
}

int main(void)
{
    /* Loop forever */
	for(;;);
}
