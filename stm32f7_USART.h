#ifndef USART_H
#define USART_H

#define number16 16
#define number1e6 1000000


//USART1
#define USART6EN 5

#define USART6_BASE 0x40011400
#define UART7_BASE 0x40007800



#define USART_ISR_OFFSET 0x1c
#define TXE_BIT 7  
#define TC_BIT 6
#define RXNE_BIT 5  
#define ORE_BIT 3  

#define USART_RDR_OFFSET 0x24
#define USART_TDR_OFFSET 0x28

#define USART_BRR_OFFSET 0x0c
#define DIV_MANTISSA_11_BIT 15
#define DIV_MANTISSA_0_BIT 4
#define DIV_FRACTION_3_BIT 3
#define DIV_FRACTION_0_BIT 0


#define USART_CR1_OFFSET 0x00
#define UE_BIT 0    
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

void usart6_send_char(const char ch);

char usart6_receive_char(void);


#endif
