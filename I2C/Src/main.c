#include <stdint.h>
#include "stm32f4xx.h"

#define GPIOBEN (1U<<1)
#define I2C_100KHZ 80
#define SD_MODE_MAX_RISE_TIME 17

#define CR1_START (1U<<8)
#define CR1_PE (1U<<0)
#define CR1_ACK (1U<<10)
#define CR1_STOP (1U<<9)

#define SR1_SB (1U<<0)
#define SR1_ADDR (1U<<1)
#define SR1_TXE (1U<<7)
#define SR1_RXNE (1U<<6)
#define SR1_BTF (1U<<2)

//ADXL245
#define DEVID_R (0X00)
#define DATA_FORMAT_R (0x31)
#define POWER_CTL_R (0X2D)
#define DATA_START_ADDR (0X32)
#define DEVICE_ADDR (0X53)
#define FOUR_G (0X01)
#define RESET (0X00)
#define SET_MEASURE_B (0X08)

#define SR2_BUSY  (1U<<1)

uint8_t data_rec[6];
uint16_t x, y, z;
float	xg, yg, zg;
char data;

void I2C1_Init(void);
void I2C1_ByteRead(char saddr, char maddr, char* data);
void I2C1_BrustRead(char saddr, char maddr, int n, char* data);
void I2C1_BrustWrite(char saddr, char maddr, int n, char* data);
void adx1_read_value(uint8_t reg);
void adx1_read_address(uint8_t reg);
void adx1_write(uint8_t reg, char value);
void adxl_init(void);

void adxl_init(void) {
	I2C1_Init();
	adx1_read_address(DEVID_R);
	adx1_write(POWER_CTL_R, FOUR_G);

	//RESET ALL THE BITS
	adx1_write(POWER_CTL_R, RESET);
	adx1_write(POWER_CTL_R, SET_MEASURE_B);
}

void adx1_write(uint8_t reg, char value){
	char data[1];
	data[0]=value;
	I2C1_BrustWrite(DEVICE_ADDR, reg, 1, &data);
}

void adx1_read_address(uint8_t reg){
	I2C1_ByteRead(DEVICE_ADDR, reg, &data);
}

void adx1_read_value(uint8_t reg){
	I2C1_BrustRead(DEVICE_ADDR, reg, 6, (char*)data_rec);
}

void I2C1_BrustWrite(char saddr, char maddr, int n, char* data){

	volatile int tmp;

	//Wait Until Bus is not busy
	while(I2C1->SR2 & (SR2_BUSY)){}

	//Generate Start Condition
	I2C1->CR1 |= CR1_START;

	//Wait until the Start Flag is set
	while(!(I2C1->SR1 & (SR1_SB))){}

	//Transmit Slave address +write (0)
	I2C1->DR = saddr<<1;

	//Wait until the Address Flag is set
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	//Clear the Address Flag
	tmp = I2C1->SR2;

	//Send memory address
	I2C1->DR = maddr;

	//Wait until the transmitter is empty
	while(!(I2C1->SR1 & (SR1_TXE))){}

	for(int i=0; i<n;i++){
		//Wait until the transmitter is empty
		while(!(I2C1->SR1 & (SR1_TXE))){}
		I2C1->DR = *data++;

		//Generate stop after data receive
		while(!(I2C1->SR1 & (SR1_BTF))){}
		//Generate Stop
		I2C1->CR1 |= CR1_STOP;
	}
}

void I2C1_BrustRead(char saddr, char maddr, int n, char* data){

	volatile int tmp;

	//Wait Until Bus is not busy
	while(I2C1->SR2 & (SR2_BUSY)){}

	//Generate Start Condition
	I2C1->CR1 |= CR1_START;

	//Wait until the Start Flag is set
	while(!(I2C1->SR1 & (SR1_SB))){}

	//Transmit Slave address +write (0)
	I2C1->DR = saddr<<1;

	//Wait until the Address Flag is set
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	//Clear the Address Flag
	tmp = I2C1->SR2;

	//Wait until the transmitter is empty
	while(!(I2C1->SR1 & (SR1_TXE))){}

	//Send memory address
	I2C1->DR = maddr;

	//Wait until the transmitter is empty
	while(!(I2C1->SR1 & (SR1_TXE))){}

	//Generate Restart
	I2C1->CR1 |= CR1_START;

	//Wait until the Address Flag is set
	while(!(I2C1->SR1 & (SR1_SB))){}

	//Transmit Slave address +Read (1)
	I2C1->DR = saddr<<1;

	//Wait until the Address Flag is set
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	//Clear the Address Flag
	tmp = I2C1->SR2;

	//Enable Acknowledgment
	I2C1->CR1 |= CR1_ACK;

	while(n > 0U){
		if(n==1U){
			//Generate Stop
			I2C1->CR1 |= CR1_STOP;

			//Wait Until RXNE Flag is Set
			while(!(I2C1->SR1 & (SR1_RXNE))){}

			//Read data from Data Register
			*data++ = I2C1->DR;

			break;
		}
		else {
			//Wait Until RXNE Flag is Set
			while(!(I2C1->SR1 & (SR1_RXNE))){}

			//Read data from Data Register
			*data++ = I2C1->DR;

			n--;
		}
	}
}


