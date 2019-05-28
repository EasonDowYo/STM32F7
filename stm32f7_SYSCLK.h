#ifndef SYSCLK_H
#define SYSCLK_H





/////      sysclk register     /////
//FLASH
#define FLASH_BASE 0x40023C00

#define FLASH_ACR_OFFSET 0x00
#define PRFTEN_BIT 8
#define LATENCY_2_BIT 2
#define LATENCY_0_BIT 0

//RCC
#define RCC_BASE 0x40023800


//RCC_CR
#define RCC_CR_OFFSET 0x00
#define PLLRDY_BIT 25
#define PLLON_BIT 24
#define HSERDY_BIT 17
#define HSEON_BIT 16
#define HSIRDY_BIT 1
#define HSION_BIT 0


#define RCC_PLLCFGR_OFFSET 0x04

#define PLLSRC_BIT 22

#define PLLP_1_BIT 17
#define PLLP_0_BIT 16

#define PLLN_8_BIT 14
#define PLLN_0_BIT 6

#define PLLM_5_BIT 5
#define PLLM_0_BIT 0

#define RCC_CFGR_OFFSET 0x08   //p165
#define MCO2_1_BIT 31
#define MCO2_0_BIT 30

#define MCO2PRE_2_BIT 29
#define MCO2PRE_0_BIT 27

#define SWS_1_BIT 3
#define SWS_0_BIT 2

#define SW_1_BIT 1
#define SW_0_BIT 0

#define RCC_APB2ENR_OFFSET 0x44
#define SYSCFG_EN_BIT 14


//EXTI
#define EXTI_BASE 0x40013C00

#define EXTI_IMR_OFFSET 0x00

#define EXTI_RTSR_OFFSET 0x08

#define EXTI_FTSR_OFFSET 0x0C

#define EXTI_PR_OFFSET 0x14

//SYSCFG
#define SYSCFG_BASE 0x40013800   //p66

#define SYSCFG_EXTICR1_OFFSET 0x08

#define EXTI0_3_BIT 3
#define EXTI0_0_BIT 0

//NVIC
#define NVIC_ISER_BASE 0xE000E100

#define NVIC_ISERn_OFFSET(n) (0x00 + 4 * (n))



typedef struct SYSCLK SYSCLKtype;
typedef struct (*SYSCLKfunc)(SYSCLKtype *self);

struct SYSCLK{
	char *clksrc;
    int freq;

    SYSCLKfunc SYSCLK_config;

}

int _init_SYSCLK(SYSCLKtype **self);

void SYSCLK_config_imp(SYSCLKtype *self);






#endif
