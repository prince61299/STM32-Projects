#include <stdint.h>
#include <stdio.h>

#define SIZE_TASK_STACK 1024U
#define SIZE_SCHED_STACK 1024U
#define MAX_TASKS 5U
#define SRAM_START 0x20000000U
#define SIZE_SRAM ((128)*(1024))
#define SRAM_END ((SRAM_START)+(SIZE_SRAM))

#define T1_STACK_START SRAM_END
#define T2_STACK_START ((SRAM_END)-(1*(SIZE_TASK_STACK)))
#define T3_STACK_START ((SRAM_END)-(2*(SIZE_TASK_STACK)))
#define T4_STACK_START ((SRAM_END)-(3*(SIZE_TASK_STACK)))
#define IDLE_STACK_START ((SRAM_END)-(4*(SIZE_TASK_STACK)))
#define SCHED_STACK_START ((SRAM_END)-(5*(SIZE_TASK_STACK)))

#define HSI_CLOCK 16000000
#define SYSTICK_TIM_CLOCK HSI_CLOCK
#define TASK_READY_STATE 0X00
#define TASK_BLOCKED_STATE 0XFF
#define DUMMY_XPSR 0X01000000U

uint32_t TICK_HZ = 1000;
uint32_t current_task = 1;

void init_systick_timer(uint32_t TICK_HZ);
void Task1_Handler(void);
void Task2_Handler(void);
void Task3_Handler(void);
void Task4_Handler(void);
void enable_processor_faults(void);
uint32_t get_psp_value(void);
uint32_t save_psp_value(uint32_t current_psp_value);
void update_next_task(void);
__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack);
void init_task_stack(void);
void idle_task(void);
__attribute__((naked)) void switch_sp_to_psp (void);

typedef struct {
	uint32_t psp_value;
	uint8_t current_state;
	void(*task_handler)(void);
}TCB_t;

TCB_t user_tasks[MAX_TASKS];

uint32_t get_psp_value(void){
	return user_tasks[current_task].psp_value;
}

uint32_t save_psp_value(uint32_t current_psp_value){
	user_tasks[current_task].psp_value = current_psp_value;
}

void update_next_task(void) {
	current_task++;
	current_task %= MAX_TASKS;
}

