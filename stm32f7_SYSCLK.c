#include "stm32f7_SYSCLK.h"
#include "reg.h"
#include <stdint.h>
#include "malloc.h"
int init_sysclk(SYSCLKtype **self){
	if ( NULL == (*self=malloc(sizeof(SYSCLKtype)))) return -1;

    (*self)->clksrc="PLL";
    (*self)->freq=216;
    //method
    (*self)->SYSCLK_config=SYSCLK_config_imp;
    
    return 0;
}


void SYSCLK_config_imp(SYSCLKtype *self){
	
    int WS,SRC=0;
	char *src1="HSI";//16M
    char *src2="HSE";//24M
    char *src3="PLL";
    
    
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
    }
    
    switch(SRC){
        case 1://HSI
		    SET_BIT(RCC_BASE + RCC_CR_OFFSET, HSION_BIT);
	        while ( READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSIRDY_BIT) != 1)
			        ;

	    	//use HSI
	    	WRITE_BITS(RCC_BASE + RCC_CFGR_OFFSET, SW_1_BIT, SW_0_BIT, 0b00);

	    	//wait SWS
	    	while ( (READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_1_BIT) != 0)&&(READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_0_BIT) != 0) )
	    		;

            break;
        case 2://HSE
	        SET_BIT(RCC_BASE + RCC_CR_OFFSET, HSEON_BIT);
	        while ( READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSERDY_BIT) != 1)
			        ;

	    	//use HSE
	    	WRITE_BITS(RCC_BASE + RCC_CFGR_OFFSET, SW_1_BIT, SW_0_BIT, 0b01);

	    	//wait SWS
	    	while ( (READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_1_BIT) != 0)&&(READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_0_BIT) != 1) )
	    		;

            break;
        case 3://PLL
	        //enable clock source
	        SET_BIT(RCC_BASE + RCC_CR_OFFSET, HSION_BIT);
	        //wait until clock source is stable
	        while ( READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSIRDY_BIT) != 1)
			        ;

	        CLEAR_BIT(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLSRC_BIT);//use HSI for PLL source
	        
            //set P  00->2 ,01->4 ,10->6 ,11->8
		    WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLP_1_BIT, PLLP_0_BIT, 0b00);
		    //set N  50~432
    	    WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLN_8_BIT, PLLN_0_BIT, self->freq);
            //set M  2~63
	        WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLM_5_BIT, PLLM_0_BIT, 8);
	        //f_HSI = 16 MHz
	    	//N=216 M=8  ,   f_VCO = 16*216/8   , f_PLL_out = 2*216/2

	    	//enable pll
	    	SET_BIT(RCC_BASE + RCC_CR_OFFSET, PLLON_BIT);

	     	//wait until PLL clock is ready.
		    while (READ_BIT(RCC_BASE + RCC_CR_OFFSET, PLLRDY_BIT) != 1)
	    		;
	
	    	//enable flash prefetch buffer
	    	SET_BIT(FLASH_BASE + FLASH_ACR_OFFSET, PRFTEN_BIT);
	        if( self->freq==216 ){
                WS=7;
            }else if( self->freq==128 ){
                WS=4;
            }
                
	    	//set flash wait state = 7
	    	WRITE_BITS(FLASH_BASE + FLASH_ACR_OFFSET,LATENCY_2_BIT,LATENCY_0_BIT,WS);

	    	//use pll
	    	WRITE_BITS(RCC_BASE + RCC_CFGR_OFFSET, SW_1_BIT, SW_0_BIT, 0b10);

	    	//wait
	    	while ( (READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_1_BIT) != 1)&&(READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_0_BIT) != 0) )
	    		;

            break;//break PLL case
    }//end switch SRC

	


}//end SYSCLK_config_imp





