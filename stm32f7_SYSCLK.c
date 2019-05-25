#include "stm32f7_SYSCLK.h"
#include "reg.h"


int _init_SYSCLK(SYSCLKtype **self){
	if ( NULL == (*self=malloc(sizeof(SYSCLKtype)))) return -1;

    (*self)->clksrc="PLL";
    (*self)->freq=216;
    //method
    (*self)->SYSCLK_config=SYSCLK_config_imp;
    
    return 0;
}


void SYSCLK_config_imp(SYSCLKtype *self){

	
	//enable HSE
	SET_BIT(RCC_BASE + RCC_CR_OFFSET, HSEON_BIT);

	//wait until HSE clock is stable.
	while ( READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSERDY_BIT) != 1)
		;
	
	//set pll 
	SET_BIT(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLSRC_BIT); //use HSE for PLL source

	//f_vco=f_pll_input * (N/M)
    //f_pll_output=f_vco / P
    //
	//f_HSE = 8 MHz
	//
	//N = 168
	//M = 4
	//
	//f_VCO = 8 * 168 / 4 = 168 * 2
	//
	//P = 2
	//
	//f_PLL_out = 168
	
    //set P  00->2 ,01->4 ,10->6 ,11->8
	WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLP_1_BIT, PLLP_0_BIT, 0b00);
	//set N  50~432
    WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLN_8_BIT, PLLN_0_BIT, 168);
	//set M  2~63
    WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLM_5_BIT, PLLM_0_BIT, 4);


	//enable pll
	SET_BIT(RCC_BASE + RCC_CR_OFFSET, PLLON_BIT);

	//wait until PLL clock is ready.
	while (READ_BIT(RCC_BASE + RCC_CR_OFFSET, PLLRDY_BIT) != 1)
		;
	
	//enable flash prefetch buffer
	SET_BIT(FLASH_BASE + FLASH_ACR_OFFSET, PRFTEN_BIT);

	//set flash wait state = 5
	WRITE_BITS(FLASH_BASE + FLASH_ACR_OFFSET,LATENCY_2_BIT,LATENCY_0_BIT,0b101);

	//use pll
	WRITE_BITS(RCC_BASE + RCC_CFGR_OFFSET, SW_1_BIT, SW_0_BIT, 0b10);
	//SET_BIT(RCC_BASE + RCC_CFGR_OFFSET,SW_1_BIT);
	//CLEAN_BIT(RCC_BASE + RCC_CFGR_OFFSET,SW_0_BIT);

	//wait
	while ( (READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_1_BIT) != 1)&&(READ_BIT(RCC_BASE + RCC_CFGR_OFFSET, SWS_0_BIT) != 0) )
		;
	


}





