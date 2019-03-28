#include "GPIO.h"


int main(void)
{
	pinMode(GPIO_PORTD,LED_GREEN,OUTPUT);
	pinMode(GPIO_PORTD,LED_ORANGE,OUTPUT);
	pinMode(GPIO_PORTD,LED_RED,OUTPUT);
	pinMode(GPIO_PORTD,LED_BLUE,OUTPUT);

	pinMode(GPIO_PORTA,userButtom,INPUT);

	while(1){
	int i=digitalRead(GPIO_PORTA,userButtom);
	if(i){
		digitalWrite(GPIO_PORTD,LED_GREEN,HIGH);
		digitalWrite(GPIO_PORTD,LED_BLUE,HIGH);
		digitalWrite(GPIO_PORTD,LED_ORANGE,HIGH);
		digitalWrite(GPIO_PORTD,LED_RED,HIGH);
		Delay(100);
		digitalWrite(GPIO_PORTD,LED_GREEN,LOW);
		digitalWrite(GPIO_PORTD,LED_BLUE,LOW);
		digitalWrite(GPIO_PORTD,LED_ORANGE,LOW);
		digitalWrite(GPIO_PORTD,LED_RED,LOW);
		Delay(100);
		}
	}
}
