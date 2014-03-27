/*
 * STYR.c
 *
 * Created: 3/27/2014 11:58:55 AM
 *  Author: fille
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void pwm_init()
{
	// Set Port 20 and 21 as outputs (for PWM)
	// Set port 18 and 19 as outputs (for choosing direction)
	// Set port 16 and 17 as inputs (for manual controllers)
	DDRD = 0xF0;
	
	// Initiate gear as 00
	PORTD |= (0<<PORTD4) | (0<<PORTD5);
}

// Chooses direction
int gear = 0;

// Gearbox, port 17
ISR(INT0_vect){
	cli();
	if (gear == 3)
	{
		PORTD |= (0<<PORTD4) | (0<<PORTD5);
		gear = 0x00;
	}
	else{
		if (gear == 0)
		{
			PORTD |= (1<<PORTD4) | (0<<PORTD5);
		}
		else if (gear == 1)
		{
			PORTD |= (0<<PORTD4) | (1<<PORTD5);
		}
		else if (gear == 2)
		{
			PORTD |= (1<<PORTD4) | (1<<PORTD5);
		}
		gear = gear + 1;
		
	}
	sei();
}

// Drive, port 16
ISR(INT1_vect){
	cli();
	int speed = 50;
	int output = speed * 255 / 100;
	
	while (PORTD2 == 1)
	{
		OCR2A = output;
		OCR2B = output;
	}
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
	sei();
	
	while(1){
	sleep_enable();
	sleep_cpu();
	}
	
	return 0;
}
