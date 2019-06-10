#include "stm32f7_SYSCLK.h"
#include "stm32f7_GPIO.h"
#include "reg.h"
#include <stdint.h>
#include "malloc.h"
int system_clock;
int init_sysclk(SYSCLKtype **self){
	if ( NULL == (*self=malloc(sizeof(SYSCLKtype)))) return -1;

	(*self)->clksrc=PLL;
	(*self)->pll_n=432;
    (*self)->pll_m=25;
    (*self)->pll_p=2;
	//method
	(*self)->SYSCLK_config=SYSCLK_config_imp;
    
    return 0;
}


void SYSCLK_config_imp(SYSCLKtype *self){
	

/*
    char *src1="HSI";//16M
    char *src2="HSE";//24M
    char *src3="PLL";
    */
    /*
	while (   *(self->clksrc++)== *(src1++)   ){//If characters are equal continue
        if(   (*self->clksrc=='\0')&&(*src1!='\0')   ){
            break;//judge lenth of string
        }else if (   (*self->clksrc!='\0')&&(*src1=='\0')   ){
        	break;//judge lenth of string
    	}else if(   (*self->clksrc=='\0')&&(*src1=='\0')   ){
            SRC=1;
            break;
        }
    }
	while (   *(self->clksrc++)== *(src2++)   ){
        if(   (*self->clksrc=='\0')&&(*src2!='\0')   ){
            break;//judge lenth of string
        }else if (   (*self->clksrc!='\0')&&(*src2=='\0')   ){
        	break;
        }else if(   (*self->clksrc=='\0')&&(*src2=='\0')   ){
            SRC=2;
            break;
        }
    }
	while (   *(self->clksrc++)== *(src3++)   ){
        if(   (*self->clksrc=='\0')&&(*src3!='\0')   ){
            break;//judge lenth of string
        }else if (   (*self->clksrc!='\0')&&(*src3=='\0')   ){
        	break;
        }else if(   (*self->clksrc=='\0')&&(*src3=='\0')   ){
            SRC=3;
            break;
        }
    }*/
	
    switch(self->clksrc){
        case HSI://HSI
		    SET_BIT(RCC_BASE + RCC_CR_OFFSET, HSION_BIT);
	        while ( READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSIRDY_BIT) != 1)
			        ;

	    	//use HSI
	    	WRITE_BITS(RCC_BASE + RCC_CFGR_OFFSET, SW_1_BIT, SW_0_BIT, 0b00);

	    	//wait SWS
	    	while ( (READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_1_BIT) != 0)&&(READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_0_BIT) != 0) )
	    		;
		system_clock=16;
            break;
        case HSE://HSE
	        SET_BIT(RCC_BASE + RCC_CR_OFFSET, HSEON_BIT);
	        while ( READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSERDY_BIT) != 1)
			        ;

	    	//use HSE
	    	WRITE_BITS(RCC_BASE + RCC_CFGR_OFFSET, SW_1_BIT, SW_0_BIT, 0b01);

	    	//wait SWS
	    	while ( (READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_1_BIT) != 0)&&(READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_0_BIT) != 1) )
	    		;
		system_clock=25;
            break;
        case PLL://PLL
		
	        //enable clock source
	        SET_BIT(RCC_BASE + RCC_CR_OFFSET, HSEON_BIT);
	        //wait until clock source is stable
	        while ( READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSERDY_BIT) != 1)
			        ;//check
		SET_BIT(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLSRC_BIT);//use HSE for PLL source

		if(self->pll_n<2) self->pll_n=2;
		if(self->pll_n>432) self->pll_n=432;
		if(self->pll_m<2) self->pll_m=2;
		if(self->pll_m>63) self->pll_m=63;

		switch(self->pll_p){ //set P  00->2 ,01->4 ,10->6 ,11->8
                case 2: 
                    WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLP_1_BIT, PLLP_0_BIT, 0b00 );
                    break;
                case 4:
                    WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLP_1_BIT, PLLP_0_BIT, 0b01 );
                    break;
                case 6: 
                    WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLP_1_BIT, PLLP_0_BIT, 0b10 );
                    break;
                case 8:
                    WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLP_1_BIT, PLLP_0_BIT, 0b11 );
                    break;
		}//end switch pll_p

		//set N  50~432
		WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLN_8_BIT, PLLN_0_BIT, self->pll_n );
		//set M  2~63
		WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLM_5_BIT, PLLM_0_BIT, self->pll_m );

	        //f_HSE = 16 MHz
	    	//N=432 M=25  ,   f_VCO = 25*432/25   , f_PLL_out = 1*432/2
            //

		
        system_clock=25* (( (self->pll_n)/(self->pll_m) ))/(self->pll_p);
	    	//enable pll
	    	SET_BIT(RCC_BASE + RCC_CR_OFFSET, PLLON_BIT);

	     	//wait until PLL clock is ready.
		    while (READ_BIT(RCC_BASE + RCC_CR_OFFSET, PLLRDY_BIT) != 1)
	    		;//check
	
	    	//enable flash prefetch buffer
	    	SET_BIT(FLASH_BASE + FLASH_ACR_OFFSET, PRFTEN_BIT);


		if (system_clock>30 && system_clock<=60)
			WRITE_BITS(FLASH_BASE+FLASH_ACR_OFFSET,LATENCY_2_BIT,LATENCY_0_BIT,0b001);
		if (system_clock>60 && system_clock<=90)
			WRITE_BITS(FLASH_BASE+FLASH_ACR_OFFSET,LATENCY_2_BIT,LATENCY_0_BIT,0b010);
		if (system_clock>90 && system_clock<=120)
			WRITE_BITS(FLASH_BASE+FLASH_ACR_OFFSET,LATENCY_2_BIT,LATENCY_0_BIT,0b011);
		if (system_clock>120 && system_clock<=150)
			WRITE_BITS(FLASH_BASE+FLASH_ACR_OFFSET,LATENCY_2_BIT,LATENCY_0_BIT,0b100);
		if (system_clock>150 && system_clock<=180)
			WRITE_BITS(FLASH_BASE+FLASH_ACR_OFFSET,LATENCY_2_BIT,LATENCY_0_BIT,0b101);
		if (system_clock>210 && system_clock<=216)
			WRITE_BITS(FLASH_BASE+FLASH_ACR_OFFSET,LATENCY_2_BIT,LATENCY_0_BIT,0b111);



	    	//use pll
	    	WRITE_BITS(RCC_BASE + RCC_CFGR_OFFSET, SW_1_BIT, SW_0_BIT, 0b10);

	    	//wait
	    	while ( (READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_1_BIT) != 1)&&(READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_0_BIT) != 0) )
	    		;

            break;//break PLL case
    }//end switch SRC

	


}//end SYSCLK_config_imp


void op_sysclk(unsigned int div){
	//RCC
	SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO1_1_BIT);
	SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO1_0_BIT);

	if (div == 1){
		CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO1PRE_2_BIT);
    }
	else if (div >= 2 && div <= 5)
	{
		SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO1PRE_2_BIT);
		REG(RCC_BASE + RCC_CFGR_OFFSET) = (REG(RCC_BASE + RCC_CFGR_OFFSET) & ~(((uint32_t)0b11) << (MCO1PRE_0_BIT))) | ((uint32_t)(div - 2) << (MCO1PRE_0_BIT));
	}
	else{
		while(1)
			;
    }
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));

	//GPIO MODER
	SET_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_1_BIT(8));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_0_BIT(8));

	//Output push-pull
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_OTYPER_OFFSET, OTy_BIT(8));

	//OSPEEDR9 = 11 => Very high speed
	SET_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(8));
	SET_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(8));

	//PUPDR9 = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(8));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(8));
}


