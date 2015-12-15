#include <xc.h>
#include <htc.h>
#include "main.h"
#define _XTAL_FREQ 8000000
#pragma config FOSC     = INTOSC
#pragma config WDTE     = OFF
#pragma config PWRTE    = OFF
#pragma config MCLRE    = ON
#pragma config CP       = OFF
#pragma config BOREN    = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO     = OFF
#pragma config FCMEN    = OFF
#pragma config WRT      = OFF
#pragma config PLLEN    = OFF
#pragma config STVREN   = ON
#pragma config BORV     = HI
#pragma config LVP      = OFF

const int length = sizeof(wav) / sizeof(wav[0]);
int position;

void interrupt timer(void) {
	if (TMR0IF == 1) {
		TMR0 = 135;
		TMR0IF = 0;
		if (position < length) {
			DAC1CON1 = wav[position++];
		}
	}
}

void main() {
	OSCCON     = 0b01110010;
	ANSELA     = 0b00000000;
	ANSELC     = 0b00000000; 
	TRISA      = 0b00000000;
	WPUA       = 0b00000000;
	TRISC      = 0b00100000;
	WPUC       = 0b00100000;
	OPTION_REG = 0b00000000;
	PORTA      = 0;
	PORTC      = 0;
	PEIE       = 1;
	GIE        = 1;
	
	// DAC + OPA
	DAC1CON1   = 0;
	DAC1CON0   = 0b10000000;
	OPA1CON    = 0b11010010;
	
	// Wave loop
	position   = length;
	TMR0       = 0;
	TMR0IF     = 0;
	TMR0IE     = 1;

	while (1) {
		if (RC5 == 0) {
			position = 0;
			while (RC5 == 0);
			__delay_ms(100);
		}
	}
}