//Assembly code
__attribute__((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack){
	__asm volatile("MSR MSP, %0" : : "r" (sched_top_of_stack) :);
	__asm volatile("BX LR");
}

__attribute__((naked)) void switch_sp_to_psp (void){
	//1. initialize the PSP with TASK1 stack start address
	//get the value of psp of current_task
	//Preserve LR which connects back to main() using PUSH
	__asm volatile ("PUSH {LR}");
	//Get the PSP Value of the Current Task
	__asm volatile ("BL get_psp_value");
	//Initialize PSP
	__asm volatile ("MSR PSP, R0");
	//Retrieve LR Value using POP
	__asm volatile ("POP {LR}");
	//Change SP to PSP using CONTROL register
	__asm volatile ("MOV R0,#0X02");
	__asm volatile ("MSR CONTROL, R0");
	//Return to Main Function
	__asm volatile ("BX LR");
}
// Initialized stack
void init_task_stack(void){
	user_tasks[0].current_state = TASK_READY_STATE;
	user_tasks[1].current_state = TASK_READY_STATE;
	user_tasks[2].current_state = TASK_READY_STATE;
	user_tasks[3].current_state = TASK_READY_STATE;
	user_tasks[4].current_state = TASK_READY_STATE;

	user_tasks[0].psp_value = IDLE_STACK_START;
	user_tasks[1].psp_value = T1_STACK_START;
	user_tasks[2].psp_value = T2_STACK_START;
	user_tasks[3].psp_value = T3_STACK_START;
	user_tasks[4].psp_value = T4_STACK_START;

	user_tasks[0].task_handler = idle_task;
	user_tasks[1].task_handler = Task1_Handler;
	user_tasks[2].task_handler = Task2_Handler;
	user_tasks[3].task_handler = Task3_Handler;
	user_tasks[4].task_handler = Task4_Handler;

	//Initialized some dummy data
	uint32_t *pPSP;
	for(int i=0; i<MAX_TASKS; i++){
		pPSP=(uint32_t*)user_tasks[i].psp_value;
		*pPSP--;
		*pPSP=DUMMY_XPSR;
		*pPSP--;
		*pPSP=(uint32_t)user_tasks[i].task_handler;
		*pPSP--;
		*pPSP=0xfffffffD;

		for(int j=0; j<13; j++){
			*pPSP--;
			*pPSP=0; //dummy data of zero
		}
		user_tasks[i].psp_value = (uint32_t)pPSP;
	}
}

void idle_task(void){
	while(1);
}

void Task1_Handler(void){

	while(1){
		printf("Executing Task 1 \n");
	}
}
void Task2_Handler(void){

	while(1){
		printf("Executing Task 1 \n");
	}
}
void Task3_Handler(void){

	while(1){
		printf("Executing Task 1 \n");
	}
}
void Task4_Handler(void){

	while(1){
		printf("Executing Task 1 \n");
	}
}

void init_systick_timer(uint32_t TICK_HZ){

	uint32_t *pSTCSR = (uint32_t*)0xe000e010;
	uint32_t *pSTRVR = (uint32_t*)0xe000e014;
	uint32_t Count_Value = (SYSTICK_TIM_CLOCK/TICK_HZ)-1;

	//clear the value of the Reload value Register
	*pSTRVR &= ~(0x00FFFFFF);

	// Load the Reload value
	*pSTRVR |= Count_Value;

	//Enable the Exception
	*pSTCSR |= (1<<1);// exception
	*pSTCSR |= (1<<2); //clock source
	*pSTCSR |= (1<<0); //counter

}


void enable_processor_faults(void) {

	//enable the fault
	uint32_t *pSHCSR = (uint32_t*)0xe000ed24;

	//enable the fault handler
	*pSHCSR |= (1<<18); //usage fault
	*pSHCSR |= (1<<17); //Bus fault
	*pSHCSR |= (1<<16); //Mem fault
}

int main(void)
{
	printf("Executing Main \n");
	enable_processor_faults();
	init_scheduler_stack(SCHED_STACK_START);
	init_task_stack();

	init_systick_timer(TICK_HZ);
	switch_sp_to_psp();
	Task1_Handler();

	for(;;);
}

__attribute__((naked)) SysTick_Handler(void){

	printf("Executing Exception \n");
	//Get current running task's PSP value
	__asm volatile("MRS R0, PSP");
	//Using that PSP value store SF2( R4 to R11)
	__asm volatile("STMDB R0!, {R4-R11}");
	//Save the LINK Register Using PUSH Operations
	__asm volatile ("PUSH {LR}");
	//Save the current value of PSP
	__asm volatile ("BL save_psp_value");
	//Decide next task to run
	__asm volatile ("BL update_next_task");
	//Get its past PSP value
	__asm volatile ("BL get_psp_value");
	//Using that PSP value retrieve SF2(R4 to R11)
	__asm volatile("LDMIA R0!, {R4-R11}");
	//Update PSP
	__asm volatile("MSR PSP, R0");
	//Retrieve the LINK Register Using POP Operations
	__asm volatile ("POP {LR}");
	//Update PSP and exit
	__asm volatile ("BX LR");
}

void HardFault_Handler(void){
	printf("Exception:Hard Fault \n");
	while(1);
}

void MemManage_Handler(void){
	printf("Exception:Mem-Manage \n");
	while(1);
}

void BusFault_Handler(void){
	printf("Exception:Bus Fault \n");
	while(1);
}

void UsageFault_Handler(void){
	printf("Exception:Usage Fault \n");
	while(1);
}
