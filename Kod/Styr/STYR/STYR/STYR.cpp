/*
 * STYR.c
 *
 * Created: 3/27/2014 11:58:55 AM
 *  Author: hanel742 och tobgr602
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Map.h"
#include "Abstraction.cpp"

void pwm_init()
{	
	OCR2A = 0;
	OCR2B = 0;
	
	TCCR2A |= (1<<WGM21)| (1<<WGM20) | (1<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (0<<COM2B0);
	TCCR2B |= (0<<WGM22) | (0<<CS22) | (1<<CS21) | (1<<CS20);
	
	OCR2A = 0;
	OCR2B = 0;
	// Set Port 20 and 21 as outputs (for PWM)
	// Set port 18 and 19 as outputs (for choosing direction)
	// Set port 16 and 17 as inputs (for manual controllers)
	DDRD = 0xF0;
	
	// Initiate gear as 00
	PORTD |= (0<<PORTD4) | (0<<PORTD5);
}

// Chooses direction
int gear = 0;
int speed = 0;

// Gearbox, port 17
ISR(INT0_vect){
	cli();
	if (gear == 0){
		PORTD |= 0x10;
		PORTD &= ~0x20;
		gear = gear + 1;
	}
	else if (gear == 1){
		PORTD &= ~0x10;
		PORTD |= 0x20;
		gear = gear + 1;
	}
	else if (gear == 2){
		PORTD |= 0x10;
		PORTD |= 0x20;
		gear = gear + 1;
	}
	else {
		PORTD &= ~0x10;
		PORTD &= ~0x20;
		gear = 0;
	}
		
	sei();
}

// Drive, port 16
ISR(INT1_vect){
	cli();
	if (speed == 0){
		speed = 25;
	}
	else{
		speed = 0;
	}
	int output = floor(speed * 255 / 100);
	
	OCR2A = output;
	OCR2B = output;
	
	sei();
}


int main(void)
{	
	cli();
	// Set up interrupts
	MCUCR = 0b00000000;
	EIMSK = 0b00000011;
	EICRA = 0b00001111;
	SMCR = 0x01;
	
	pwm_init();

	// Create map
	sei();
	
	while(1){
	}
	
	return 0;
}


