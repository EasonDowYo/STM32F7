#include <stdint.h>
#include "reg.h"
#include "blink.h"
#include "stm32f7_GPIO.h"
void init_usart6(USARTtype *self)
{
	//PC6 TX  PC7 RX
	GPIOtype *TX6=NULL;
    init_GPIO(&TX6);
	TX6->port = GPIO_PORTC
    TX6->pin = 6;
    TX6->mode = AF;

	GPIOtype *RX6=NULL;
    init_GPIO(&RX6);
	RX6->port = GPIO_PORTC
    RX6->pin = 7;
    RX6->mode = AF;
    //GPIO
    WRITE_BITS(GPIO_BASE(GPIO_PORTB) +GPIOx_AFRL_OFFSET , AFRLy_3_BIT(6) ,AFRLy_0_BIT(6) ,8);
	WRITE_BITS(GPIO_BASE(GPIO_PORTB) +GPIOx_AFRL_OFFSET , AFRLy_3_BIT(7) ,AFRLy_0_BIT(7) ,8);

	//RCC EN USART1
	SET_BIT(RCC_BASE + RCC_APB2ENR_OFFSET , USART1EN);

	//Baud
	const unsigned int BAUD = self->baud;
	const unsigned int SYSCLK_MHZ = 216;
	const double USARTDIV = SYSCLK_MHZ * 1.0e6 / 16 / BAUD;

	const uint32_t DIV_MANTISSA = (uint32_t) USARTDIV;
	const uint32_t DIV_FRACTION = (uint32_t) ((USARTDIV-DIV_MANTISSA)*16);

	//USART  Configurations
	SET_BIT(USART1_BASE + USART_CR1_OFFSET , UE_BIT );

    SET_BIT(USART1_BASE + USART_CR1_OFFSET, RXNEIE_BIT );
	//enable NVIC  IRQ71  IRQ37 => (m+(32*n))  m=5,n=1
    SET_BIT(NVIC_ISER_BASE+NVIC_ISERn_OFFSET(1),5);

	WRITE_BITS(USART1_BASE + USART_BRR_OFFSET, DIV_MANTISSA_11_BIT , DIV_MANTISSA_0_BIT , DIV_MANTISSA);
    WRITE_BITS(USART1_BASE + USART_BRR_OFFSET, DIV_FRACTION_3_BIT , DIV_FRACTION_0_BIT , DIV_FRACTION);

	SET_BIT(USART1_BASE + USART_CR1_OFFSET,TE_BIT);
	SET_BIT(USART1_BASE + USART_CR1_OFFSET,RE_BIT);


}

void usart1_send_char(const char ch)
{
	while( !READ_BIT( USART1_BASE+USART_SR_OFFSET , TXE_BIT) )
        ;
    REG(USART1_BASE+USART_DR_OFFSET)=ch; 
}

char usart1_receive_char(void)
{
	while( !READ_BIT( USART1_BASE+USART_SR_OFFSET , RXNE_BIT) )
        ;
	return (char)REG(USART1_BASE+USART_DR_OFFSET); 
}

void usart1_handler(void){


    if (READ_BIT(USART1_BASE + USART_SR_OFFSET , RXNE_BIT)){
		char ch;
		ch = usart1_receive_char();

		if (ch == '\r')
			usart1_send_char('\n');

		usart1_send_char(ch);

    	blink_count(LED_RED,1);
    }
	if (READ_BIT(USART1_BASE + USART_SR_OFFSET , ORE_BIT)){
        char *ch="ORE error\r\n";
		while (*ch != '\0')
			usart1_send_char(*ch++);
	    REG(USART1_BASE+USART_DR_OFFSET);



    }
}

