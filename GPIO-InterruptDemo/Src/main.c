#include <stdint.h>
#include <stdio.h>

// enable the AHB1 bus for the GPIO
uint32_t volatile *pAHBIENR = (uint32_t*)0x40023830;

// enable the APB2 for EXTI
uint32_t volatile *pAPB2ENR = (uint32_t*)0x40023844;

uint32_t volatile *pGPIOC = (uint32_t*)0x40020800;

uint32_t volatile *pEXTIPR = (uint32_t*)0x40013c14;
uint32_t volatile *pEXTIMR = (uint32_t*)0x40013c00;
uint32_t volatile *pEXTIFTSR = (uint32_t*)0x40013c0c;
uint32_t volatile *pSYSCFG_EXTICR4 = (uint32_t*)0x40013814;

uint32_t *pNVIC_ISER1 = (uint32_t*) 0xe000e104;

void Button_Init(void) {
	//gpioc clock enable
	*pAHBIENR |= (1U<<2);

	//syscfg clock enable
	*pAPB2ENR |= (1U<<14);
	*pSYSCFG_EXTICR4 |= (1U<<5);

	// edge detection configuration
	*pEXTIFTSR |= (1U<<13);

	//ext1 interrupt enable
	*pEXTIMR |= (1U<<13);

	//nvic ENABLE
	*pNVIC_ISER1 |= (1U<<8);



}

int main(void)
{
	Button_Init();
 	while(1){

	}
}

void EXTI15_10_IRQHandler(void){
	printf("Button is Pressed \n");
	*pEXTIPR |= (1U<<13);
}
