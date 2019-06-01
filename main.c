#include <stdint.h>
#include "reg.h"
#include "stm32f7_GPIO.h"
#include "stm32f7_SYSCLK.h"

int main(void)
{
	GPIOtype *PI1=NULL;
    init_GPIO(&PI1);  // OUTPUT medium
    PI1->port=GPIO_PORTI;
    PI1->pin=1;
    PI1->mode=OUTPUT;
	PI1->IO_config(PI1);




    
}
/*
void exti0_handler(void)
{
	blink_count(LED_ORANGE, 10);

	//clear pending
	SET_BIT(EXTI_BASE + EXTI_PR_OFFSET, 0);
}
*/

