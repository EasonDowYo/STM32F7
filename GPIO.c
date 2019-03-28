#include <stdint.h>
#include "GPIO.h"

void pinMode(unsigned int GPIOport,unsigned int pin,int mode)
{
    if(mode==1){
        SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIOport));

	    //MODER led pin = 01 => General purpose output mode
	    CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_MODER_OFFSET, MODERy_1_BIT(pin));
	    SET_BIT(GPIO_BASE(GPIOport) + GPIOx_MODER_OFFSET, MODERy_0_BIT(pin));

    	//OT led pin = 0 => Output push-pull
    	CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_OTYPER_OFFSET, OTy_BIT(pin));

    	//OSPEEDR led pin = 00 => Low speed
    	CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(pin));
    	CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(pin));

    	//PUPDR led pin = 00 => No pull-up, pull-down
    	CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(pin));
    	CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(pin));
    }else if(mode==0){
        SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIOport));

	//MODER led pin = 00 => General purpose input mode
	CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_MODER_OFFSET, MODERy_1_BIT(pin));
	CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_MODER_OFFSET, MODERy_0_BIT(pin));


    	//PUPDR led pin = 00 => No pull-up, pull-down
    	CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(pin));
    	CLEAR_BIT(GPIO_BASE(GPIOport) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(pin));
    }
}

void digitalWrite(unsigned int GPIOport,unsigned int pin,unsigned int mode)
{
    if(mode==1){
        //set GPIOD led pin
		SET_BIT(GPIO_BASE(GPIOport) + GPIOx_BSRR_OFFSET, BSy_BIT(pin));
    }
    else if(mode==0){
        //reset GPIOD led pin
		SET_BIT(GPIO_BASE(GPIOport) + GPIOx_BSRR_OFFSET, BRy_BIT(pin));
    }
}

int digitalRead(unsigned int GPIOport,unsigned int pin)
{

	return READ_BIT(GPIO_BASE(GPIOport) + GPIOx_IDR_OFFSET,pin);

}

void Delay(unsigned int ms)
{
		for (int i = 0; i < (ms*(200+1000)); i++);
}

