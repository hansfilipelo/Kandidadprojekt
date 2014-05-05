/*
 * sensormodul.cpp
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: Erik our grand receiver and David  our conqueror and master!
 *	All snopp åt Erik, vår buttyboy.
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "slave.h"

//------------sensorer----------------------
volatile int numOfSamples = 50;
volatile int savepos = 0;  //counter for the storage array

volatile int sensor0[50];
volatile int sensor1[50]; // not certain that this initialization will fly. 
volatile int sensor2[50];
volatile int sensor3[50];
volatile int sensor4[50];
volatile int sensor5[50];
volatile int sensor6[50];


volatile long int sen0;
volatile long int sen1;
volatile long int sen2;
volatile long int sen3;
volatile long int sen4;
volatile long int sen5;
volatile long int sen6;

//------------------ADC---------------------------
volatile double decadc=0;
volatile bool ADCdone = false;
double spanning = 0;

//------------------USART-------------------------
unsigned char indata;
unsigned char startbit = 0x0A;

//------------------GYRO--------------------------
bool gyromode = false;
volatile double medurs = -137;
volatile double moturs = 130;
volatile double angle = 0;
volatile double gyrovila = 0;
Slave sensormodul;

//-------------------------------Init--------------------------------------
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
	TCNT0 = 0x00;			//set timer to 0
	TCCR0B = 0x04;			//prescaler 256 på timer
	TIMSK0 = 0x00;		//stäng av tidsavbrott
}


//------------------------------------------------------------------------------
void gyrocal(){
	ADMUX = 0x27;
	volatile int i = 0;
	while(i < 100){
		asm("");
		ADCdone = false;
		ADCSRA |= 1<<ADSC;	// Start Conversion
		while(!ADCdone);	//vänta tills ADC klar
		cli();
		gyrovila = spanning + gyrovila;
		i++;
		sei();
	}
	gyrovila = gyrovila/100;
	ADMUX = 0x20;
}

void handleInDataArray(){		//hanterar det som skickats till sensormodulen från bussen
	if((sensormodul.inDataArray[1] == 'g') and (sensormodul.inDataArray[2] == 1)){
		gyromode = true;
		TCNT0 = 0x00;			//set timer to 0
	}
	else if((sensormodul.inDataArray[1] == 'g') and (sensormodul.inDataArray[2] == 0)){
		gyrocal();
	}
	else if((sensormodul.inDataArray[1] == 'k') and(sensormodul.inDataArray[2] == 0)){
		int hundra = sensormodul.inDataArray[3];
		int tio = sensormodul.inDataArray[4];
		int en = sensormodul.inDataArray[5];
		medurs = hundra * 10 + tio + en /10;
	}
	else if((sensormodul.inDataArray[1] == 'k') and(sensormodul.inDataArray[2] == 1)){
		int hundra = sensormodul.inDataArray[3];
		int tio = sensormodul.inDataArray[4];
		int en = sensormodul.inDataArray[5];
		moturs = hundra * 10 + tio + en /10;
	}
	else if(sensormodul.inDataArray[1] == 'r'){
		UCSR0B |= (1<<RXCIE0);						//enable USART interrups
	}
}

long int average(volatile int* inArray){
	long int sum=0;
	for(unsigned int i= 0; i < numOfSamples; i ++){
		sum = sum + inArray[i];
	}
	return (long int)sum/numOfSamples;
}

void sendSensors(){
    sensormodul.outDataArray[0] = 23;
    sensormodul.outDataArray[1] = 'S';
    sensormodul.outDataArray[2] =  'A'; //'DNC'
    sensormodul.outDataArray[3] = (sen0/100); //plats 4
    sensormodul.outDataArray[4] = ((sen0/10) %10); // plats 5
    sensormodul.outDataArray[5] = (sen0 % 10); // plats 6
    sensormodul.outDataArray[6] = (sen1/100); //plats 4
    sensormodul.outDataArray[7] = ((sen1/10) %10); // plats 5
    sensormodul.outDataArray[8] = (sen1 % 10); // plats 6
    sensormodul.outDataArray[9] = (sen2/100); //plats 4
    sensormodul.outDataArray[10] = ((sen2/10) %10); // plats 5
    sensormodul.outDataArray[11] = (sen2 % 10); // plats 6
    sensormodul.outDataArray[12] = (sen3/100); //plats 4
    sensormodul.outDataArray[13] = ((sen3/10) %10); // plats 5
    sensormodul.outDataArray[14] = (sen3 % 10); // plats 6
    sensormodul.outDataArray[15] = (sen4/100); //plats 4
    sensormodul.outDataArray[16] = ((sen4/10) %10); // plats 5
    sensormodul.outDataArray[17] = (sen4 % 10); // plats 6
    sensormodul.outDataArray[18] = (sen5/100); //plats 4
    sensormodul.outDataArray[19] = ((sen5/10) %10); // plats 5
    sensormodul.outDataArray[20] = (sen5 % 10); // plats 6
    sensormodul.outDataArray[21] = (sen6/100); //plats 4
    sensormodul.outDataArray[22] = ((sen6/10) %10); // plats 5
    sensormodul.outDataArray[23] = (sen6 % 10); // plats 6

    sensormodul.SPI_Send();

}


//------------------------------------INTERRUPTS---------------------------------

//Extremt känslig för tillägg av kod
//bussavbrott
ISR(SPI_STC_vect){
	sensormodul.position++;
	SPDR = sensormodul.outDataArray[sensormodul.position];
	sensormodul.inDataArray[sensormodul.position-1] = SPDR;
	
	if ((sensormodul.position == (sensormodul.inDataArray[0]+1))&(sensormodul.inDataArray[0]!= 0)){
		sensormodul.position = 0;
		PORTC |= (1<<PORTC0);
		PORTC &= ~(1<<PORTC0);
	}
	if((sensormodul.slaveSend && (sensormodul.position == sensormodul.outDataArray[0]+1))){ //When slave sends, position is not set to zero.
		sensormodul.slaveSend = false;
		sensormodul.position = 0;
	}
}

//internt avbrott för färdig mottagning
ISR(PCINT2_vect){
	handleInDataArray();
}
		
//ADC Conversion Complete Interrupt
ISR(ADC_vect)
{	
	sei();
	decadc = ADCH;
	spanning = decadc*5/256;
	ADCdone = true;
	if(gyromode){
		if( spanning < gyrovila + 0.08 && spanning > gyrovila - 0.05){	//gränser kan behöva justeras
		}
		else{
			angle = angle + spanning-gyrovila;			//gränser kan behöva justeras
		}
	}
}

//Avbrott för USART, kanske disabla efter några läsningar för att inte avbryta ADC
ISR(USART0_RX_vect){
	indata=UDR0;
		
	if(indata==startbit){
		sensormodul.outDataArray[0] = 1;
		sensormodul.outDataArray[1] = 'R';
		sensormodul.SPI_Send();				//skicka RFID detekterad
		UCSR0B &= ~(1<<RXCIE0);				//disable USART interrups
	}
}

//avbrott för timer
ISR(TIMER0_OVF_vect){
	asm("");
	ADCSRA |= 1<<ADSC;
}


int main(void)
{	
	sensormodul.SPI_Init();
	Sensor_Init();
	USART_Init(383);
	Timer_Init();
	
	sei();					// Enable Global Interrupts
	gyrocal();
	ADCSRA |= 1<<ADSC;		// Start Conversion
	
	while(1){				// Wait forever
		
		while(gyromode){		//hoppa till gyrodelen
			ADMUX = 0x27;
			TIMSK0 = 0x01;		//tillåt tidsavbrott
			/* kan behövas för att ge lite tid åt avbrotten
			ADCdone = false;
			while(!ADCdone);	//vänta tills ADC klar
			*/
			cli();
			if((angle <= medurs) or (angle >= moturs)){
				asm("");
				gyromode = false;
				angle = 0;
				TIMSK0 = 0x00;		//stäng av tidsavbrott
				sensormodul.outDataArray[0] = 1;
				sensormodul.outDataArray[1] = 'G';
				sensormodul.SPI_Send();
			}
			sei();
		}
		
		if(ADMUX == 0x20){			//konvertering av A0
			asm("");
			sensor0[savepos]	= round(45.64*pow(spanning,4)-320.2*pow(spanning,3)+830.3*pow(spanning,2)-984.9*spanning+524.4);
			asm("");
		}
		
		if(ADMUX == 0x21){		//konvertering av A1
			asm("");
			sensor1[savepos]	= round(1.031*pow(spanning,4)-68*pow(spanning,3)+364.8*pow(spanning,2)-683.2*spanning+492.2);
		}
				
		if( (ADMUX == 0x22)){	//konvertering av A2-A5 (kortdistanssensorer)
			asm("");
			sensor2[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}
		
        if( (ADMUX == 0x23)){	//konvertering av A2-A5 (kortdistanssensorer)
			asm("");
			sensor3[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}
		
        if( (ADMUX == 0x24)){	//konvertering av A2-A5 (kortdistanssensorer)
			asm("");
			sensor4[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}
		
        if( (ADMUX == 0x25)){	//konvertering av A2-A5 (kortdistanssensorer)
			asm("");
			sensor5[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}

		if(ADMUX == 0x26){		//konvertering av A7 (mellandistanssensorn)
			asm("");
			sensor6[savepos]	= round(8.139*pow(spanning,4)-81.21*pow(spanning,3)+282.6*pow(spanning,2)-414.2*spanning+259.7);
			
		}

		if(ADMUX == 0x26){
			ADMUX = 0x20;
			savepos++;
		}
		else{
			ADMUX = ADMUX + 1;
		}
        if(savepos == numOfSamples){
            sen0 = average(sensor0);
            sen1 = average(sensor1);
            sen2 = average(sensor2);
            sen3 = average(sensor3);
            sen4 = average(sensor4);
            sen5 = average(sensor5);
            sen6 = average(sensor6);
            
            sendSensors();
            
            savepos = 0;
            
        }

		ADCdone = false;
		ADCSRA |= 1<<ADSC;	// Start Conversion
		
		while(!ADCdone);	//vänta tills ADC klar
	}
}