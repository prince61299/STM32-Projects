#include <stdint.h>

#define SYSTICK_LOAD_VALUE 16000 // to generate 1ms delay
#define CTRL_ENABLE (1U<<0)
#define CTRL_CLKSRC (1U<<2)
#define CTRL_COUNTFLAG (1U<<16)

// for Enabling the GPIO
#define GPIOAEN (1U<<0);
#define PIN5 (1U<<5); // for LED

//this is for Blinking the LEDs
#define LED (1U<<5);

void systickdelayms(int delay);

uint32_t *pSTCSR = (uint32_t*)0xe000e010;
uint32_t *pSTRVR = (uint32_t*)0xe000e014;
uint32_t *pSTCVR = (uint32_t*)0xe000e018;
uint32_t *pSTCR  = (uint32_t*)0xe000e01c;

//for LEDs
uint32_t *pAHB1ENR  = (uint32_t*)0x40023830;

//for GPIOA MODER
uint32_t *pGPIOAMODER  = (uint32_t*)0x40020000;

//OUTPUT DATA REGISTER
uint32_t *pGPIOAMODR  = (uint32_t*)0x40020014;

void systickdelayms(int delay){
	//load the reload value register
	*pSTRVR |= SYSTICK_LOAD_VALUE;

	// set the Current Value register to 0
	*pSTCVR |= 0;

	//current status register
	*pSTCSR |= CTRL_ENABLE;
	*pSTCSR |= CTRL_CLKSRC;

	for(int i=0; i<delay;i++){
		while((*pSTCSR & CTRL_COUNTFLAG) == 0){}
	}
	*pSTCSR |= 0;

}

int main(void)
{
	//for Enable the clock for GPIO
	*pAHB1ENR |= GPIOAEN;
	//set the MODE register
	*pGPIOAMODER |= (1U<<10);
	//Reset the output data register
	*pGPIOAMODR &= ~(1U<<11);

	while(1) {
		*pGPIOAMODR ^= LED;
		systickdelayms(1000);
	}

}
