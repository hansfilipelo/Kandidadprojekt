/*
 * sensormodul.cpp
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: Erik our conqueror and master!
 *	All makt �t Erik v�r befriare.
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "slave.h"

#define F_CPU 14.7456E6
#include <util/delay.h>


volatile int sensordata[7]={};
volatile int savepos = 0;		//counter for the storage array
volatile double decadc=0;
volatile bool ADCdone = false;
double spanning = 0;

unsigned char indata;
unsigned char startbit = 0x0A;


bool gyromode = false;
double gyrodata = 0;
long int leftTurnConstant = 1000000;
long int rightTurnConstant = 1000000;
int time = 0;

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

void Sensor_Init()
{
	DDRA = 0x00;			// Configure PortA as input
	
	ADCSRA = 0x8F;			// Enable the ADC and its interrupt feature
							// and set the ACD clock pre-scalar to clk/128
	ADMUX = 0x20;			// Start ADC at port 0, 5v ref
}

void Timer_Init()
{
	OCR0A = 0x80;			//set COMPA
	TCNT0 = 0x00;			//set timer to 0
	TCCR0B = 0x04;			//prescaler 256 p� timer
}

void handleInDataArray(){
	if(sensormodul.inDataArray[1] == 'g'){
		gyromode = true;
		//nollst�lla tiden om den ska anv�ndas
	}
	else if((sensormodul.inDataArray[1] == 'k') and(sensormodul.inDataArray[2] == '0')){
		int hundratusen = sensormodul.inDataArray[3];
		int tiotusen = sensormodul.inDataArray[4];
		int tusen = sensormodul.inDataArray[5];
		int hundra = sensormodul.inDataArray[6];
		int tio = sensormodul.inDataArray[7];
		int en = sensormodul.inDataArray[8];
		leftTurnConstant = hundratusen * 100000 + tiotusen * 10000 + tusen * 10000 + hundra * 100 + tio * 10 + en;
	}
	else if((sensormodul.inDataArray[1] == 'k') and(sensormodul.inDataArray[2] == '1')){
		int hundratusen = sensormodul.inDataArray[3];
		int tiotusen = sensormodul.inDataArray[4];
		int tusen = sensormodul.inDataArray[5];
		int hundra = sensormodul.inDataArray[6];
		int tio = sensormodul.inDataArray[7];
		int en = sensormodul.inDataArray[8];
		rightTurnConstant = hundratusen * 100000 + tiotusen * 10000 + tusen * 10000 + hundra * 100 + tio * 10 + en;
	}
}

//Extremt k�nslig f�r till�gg av kod
ISR(SPI_STC_vect){
	sensormodul.position++;
	SPDR = sensormodul.outDataArray[sensormodul.position];
	sensormodul.inDataArray[sensormodul.position-1] = SPDR;
	
	if ((sensormodul.position == (sensormodul.inDataArray[0]+1))&(sensormodul.inDataArray[0]!= 0)){
		PORTC |= (1<<PORTC0);
		sensormodul.position = 0;
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
	ADCdone = true;
}

//Avbrott f�r USART, kanske disabla efter n�gra l�sningar f�r att inte avbryta ADC
ISR(USART0_RX_vect){
	indata=UDR0;
		
	if(indata==startbit){
		sensormodul.outDataArray[0] = 1;
		sensormodul.outDataArray[1] = 'R';
		sensormodul.SPI_Send();				//skicka RFID detekterad
		//skicka detekterad till styrmodul ->st�nga av RFID l�sning -> v�nta p� ruta l�mnad fr�n styrmodul ->till�t RFID-l�sning igen?
	}
}

//avbrott f�r timer
ISR(TIMER0_COMPA_vect){
	TIMSK0 = 0x00;			//disable compare A interrupt
	ADCSRA |= 1<<ADSC;		// Start Conversion
	/*
		TCNT0 = 0x00;			//set timer to 0
		TIMSK0 = 0x02;			//enable compare A interrupt
	*/
}

//rutin f�r gyroutslag
void gyroberakning(){
	ADMUX = 0x27;
	ADCdone = false;		// adc is in progress
	time = TCNT0;
	ADCSRA |= 1<<ADSC;		// Start Conversion
	while(!ADCdone);		// v�nta tills adc klar
	TCNT0 = 0x00;			//set timer to 0
	if((spanning > 4.5)||(spanning<0.5));
	else if((spanning < 2.46)||(spanning > 2.55)){
		gyrodata =  gyrodata + ((spanning - 2.5)*150)*time/1000;
	}
	if((gyrodata > leftTurnConstant) || (gyrodata < -1*rightTurnConstant))
	{
		leftTurnConstant = TCNT0;
	}
}

int main(void)
{	
	sensormodul.SPI_Init();
	Sensor_Init();
	USART_Init(7);
	Timer_Init();
	
	sei();					// Enable Global Interrupts
	ADCSRA |= 1<<ADSC;		// Start Conversion
	
	while(1){				// Wait forever
		
		while(gyromode){
			gyroberakning();
		}
		
		if(ADMUX == 0x20){			//konvertering av A0
			sensordata[savepos]	= round(45.64*pow(spanning,4)-320.2*pow(spanning,3)+830.3*pow(spanning,2)-984.9*spanning+524.4);
		}
		
		if(ADMUX == 0x21){		//konvertering av A1
			sensordata[savepos]	= round(1.031*pow(spanning,4)-68*pow(spanning,3)+364.8*pow(spanning,2)-683.2*spanning+492.2);
		}
		
		
		if( (ADMUX == 0x22) ||(ADMUX == 0x23) || (ADMUX ==0x24) || (ADMUX == 0x25)){	//konvertering av A2-A5 (kortdistanssensorer)
			asm("");
			sensordata[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}
		
		if(ADMUX == 0x26){		//konvertering av A7 (mellandistanssensorn)
			asm("");
			sensordata[savepos]	= round(8.139*pow(spanning,4)-81.21*pow(spanning,3)+282.6*pow(spanning,2)-414.2*spanning+259.7);
			
		}
		
		sensormodul.outDataArray[0] = 5;
		sensormodul.outDataArray[1] = 'S';
		sensormodul.outDataArray[2] =  savepos;
		sensormodul.outDataArray[3] = (sensordata[savepos]/100); //plats 4
		sensormodul.outDataArray[4] = ((sensordata[savepos]/10) %10); // plats 5
		sensormodul.outDataArray[5] = (sensordata[savepos] % 10); // plats 6
		sensormodul.SPI_Send();

		if(ADMUX == 0x26){
			ADMUX = 0x20;
			savepos = 0;
		}
		else{
			ADMUX = ADMUX + 1;
			savepos++;
		}

		ADCdone = false;		
		_delay_ms(50);
		ADCSRA |= 1<<ADSC;	// Start Conversion
		
		while(!ADCdone);	//v�nta tills ADC klar
	}
}