void I2C1_ByteRead(char saddr, char maddr, char* data){

	volatile int tmp;

	//Wait Until Bus is not busy
	while(I2C1->SR2 & (SR2_BUSY)){}

	//Generate Start Condition
	I2C1->CR1 |= CR1_START;

	//Wait until the Start Flag is set
	while(!(I2C1->SR1 & (SR1_SB))){}

	//Transmit Slave address +write (0)
	I2C1->DR = saddr<<1;

	//Wait until the Address Flag is set
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	//Clear the Address Flag
	tmp = I2C1->SR2;

	//Send memory address
	I2C1->DR = maddr;

	//Wait until the transmitter is empty
	while(!(I2C1->SR1 & (SR1_TXE))){}

	//Generate Restart
	I2C1->CR1 |= CR1_START;

	//Wait until the Start Flag is set
	while(!(I2C1->SR1 & (SR1_SB))){}

	//Transmit Slave address +Read (1)
	I2C1->DR = saddr<<1;

	//Wait until the Address Flag is set
	while(!(I2C1->SR1 & (SR1_ADDR))){}

	//Disable Acknowledgment
	I2C1->CR1 &= ~(CR1_ACK);

	//Clear the Address Flag
	tmp = I2C1->SR2;

	//Generate Stop
	I2C1->CR1 |= CR1_STOP;

	//Wait Until RXNE Flag is Set
	while(!(I2C1->SR1 & (SR1_RXNE))){}

	//Read data from Data Register
	*data++ = I2C1->DR;
}

void I2C1_Init(void){
//	1) Enable the clock access to PORT B -12C1
	RCC->AHB1ENR |= GPIOBEN;
//	RCC->AHB1ENR |= (1U<<1);
	RCC->APB1ENR |= (1U<<21);

//	2) SET PB8 AND PB9 to alternate function
	GPIOB->MODER &= ~(1U<<16); //to set zero
	GPIOB->MODER |= (1U<<17); //to set one
	GPIOB->MODER &= ~(1U<<18); //to set zero
	GPIOB->MODER |= (1U<<19); //to set one

//	3) SET OUTPUTTYPE as OPEN DRAIN
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);

//	4) SET Internal PULLUP FOR BOTH
	GPIOB->PUPDR |= (1U<<16); //to set zero
	GPIOB->PUPDR &= ~(1U<<17); //to set one
	GPIOB->PUPDR |= (1U<<18); //to set zero
	GPIOB->PUPDR &= ~(1U<<19); //to set one

//	5) SET THE ALTERNATE FUNCTION TYPE FOR I2C
	GPIOB->AFR[1] &= ~(1U<<0);
	GPIOB->AFR[1] &= ~(1U<<1);
	GPIOB->AFR[1] |= (1U<<2);
	GPIOB->AFR[1] &= ~(1U<<3);
	GPIOB->AFR[1] &= ~(1U<<4);
	GPIOB->AFR[1] &= ~(1U<5);
	GPIOB->AFR[1] |= (1U<<6);
	GPIOB->AFR[1] &= ~(1U<<7);

//	6) ENTER TO RESET MODE
	I2C1->CR1 |= (1U<<15);

//	7) COME OUR OF THE RESET MODE
	I2C1->CR1 &= ~(1U<<15);

//	8) Set the Peripheral Clock Frequency(16 mhz)
	I2C1->CR2 |= (1U<<4);

//	9)Set the Standard Mode (12C_100KHZ)
	I2C1->CCR = I2C_100KHZ;
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	//Enable I2C1
	I2C1->CR1 |= (1U<<0); //for peripheral enable
}

int main(void)
{
	adxl_init();
	while(1){
		adx1_read_value(DATA_START_ADDR);
		x = ((data_rec[1]<<8) | data_rec[0]);
		y = ((data_rec[3]<<8) | data_rec[2]);
		z = ((data_rec[5]<<8) | data_rec[4]);

		xg = (x*0.0078);
		yg = (y*0.0078);
		zg = (z*0.0078);
	}
	for(;;);
}
