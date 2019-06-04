#include <stdint.h>
#include "malloc.h"
#include "stm32f7_GPIO.h"
#include "reg.h"
/*
   Before enable the GPIO x port, we have to enable the AHB1 bus.
   After enable AHB1,we can set MODER,OTYPER,OSPEEDR and PUPDR register.
*/

void IO_config_imp(GPIOtype *self){
	
    SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT( self->port ));
	//set mode case
	switch(self->mode){
        case 1: // MODER pin 01  Gernaral purpose output mode
   		    CLEAR_BIT(GPIO_BASE( self->port ) + GPIOx_MODER_OFFSET, MODERy_1_BIT(self->pin));
	    	SET_BIT(GPIO_BASE( self->port ) + GPIOx_MODER_OFFSET, MODERy_0_BIT(self->pin));
		break;
        case 2:// MODER pin 10 alternative function mode
   		    SET_BIT(GPIO_BASE(self->port) + GPIOx_MODER_OFFSET, MODERy_1_BIT(self->pin));
	    	CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_MODER_OFFSET, MODERy_0_BIT(self->pin));
		break;
        case 0:// MODER pin 00 input mode
   		    CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_MODER_OFFSET, MODERy_1_BIT(self->pin));
	    	CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_MODER_OFFSET, MODERy_0_BIT(self->pin));
		break;
        case 3:// 
   		    SET_BIT(GPIO_BASE(self->port) + GPIOx_MODER_OFFSET, MODERy_1_BIT(self->pin));
	    	SET_BIT(GPIO_BASE(self->port) + GPIOx_MODER_OFFSET, MODERy_0_BIT(self->pin));            
		break;
    }//end mode case

	//set otyper case
    switch(self->otyper){
        case 0://push-pull
            CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_OTYPER_OFFSET, OTy_BIT(self->pin));
		break;
        case 1://open-drain
			SET_BIT(GPIO_BASE(self->port) + GPIOx_OTYPER_OFFSET, OTy_BIT(self->pin));
		break;
    }//end otyper case
	
    //set ospeed case
    switch(self->ospeed){
        case 0:// low speed
	    	CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(self->pin));
    		CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(self->pin));
		break;
        case 1:// medium speed
	    	CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(self->pin));
    		SET_BIT(GPIO_BASE(self->port) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(self->pin));
		break;
        case 2:// high speed
	    	SET_BIT(GPIO_BASE(self->port) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(self->pin));
    		CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(self->pin));
		break;
        case 3:// very high speed
	    	SET_BIT(GPIO_BASE(self->port) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(self->pin));
    		SET_BIT(GPIO_BASE(self->port) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(self->pin));
		break;
    }//end ospeed case

    //set pupd case
    switch(self->pupd){
        case 0://none PU or PD
    		CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(self->pin));
    		CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(self->pin));
		break;
        case 1://pull-up
    		CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(self->pin));
    		SET_BIT(GPIO_BASE(self->port) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(self->pin));
		break;
        case 2://pull-down
    		SET_BIT(GPIO_BASE(self->port) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(self->pin));
    		CLEAR_BIT(GPIO_BASE(self->port) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(self->pin));
		break;
        case 3://Reserved
    		SET_BIT(GPIO_BASE(self->port) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(self->pin));
    		SET_BIT(GPIO_BASE(self->port) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(self->pin));
		break;
    }//end pupd case
}// end IO_config 

void GPIO_onoff_imp(GPIOtype *self){
    if(self->onoff==1)//set GPIO pin
		SET_BIT(GPIO_BASE(self->port) + GPIOx_BSRR_OFFSET, BSy_BIT(self->pin));
    else if(self->onoff==0)//reset GPIO pin
		SET_BIT(GPIO_BASE(self->port) + GPIOx_BSRR_OFFSET, BRy_BIT(self->pin));
}//end GPIO_onoff

void blink_ct_imp(GPIOtype *self){
	for(int ct=10;ct>0;ct--){
	    SET_BIT(GPIO_BASE(self->port) + GPIOx_BSRR_OFFSET, BSy_BIT(self->pin));
	    for(int i=0;i<100000;i++);
        SET_BIT(GPIO_BASE(self->port) + GPIOx_BSRR_OFFSET, BSy_BIT(self->pin));
	    for(int i=0;i<100000;i++);
    }
}

void blink_imp(GPIOtype *self){
    int ii=0;
	while(ii<10){
	    SET_BIT(GPIO_BASE(self->port) + GPIOx_BSRR_OFFSET, BSy_BIT(self->pin));
	    for(int i=0;i<100000;i++);
        SET_BIT(GPIO_BASE(self->port) + GPIOx_BSRR_OFFSET, BSy_BIT(self->pin));
	    for(int i=0;i<100000;i++);
        
        ii++;
    }
}//end blink_imp

int DRead_imp(GPIOtype *self){
	return READ_BIT(GPIO_BASE(self->port) + GPIOx_IDR_OFFSET , self->pin);
}//end DRead_imp


int init_GPIO(GPIOtype **self){
	if ( NULL == (*self=malloc(sizeof(GPIOtype)))) return -1;
    (*self)->port= GPIO_PORTD;
    (*self)->pin=TEST_LED;
    (*self)->onoff=0;
    //GPIO set
    (*self)->mode=OUTPUT;
    (*self)->otyper=PP;
    (*self)->ospeed=MEDIUM;
    (*self)->pupd=none_PUPD;
    //method
    (*self)->IO_config=IO_config_imp;
    (*self)->GPIO_onoff=GPIO_onoff_imp;
    (*self)->blink_ct=blink_ct_imp;
    (*self)->blink=blink_imp;
    (*self)->DRead=DRead_imp;

    return 0;
}


