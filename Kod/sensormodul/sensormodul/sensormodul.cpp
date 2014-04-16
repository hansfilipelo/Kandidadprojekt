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


int sensordata[7]={};
long double gyrodata = 0;
int savepos = 0;		//counter for the storage array
double decadc=0;
double spanning = 0;
int intspanning = 0;
unsigned char indata;
unsigned char startbit = 0x0A;
bool gyromode = false;
long int konstantett = 90000;
long int konstanttwa = 170000;
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
	TIMSK0 = 0x02;			//enable compare A interrupt
	OCR0A = 0xFF;			//set COMPA
	TCNT0 = 0x00;			//set timer to 0
	TCCR0B = 0x05;			//prescaler 8 på timer
}


void handleInDataArray(){
	if(sensormodul.inDataArray[1] == 'g'){
		gyromode = true;
	}
	else if((sensormodul.inDataArray[1] == 'k') and(sensormodul.inDataArray[2] == '0')){
		int hundratusen = sensormodul.inDataArray[3];
		int tiotusen = sensormodul.inDataArray[4];
		int tusen = sensormodul.inDataArray[5];
		int hundra = sensormodul.inDataArray[6];
		int tio = sensormodul.inDataArray[7];
		int en = sensormodul.inDataArray[8];
		konstantett = hundratusen * 100000 + tiotusen * 10000 + tusen * 10000 + hundra * 100 + tio * 10 + en;
	}
	else if((sensormodul.inDataArray[1] == 'k') and(sensormodul.inDataArray[2] == '1')){
		int hundratusen = sensormodul.inDataArray[3];
		int tiotusen = sensormodul.inDataArray[4];
		int tusen = sensormodul.inDataArray[5];
		int hundra = sensormodul.inDataArray[6];
		int tio = sensormodul.inDataArray[7];
		int en = sensormodul.inDataArray[8];
		konstanttwa = hundratusen * 100000 + tiotusen * 10000 + tusen * 10000 + hundra * 100 + tio * 10 + en;
	}
}

//Extremt känslig för tillägg av kod
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
}

//Avbrott för USART, kanske disabla efter några läsningar för att inte avbryta ADC
ISR(USART0_RX_vect){
	indata=UDR0;
		
	if(indata==startbit){
		sensormodul.outDataArray[0] = 1;
		sensormodul.outDataArray[1] = 'R';
		sensormodul.SPI_Send();				//skicka RFID detekterad
		//skicka detekterad till styrmodul ->stänga av RFID läsning -> vänta på ruta lämnad från styrmodul ->tillåt RFID-läsning igen?
	}
}

ISR(TIMER0_COMPA_vect){
	TCNT0 = 0x00;
	ADCSRA |= 1<<ADSC;		// Start Conversion
}


int main(void)
{	
	//gyromode = true;
	sensormodul.SPI_Init();
	Sensor_Init();
	USART_Init(7);
	
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
			intspanning = round(spanning*100);
			if(intspanning < 246){
				gyrodata = gyrodata - (2.48*2 - intspanning/100);
			}
			else if(intspanning > 250){
				gyrodata = gyrodata + intspanning/100;
			}
			if ((gyrodata > konstantett) or (gyrodata < -1*konstanttwa)){ // mindre än 90 gradersvärdet
				TIMSK0 = 0x00;			//disable compare A interrupt
				gyromode = false;
				sensormodul.outDataArray[0] = 1;
				sensormodul.outDataArray[1] = 'G';
				sensormodul.SPI_Send();				//skicka att 90grader är klar
			}
		}
		else if(ADMUX == 0x22 || 0x23 || 0x24 || 0x25){	//konvertering av A2-A5 (kortdistanssensorer)
			sensordata[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}
		
		if (gyromode){
			ADMUX = 0x27;
			Timer_Init();
		}
		
		if(ADMUX == 0x26 or (!gyromode and ADMUX == 0x27)){
			ADMUX = 0x20;
			savepos = 0;
		}
		else if (ADMUX !=0x27)
		{
			sensormodul.outDataArray[0] = 5;
			sensormodul.outDataArray[1] = 'S';
			sensormodul.outDataArray[2] =  savepos + 48;
			sensormodul.outDataArray[3] = (sensordata[savepos]/100) + 48; //plats 4
			sensormodul.outDataArray[4] = ((sensordata[savepos]/10) %10) + 48; // plats 5
			sensormodul.outDataArray[5] = (sensordata[savepos] % 10) + 48; // plats 6
			
			ADMUX = ADMUX + 1;
			savepos++;
		}
		
		if ((PIND==0x80)){
			sensormodul.SPI_Send();
			_delay_ms(5);
		}	
		ADCSRA |= 1<<ADSC;	// Start Conversion
		
	}
}