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
	cli();
	// initialize TCCRX for fast PWM
	TCCR2A |= (1<<WGM21)| (0<<WGM20) | (1<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (0<<COM2B0);
	
	// Set Port 20 and 21 as outputs (for PWM)
	DDRD |= (1<<DDRD6) | (1<<DDRD7);
	
	// Set port 18 and 19 as outputs (for choosing direction)
	DDRD |= (1<<DDRD5) | (1<<DDRD4);
	
	// Set port 16 and 17 as inputs (for manual controllers)
	DDRD |= (0<<DDRD3) | (0<<DDRD2);
	
	// Initiate gear as 00
	PORTD |= (0<<PORTD4) | (0<<PORTD5);
	sei();
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
	pwm_init();
	sleep();
	
	return 0;
}