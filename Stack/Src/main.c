#include <stdint.h>
#include <stdio.h>

int function_add(int a, int b, int c, int d);

int function_add(int a, int b, int c, int d) {
	int sum;
	sum = a+b+c+d;
	return sum;
}

// Assembly language program
__attribute__((naked))void change_sp_to_psp(void) {
	__asm volatile(".equ SRAM_END,(0X20000000+(128 *1024))"); //Declare SRAM ending address
	__asm volatile(".equ PSP_START,(SRAM_END-512)");
	__asm volatile("LDR R0, =PSP_START");
	__asm volatile("MSR PSP, R0");
	__asm volatile("MOV R0, 0X02"); //to set control register
	__asm volatile("MSR CONTROL, R0");
	__asm volatile("BX LR");
}

void generate_exception(void) {
	__asm volatile("SVC #0X2");
}

int main(void)
{
	change_sp_to_psp();
	int sum;
	int data =10;
	sum = function_add(1, 2, 3, 4);
	generate_exception();
	return 0;
}

void SVC_Handler(void) {
	printf("in svc handler \n");
}
