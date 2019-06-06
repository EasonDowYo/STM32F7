#include <stdint.h>
#include "reg.h"
#include "malloc.h"
#include "stm32f7_GPIO.h"
#include "stm32f7_SYSCLK.h"
//#include "stm32f7_USART.h"





int main(void)
{
	SYSCLKtype *clock=NULL;
	init_sysclk(&clock);
	clock->SYSCLK_config(clock);

	REG(SYST_RVR)=0xFFFFFFFF;
	REG(SYST_CVR)=0x00000000;
	REG(SYST_CSR)=0x00000007;


	/*
	USARTtype *usart6=NULL;
	init_usart(&usart6);
	usart6->usart_config(usart6);
	usart6_send_char('H');*/
	while(1)
        ;
    
}


void systick_handler(void){
	GPIOtype *PI1=NULL;
	init_gpio(&PI1);  // OUTPUT medium
	PI1->port=GPIO_PORTI;
	PI1->pin=1;
	PI1->mode=OUTPUT;
	PI1->IO_config(PI1);
	PI1->blink_ct(PI1);
    free(PI1);
}


/*
void exti0_handler(void)
{
	blink_count(LED_ORANGE, 10);

	//clear pending
	SET_BIT(EXTI_BASE + EXTI_PR_OFFSET, 0);
}
*/

