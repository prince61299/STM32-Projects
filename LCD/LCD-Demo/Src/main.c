#include <stdint.h>
#include "stm32f4xx.h"

//PB5 - RS
//PB6 - RW
//PB7 - EN
//PC0-PC7 -> D0-D7 IN LCD

#define RS 0X20 //0010 0000
#define RW 0X40 //0100 0000
#define EN 0X80 //1000 0000

unsigned char Message[] = ("Prince Kushwaha");

void GPIO_Init();
void LCD_Data(unsigned char data);
void LCD_Command(unsigned char command);
void delayMS(int delay);
void LCD_Init();

void delayMS(int delay){
	int i;
	for(;delay>0; delay--){
		for(i=0;i<3195;i++){

		}
	}
}

void LCD_Data(unsigned char data){
	//RS=1, RW=0
	GPIOB->BSRR = RS;
	GPIOB->BSRR = RW << 16;
	GPIOB->ODR = data;

	//EN = 1
	GPIOB->BSRR = EN; //set
	delayMS(30);
	GPIOB->BSRR = EN << 16; // clear
	delayMS(30);
}

void LCD_Command(unsigned char command){
	//RS=0, RW=0
	GPIOB->BSRR = (RS | RW) << 16;
	GPIOB->ODR = command;

	//EN = 1
	GPIOB->BSRR = EN; //set
	delayMS(30);
	GPIOB->BSRR = EN << 16; // clear

}

void GPIO_Init(){
	//set RCC AHB1ENR
	RCC->AHB1ENR |= 0X06;
	//set GPIOB MODE Register as O/P
	GPIOB->MODER |= 0X5400;
	//set GPIOC MODE Register as O/P
	GPIOC->MODER |= 0X00005555;
	//set BSRR of GPIOB as Low
	GPIOB->BSRR |= 0X00C; // this is for set EN and RW as LOW
}

void LCD_Init(void){
	unsigned char i = 0;

	GPIO_Init();

	LCD_Command(0X38); //16x2
	delayMS(20);
	LCD_Command(0X0C);
	delayMS(20);
	LCD_Command(0X01);
	delayMS(20);
	LCD_Command(0X80);
	delayMS(20);

	while(Message[i] != '\0'){
		LCD_Data(Message[i]);
		i++;
	}
	delayMS(50);
}

int main(void)
{
    /* Loop forever */
	LCD_Init();

	//Program for LCD Commands

	for(;;);
}
