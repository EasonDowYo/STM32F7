#include <stdint.h>
#include "reg.h"

#include "stm32f7_GPIO.h"

#include "stm32f7_USART.h"

#include "malloc.h"

extern int system_clock;

int init_usart(USARTtype **self){

	if( NULL==(*self=malloc(sizeof(USARTtype))) ){return -1;}
	(*self)->number=6;
	(*self)->baud=115200;
	
	(*self)->usart_config=usart_config_imp;

	return 0;
}

int usart_config_imp(USARTtype *self){
	
    
    if (self->number!=6 || self->number!=7){return -1;}
    if (self->number==6){
	//RCC EN GPIO
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTC));

	//MODER => 10
	SET_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_MODER_OFFSET, MODERy_1_BIT(6));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_MODER_OFFSET, MODERy_0_BIT(6));

	SET_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_MODER_OFFSET, MODERy_1_BIT(7));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_MODER_OFFSET, MODERy_0_BIT(7));

	//OT => 0
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OTYPER_OFFSET, OTy_BIT(6));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OTYPER_OFFSET, OTy_BIT(7));

	//OSPEEDR => 10
	SET_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(6));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(6));

	SET_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(7));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(7));

	//PUPDR = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(6));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(6));

	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(7));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(7));


/*
    	//usart6  port c;pin 6,7
		GPIOtype *TX6=NULL;
        init_gpio(&TX6);
    	TX6->port = GPIO_PORTC;
        TX6->pin = 6;
        TX6->mode = AF;
        TX6->ospeed = HIGH;
	    TX6->IO_config(TX6);

    	GPIOtype *RX6=NULL;
        init_gpio(&RX6);
    	RX6->port = GPIO_PORTC;
        RX6->pin = 7;
        RX6->mode = AF;
        TX6->ospeed = HIGH;
	    RX6->IO_config(RX6);
*/
        //GPIO
        WRITE_BITS(GPIO_BASE(GPIO_PORTC) +GPIOx_AFRL_OFFSET , AFRLy_3_BIT(6) ,AFRLy_0_BIT(6) ,8);
    	WRITE_BITS(GPIO_BASE(GPIO_PORTC) +GPIOx_AFRL_OFFSET , AFRLy_3_BIT(7) ,AFRLy_0_BIT(7) ,8);

	    //RCC EN USART1
	    SET_BIT(RCC_BASE + RCC_APB2ENR_OFFSET , USART6EN);

	    //Baud
	    const unsigned int BAUD = self->baud;
    	
        const double USARTDIV = system_clock * number1e6 / number16 / BAUD;
    
    	const uint32_t DIV_MANTISSA = (uint32_t) USARTDIV;
    	const uint32_t DIV_FRACTION = (uint32_t) ((USARTDIV-DIV_MANTISSA)*number16);

        WRITE_BITS(USART6_BASE + USART_BRR_OFFSET, DIV_MANTISSA_11_BIT , DIV_MANTISSA_0_BIT , DIV_MANTISSA);
        WRITE_BITS(USART6_BASE + USART_BRR_OFFSET, DIV_FRACTION_3_BIT , DIV_FRACTION_0_BIT , DIV_FRACTION);   
    	//USART  Configurations
    	SET_BIT(USART6_BASE + USART_CR1_OFFSET , UE_BIT );

    	SET_BIT(USART6_BASE + USART_CR1_OFFSET,TE_BIT);

	SET_BIT(USART6_BASE + USART_CR1_OFFSET,RE_BIT);

        SET_BIT(USART6_BASE + USART_CR1_OFFSET, RXNEIE_BIT );
    	//enable NVIC  IRQ71  m=7n=2               IRQ37 => (m+(32*n))  m=5,n=1
        SET_BIT(NVIC_ISER_BASE+NVIC_ISERn_OFFSET(2),7);
    

    

    }
	return 0;
}// end usart config imp


void usart6_send_char(const char ch)
{
	while( !READ_BIT( USART6_BASE+USART_ISR_OFFSET , TXE_BIT) )
        ;
    REG(USART6_BASE+USART_TDR_OFFSET)=ch; 
}


char usart6_receive_char(void)
{
	while( !READ_BIT( USART6_BASE+USART_ISR_OFFSET , RXNE_BIT) )
        ;
	return (char)REG(USART6_BASE+USART_RDR_OFFSET); 
}


void usart6_handler(void){
	if( READ_BIT(USART6_BASE+USART_ISR_OFFSET , ORE_BIT)==1){
    	for(int i=0; i<1000000;i++)
            ;
        char ch=(char)REG(USART6_BASE + USART_RDR_OFFSET);
        
        if(ch=='\r')
            usart6_send_char('\n');
        usart6_send_char('\n');
        usart6_send_char('~');

    
    }else{
    	char ch=(char)REG(USART6_BASE + USART_RDR_OFFSET) ;
        ch = usart6_receive_char();
        	if(ch=='\r')
                usart6_send_char('\n');
            usart6_send_char(ch);
    
    
    }
}






/*
void usart6_handler(void){


    if (READ_BIT(USART6_BASE + USART_SR_OFFSET , RXNE_BIT)){
		char ch;
		ch = usart6_receive_char();

		if (ch == '\r')
			usart6_send_char('\n');

		usart6_send_char(ch);

    	blink_count(LED_RED,1);
    }
	if (READ_BIT(USART6_BASE + USART_SR_OFFSET , ORE_BIT)){
        char *ch="ORE error\r\n";
		while (*ch != '\0')
			usart6_send_char(*ch++);
	    REG(USART6_BASE+USART_DR_OFFSET);




}
*/
