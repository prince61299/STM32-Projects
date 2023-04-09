#include <stdint.h>
#include <stdio.h>

uint32_t *pAPB1ENR = (uint32_t*)0x40023840;
uint32_t *pAPB2ENR = (uint32_t*)0x40023844;
uint32_t *pGPIOMODER = (uint32_t*)0x40020000;
uint32_t *pADC_SR = (uint32_t*)0x40012000;
uint32_t *pADC_CR2 = (uint32_t*)0x40012008;
uint32_t *pADC_SQR3 = (uint32_t*)0x40012034;
uint32_t *pADC_DR = (uint32_t*)0x4001204c;
uint16_t analogValue;

//Initialization
void ADC_Init();
void delay();

void ADC_Init(){
	*pAPB1ENR |= 1; //enable the clock access for GPIOA
	*pAPB2ENR |= 0X100; //enable the clock access for ADC1
	*pGPIOMODER |= 0XC; //configure GPIO for Analog Pin

	*pADC_CR2 |= 0; //to disable the ADC
	*pADC_SQR3 = 1; //to set the sequence
	*pADC_CR2 |= 1; //to enable the ADC
}

void delay(){
	for(uint32_t i=0;i<300000;i++);
}

int main(void)
{
    printf("Display ADC value \n");
    ADC_Init();

    while(1){
    	//start the ADC Conservation
    	*pADC_CR2 |= 0x40000000; // Start ADC Conversion

    	while(!(*pADC_SR & 2)){} //Wait Till End of the Conversion
    	analogValue = *pADC_DR;
    	printf("Display ADC Value %d \n", analogValue);
    	fflush(stdout);
    	delay();
    }

	for(;;);
}
