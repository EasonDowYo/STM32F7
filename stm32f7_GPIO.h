#ifndef GPIO_H
#define GPIO_H




// GPIO moder
#define OUTPUT 1
#define AF 2
#define INPUT 0
#define IOA 3
// GPIO otyper
#define PP 0
#define OD 1
// GPIO ospeed
#define LOW 0
#define MEDIUM 1
#define HIGH 2
#define VERY_HIGH 3
//GPIO PUPD
#define none_PUPD 0
#define PU 1
#define PD 2
#define RESERVED 4

// userButtom is GPIO_PORTB pin 1
#define userButtom 1

// test led is GPIO_PORTI pin 1
#define TEST_LED 1

//RCC  
#define RCC_BASE 0x40023800

#define RCC_AHB1ENR_OFFSET 0x30

//GPIO PORT
#define GPIO_PORTA 0
#define GPIO_PORTB 1
#define GPIO_PORTC 2
#define GPIO_PORTD 3
#define GPIO_PORTE 4
#define GPIO_PORTF 5
#define GPIO_PORTG 6
#define GPIO_PORTH 7
#define GPIO_PORTI 8
#define GPIO_PORTJ 9
#define GPIO_PORTK 10

#define GPIO_EN_BIT(port) (port)


#define GPIO_BASE(port) (0x40020000 + 0x400 * (port))

/* ======= GPIO register start =======*/
#define GPIOx_MODER_OFFSET 0x00
#define MODERy_1_BIT(y) ((y)*2 + 1)
#define MODERy_0_BIT(y) ((y)*2)

#define GPIOx_OTYPER_OFFSET 0x04
#define OTy_BIT(y) (y)

#define GPIOx_OSPEEDR_OFFSET 0x08
#define OSPEEDRy_1_BIT(y) ((y)*2 + 1)
#define OSPEEDRy_0_BIT(y) ((y)*2)

#define GPIOx_PUPDR_OFFSET 0x0C
#define PUPDRy_1_BIT(y) ((y)*2 + 1)
#define PUPDRy_0_BIT(y) ((y)*2)

#define GPIOx_BSRR_OFFSET 0x18
#define BRy_BIT(y) ((y) + 16)
#define BSy_BIT(y) (y)

#define GPIOx_IDR_OFFSET 0x10
#define IDRy_BIT(y) (y)

#define GPIOx_AFRL_OFFSET 0x20
#define AFRLy_3_BIT(y) ((y)*4 + 3)
#define AFRLy_0_BIT(y) ((y)*4)

/* ======= GPIO register end =======*/


/*======== GPIO function ========*/
typedef struct GPIO GPIOtype;
typedef void (*GPIOfunc)(GPIOtype *self);
typedef int (*GPIOvalue)(GPIOtype *self);
struct GPIO{
	uint32_t port , pin , onoff;
    uint32_t mode , otyper , ospeed , pupd;
	GPIOfunc IO_config,GPIO_onoff,blink_ct,blink;
    GPIOvalue DRead;
};
int init_GPIO(GPIOtype **self);

void IO_config_imp(GPIOtype *self);//method

void GPIO_onoff_imp(GPIOtype *self);//method

void blink_ct_imp(GPIOtype *self);//method

void blink_imp(GPIOtype *self);//method

int DRead_imp(GPIOtype *self);//method


#endif
