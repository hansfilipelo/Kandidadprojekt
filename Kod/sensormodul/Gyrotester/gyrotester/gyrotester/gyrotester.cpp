/*
 * gyrotester.cpp
 *
 * Created: 4/28/2014 2:56:35 PM
 *  Author: jened502
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

int varde;
volatile bool butt;
long int calc = 0;
double hey;
volatile double medurs = -16.5;
volatile double moturs = 17;
volatile double angle = 0;
volatile double yo;

void Timer_Init()
{
	TCNT0 = 0x00;			//set timer to 0
	TCCR0B = 0x04;			//prescaler 1024 på timer
	TIMSK0 = 0x01;
}


void Sensor_Init()
{
	DDRA = 0x00;			// Configure PortA as input
	
	ADCSRA = 0x8F;			// Enable the ADC and its interrupt feature
	// and set the ACD clock pre-scalar to clk/128
	ADMUX = 0x24;			// Start ADC at port 4, 5v ref
}


//ADC Conversion Complete Interrupt
ISR(ADC_vect)
{
	varde = ADCH;
	double temp = (double)varde;
	hey = (double)temp*5/256;
	asm("");
	if( hey < 2.51 && hey > 2.47){
		
	}
	else{
	angle = angle + hey-2.48;	
	}
	
}
ISR(TIMER0_OVF_vect){
	asm("");
	calc++;
	ADCSRA |= 1<<ADSC; 
}




int main(void)
{
	asm("");
	Sensor_Init();
	Timer_Init();
	while(1){
	yo = medurs -angle; 
	
	cli();	
	if(yo >= 0){
		asm("");
		butt = true;
		angle = 0;
		
	}
	if(angle >= moturs){
		asm("");
		butt = false;
		angle = 0; 
	}
	sei();
	butt = true;
	
	
		   //do nothing
	}
}