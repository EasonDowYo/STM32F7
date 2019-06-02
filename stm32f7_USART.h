#ifndef USART_H
#define USART_H


//USART1
#define USART1_BASE 0x40011000

#define USART_SR_OFFSET 0x00
#define TXE_BIT 7
#define RXNE_BIT 5
#define ORE_BIT 3

#define USART_DR_OFFSET 0x04

#define USART_BRR_OFFSET 0x08
#define DIV_MANTISSA_11_BIT 15
#define DIV_MANTISSA_0_BIT 4
#define DIV_FRACTION_3_BIT 3
#define DIV_FRACTION_0_BIT 0

#define USART_CR1_OFFSET 0x0C
#define UE_BIT 13
#define RXNEIE_BIT 5
#define TE_BIT 3
#define RE_BIT 2

#define RCC_BASE 0x40023800

#define RCC_AHB1ENR_OFFSET 0x30
#define GPIO_EN_BIT(port) (port)

#define RCC_APB2ENR_OFFSET 0x44
#define SYSCFGEN_BIT 14
#define USART1EN 4



typedef struct USART USARTtype;
typedef void (*USARTfunc)(USARTtype *self);
typedef int (*USARTvalue)(USARTtype *self);
struct USART{
	int number,baud;
	
	USARTvalue usart_config;
};

int init_usart(USARTtype **self);
int usart_config_imp(USARTtype *self);




#endif
