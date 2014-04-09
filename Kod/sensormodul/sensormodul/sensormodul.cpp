/*
 * sensormodul.cpp
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: jened502
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "slave.h"

#define F_CPU 14.7456E6
#include <util/delay.h>


//unsigned int timer;
int sensordata[7]={};
int savepos = 0;		//counter for the storage array
double decadc=0;
double spanning = 0;
unsigned char indata;
unsigned char startbit = 0x0A;
double vinkelv = 0;
bool gyromode = false;
long int konstant = 458610;
Slave sensormodul;

//Initiera USART
void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	/* Enable receiver and interrupts */
	UCSR0B = (1<<RXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}




void handleInDataArray(){
sensormodul.outDataArray[0]=0;
sensormodul.outDataArray[0]=1;

}

//Extremt känslig för tillägg av kod
ISR(SPI_STC_vect){
	sensormodul.position++;
	SPDR = sensormodul.outDataArray[sensormodul.position];
	sensormodul.inDataArray[sensormodul.position-1] = SPDR;
	
	if ((sensormodul.position == (sensormodul.inDataArray[0]+1))&(sensormodul.inDataArray[0]!= 0)){
		PORTC |= (1<<PORTC0);
	}
}

ISR(PCINT2_vect){
	handleInDataArray();
}
		
//ADC Conversion Complete Interrupt Service Routine (ISR)
ISR(ADC_vect)
{	
	sei();
	decadc = ADCH;
	spanning = decadc*5/256;
}

//Avbrott för USART, kanske disabla efter några läsningar för att inte avbryta ADC
ISR(USART0_RX_vect){
	indata=UDR0;
		
	if(indata==startbit){
		sensormodul.outDataArray[0] = 3;
		sensormodul.outDataArray[1] = 'r';
		sensormodul.outDataArray[2] = '0';
		sensormodul.outDataArray[3] = '1';
		//skicka RFID
	}
}

int main(void)
{	
	
	sensormodul.SPI_Init();
	USART_Init(383);
	
	DDRA = 0x00;			// Configure PortA as input
							// PA0 is ADC0 input
							
	ADCSRA = 0x8F;			// Enable the ADC and its interrupt feature
							// and set the ACD clock pre-scalar to clk/128
	ADMUX = 0x20;			// Select 5V as Vref, left justify
							// data registers and select ADC0 as input channel
	
	sei();					// Enable Global Interrupts
	ADCSRA |= 1<<ADSC;		// Start Conversion
	
	while(1){				// Wait forever
		
		if(ADMUX == 0x20){	//konvertering av A0
			sensordata[savepos]	= round(45.64*pow(spanning,4)-320.2*pow(spanning,3)+830.3*pow(spanning,2)-984.9*spanning+524.4);
		}
		
		else if(ADMUX == 0x21){ //konvertering av A1
			sensordata[savepos]	= round(1.031*pow(spanning,4)-68*pow(spanning,3)+364.8*pow(spanning,2)-683.2*spanning+492.2);
		}
		
		else if(ADMUX == 0x26){	//konvertering av A7 (mellandistanssensorn)
			sensordata[savepos]	= round(8.139*pow(spanning,4)-81.21*pow(spanning,3)+282.6*pow(spanning,2)-414.2*spanning+259.7);
		}
		
		else if(ADMUX == 0x27){ //gyro
			vinkelv = (spanning)*150-370+0.859375;
			//timer = TCNT0;
			sensordata[7] = sensordata[7] + vinkelv; //sensordata[7] = sensordata[7] + vinkelv(timer*8/14000000.0);
			
			if ((sensordata[7] > konstant) or (sensordata[7] < -1*konstant)){ // mindre än 90 gradersvärdet
				gyromode = false;
				/*
				outDataArray[0] = 3;
				outDataArray[1] = 'g';
				outDataArray[2] = '0';
				outDataArray[3] = '1';
				//skicka att 90grader är klar
				*/
			}
			//TCNT0 = 0;
		}
		else if(ADMUX == 0x22 || 0x23 || 0x24 || 0x25){	//konvertering av A2-A5 (kortdistanssensorer)
			sensordata[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}
		
		if (gyromode){
			ADMUX = 0x27;
		}
		
		if(ADMUX == 0x26 or (!gyromode and ADMUX == 0x27)){
			ADMUX = 0x20;
			savepos = 0;
		}
		else if (ADMUX !=0x27)
		{
			
			sensormodul.outDataArray[0] = 5;
			sensormodul.outDataArray[1] = 'a';
			sensormodul.outDataArray[2] =  savepos + 48;
			sensormodul.outDataArray[3] = (sensordata[savepos]/100) + 48; //plats 4
			sensormodul.outDataArray[4] = ((sensordata[savepos]/10) %10) + 48; // plats 5
			sensormodul.outDataArray[5] = (sensordata[savepos] % 10) + 48; // plats 6
				
			//ADMUX = ADMUX + 1;
			//savepos++;
		}
		
		if ((PIND==0x80)){
			sensormodul.SPI_Send();
			_delay_ms(5);
		}	
		ADCSRA |= 1<<ADSC;	// Start Conversion
		
	}
}