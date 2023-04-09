#include <stdint.h>

#define USART2_IRQNO 38

// AHBIENR 0x40023830
// APBIENR 0x40023840
// GPIOMODER 0x40020000
// GPIOAFRL 0x40020020
// USART2_SR (status) 0x40004400
// USART_DR (data) 0x40004404
// USART_BRR (baud rate) 0x40004408
// USART_CR1 (control) 0x4000440c

uint32_t *pAB1ENR   = (uint32_t*)0x40023830;
uint32_t *pAPB1ENR   = (uint32_t*)0x40023840;
uint32_t *pGPIOAMODER = (uint32_t*)0x40020000;
uint32_t *pGPIOAODR = (uint32_t*)0x40020014;
uint32_t *pGPIOAAFRL  = (uint32_t*)0x40020020;
uint32_t *pUSART2_SR = (uint32_t*)0x40004400;
uint32_t *pUSART2_DR  = (uint32_t*)0x40004404;
uint32_t *pUSART2_BRR = (uint32_t*)0x40004408;
uint32_t *pUSART2_CR1 = (uint32_t*)0x4000440c;

uint32_t *pNVIC_ISER1 = (uint32_t*)0xe000e104;
uint32_t *pNVIC_ISPR1 = (uint32_t*)0xe000e204;

void Usart_Init(void);
void Usart_Write(int ch);
char Usart_Read(void);

void Usart_Init(void){
//	-> Enable the clock access for GPIOA
	*pAB1ENR |= (1U<<0); // U for unsigned

//	-> Enable the clock access for USART2
	*pAPB1ENR |= (1U<<17);

//	Configure GPIOA Mode Register to ALT Function
	// SET PA2 as ALT Function
	*pGPIOAMODER &= ~(1U<<4); //RESET
	*pGPIOAMODER |= (1U<<5); //SET

// SET PA3 as ALT Function
	*pGPIOAMODER &= ~(1U<<6); //RESET
	*pGPIOAMODER |= (1U<<7); //SET

	//Configure the GPIO to output function
	// SET PA5 as Output
	*pGPIOAMODER |= (1U<<10); //SET
	*pGPIOAMODER &= ~(1U<<11); //RESET

	//Configure the ALT Function to AFRL for PA2 and PA3
		*pGPIOAAFRL |= (1U<<8); //SET
		*pGPIOAAFRL |= (1U<<9); //SET
		*pGPIOAAFRL |= (1U<<10); //SET
		*pGPIOAAFRL &= ~(1U<<11); //RESET

		*pGPIOAAFRL |= (1U<<12); //SET
		*pGPIOAAFRL |= (1U<<13); //SET
		*pGPIOAAFRL |= (1U<<14); //SET
		*pGPIOAAFRL &= ~(1U<<15); //RESE

	//Configure the BRR to 9600
	*pUSART2_BRR = 0X0683; //Load the value


	// Configure the USART for Tx and Rx
	*pUSART2_CR1 |= 0X0008;
	*pUSART2_CR1 |= 0X0004;
	*pUSART2_CR1 |= (1U<<5); //set

	*pUSART2_CR1 |= 0X2000; // to SET the USART Enable

	*pNVIC_ISER1 |= (1<<(USART2_IRQNO%32));
}

void Usart_Write(int ch) {
	//make sure that the Transmit data Register is Empty
	while(!(*pUSART2_SR & 0x0080)) {}
	*pUSART2_DR =(ch&0xff);
}

char Usart_Read(void) {
	while(!(*pUSART2_SR & 0x0020)) {}
	return *pUSART2_DR;
}

int __io_putchar(int ch){
	Usart_Write(ch);
	return ch;

}

int main(void)
{
	char ch;
	Usart_Init();
	printf("Enter the loop \n");

	while(1){

	}
	for(;;);
}

void USART2_IRQHandler(void){
	char ch;
	*pNVIC_ISPR1 |= (1<<(USART2_IRQNO%32));
	ch = Usart_Read();

			if(ch == 'a'){
				*pGPIOAODR |= (1<<5);
			}
			else {
				*pGPIOAODR &= ~(1<<5);
			}
			printf("Data is %c \n", ch);
}
