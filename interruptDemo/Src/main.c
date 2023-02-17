#include <stdint.h>
#include <stdio.h>

#define USART2_IRQNO 38


int main(void)
{
	uint32_t *pNVIC_ISPR1 = (uint32_t*)0xe000e204;
	uint32_t *pNVIC_ISER1 = (uint32_t*)0xe000e104;

	//to set
	*pNVIC_ISPR1 |= (1<<(USART2_IRQNO%32));
	*pNVIC_ISER1 |= (1<<(USART2_IRQNO%32));
    /* Loop forever */
	for(;;);
}

void USART2_IRQHandler(void){
	printf("In USART2");
}
