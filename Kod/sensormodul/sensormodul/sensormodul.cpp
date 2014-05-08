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
volatile int numOfSamples = 25;		//number of samples for mean value
volatile int savepos = 0;			//counter for the storage array

volatile int sensor0[25];			//arrays for sensordata
volatile int sensor1[25];
volatile int sensor2[25];
volatile int sensor3[25];
volatile int sensor4[25];
volatile int sensor5[25];
volatile int sensor6[25];


volatile long int sen0;				//integers for mean distance
volatile long int sen1;
volatile long int sen2;
volatile long int sen3;
volatile long int sen4;
volatile long int sen5;
volatile long int sen6;
volatile int RfidCount = 0;

//------------------ADC---------------------------
volatile double decadc=0;			//variable used in the ADC-interrupt (decimal adc-value, ADC-value with 5 V ref)
volatile bool ADCdone = false;		//Flag for checking if ADC is done
double spanning = 0;				//ADC-value without 5v ref

//------------------USART-------------------------
unsigned char indata;				//data from USART-reading
unsigned char startbit = 0x0A;		//start sequence for RFID-tags

//------------------GYRO--------------------------
bool gyromode = false;				//flag for initializing gyroreadings
volatile double medurs = -137;		//constant to end right turn
volatile double moturs = 130;		//constant to end left turn
volatile double angle = 0;			//constant for angle
volatile double gyrovila = 0;		//calibration constant for reference
Slave sensormodul;					//construct the slave "sensormodul"

//-------------------------------Init--------------------------------------
//Usart initialization
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

//Sensor initialization
void Sensor_Init()
{
	DDRA = 0x00;			// Configure PortA as input
	
	ADCSRA = 0x8F;			// Enable the ADC and its interrupt feature
							// and set the ACD clock pre-scalar to clk/128
	ADMUX = 0x20;			// Start ADC at port 0, external Vref
}

//Timer initialization
void Timer_Init()
{
	TCNT0 = 0x00;			//set timer to 0
	TCCR0B = 0x04;			//pre-scalar 256 på timer
	TIMSK0 = 0x00;			//dont't allow time-interrups
}


//------------------------------------------------------------------------------
//calibration of reference constant "gyrovila"
void gyrocal(){
	ADMUX = 0x27;							//set ADMUX to input 7, ADC gets it's data from gyro
	volatile int i = 0;						//loopcounter
	while(i < 100){							//loop
		asm("");
		ADCdone = false;					//set ADC flag to false
		ADCSRA |= 1<<ADSC;					//start Conversion
		while(!ADCdone);					//wait until ADC is done
		cli();
		gyrovila = spanning + gyrovila;		//sum 100 measurements
		i++;								//add one to counter
		sei();
	}
	gyrovila = gyrovila/100;				//calculate mean constant
	ADMUX = 0x20;							//set ADMUX to input 0
}

//handles data from SPI
void handleInDataArray(){
	//if [g 1] is received
	if((sensormodul.inDataArray[1] == 'g') and (sensormodul.inDataArray[2] == 1)){
		gyromode = true;		//set gyromode to true
		TCNT0 = 0x00;			//set timer to 0
	}
	//if [g 0] is received
	else if((sensormodul.inDataArray[1] == 'g') and (sensormodul.inDataArray[2] == 0)){
		gyrocal();				//run gyrocal
	}

	//if [g 2] is received
	else if((sensormodul.inDataArray[1] == 'g') and (sensormodul.inDataArray[2] == 2)){
		gyromode = false;	//done with gyroreadings
		angle = 0;			//reset angle
		TIMSK0 = 0x00;		//don't allow time interrupts
	}
	//if [k 1] is received
	else if((sensormodul.inDataArray[1] == 'k') and(sensormodul.inDataArray[2] == 1)){
		int hundra = sensormodul.inDataArray[3];
		int tio = sensormodul.inDataArray[4];
		int en = sensormodul.inDataArray[5];
		moturs = hundra * 10 + tio + en /10;			//update the left turn constant
	}
	//if [k 0] is received
	else if((sensormodul.inDataArray[1] == 'k') and(sensormodul.inDataArray[2] == 0)){
		int hundra = sensormodul.inDataArray[3];
		int tio = sensormodul.inDataArray[4];
		int en = sensormodul.inDataArray[5];
		medurs = hundra * 10 + tio + en /10;			//update the right turn constant
	}
	//if [r] is received
	else if(sensormodul.inDataArray[1] == 'r'){
		UCSR0B |= (1<<RXCIE0);							//enable USART interrups
	}
}

//calculate the mean of a array of integers
long int average(volatile int* inArray){
	long int sum=0;										//integer for the sum of the arrays integers
	for(unsigned int i= 0; i < numOfSamples; i ++){		//loop
		sum = sum + inArray[i];							//sum all integers
	}
	return (long int)sum/numOfSamples;					//return mean of integers
}

//send sensordata
void sendSensors(){
	//set outDataArray
    sensormodul.outDataArray[0] = 26;
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
	sensormodul.outDataArray[24] = (RfidCount/100);
	sensormodul.outDataArray[25] = ((RfidCount/10) %10);
	sensormodul.outDataArray[26] = (RfidCount % 10);
	
	
	if (sensormodul.outDataArray[9] == 0){
		volatile int p = 0;
		p++;
	}

    sensormodul.SPI_Send();			//send outDataArray
}


//------------------------------------INTERRUPTS---------------------------------

//SPI interrupt
//Extremt känslig för tillägg av kod
ISR(SPI_STC_vect){
	sensormodul.position++;			//update position
	SPDR = sensormodul.outDataArray[sensormodul.position];	//set SPDR to current integer in array
	sensormodul.inDataArray[sensormodul.position-1] = SPDR;	//set inDataArray to SPDR
	
	//if the complete sequence
	if ((sensormodul.position == (sensormodul.inDataArray[0]+1))&(sensormodul.inDataArray[0]!= 0)){
		sensormodul.position = 0;		//update position
		PORTC |= (1<<PORTC0);			//call internal interrupt
		PORTC &= ~(1<<PORTC0);
	}
	//resets position when send is done
	if((sensormodul.slaveSend && (sensormodul.position == sensormodul.outDataArray[0]+1))){
		sensormodul.slaveSend = false;
		sensormodul.position = 0;
	}
}

//Internal interrupt called when complete array is received
ISR(PCINT2_vect){
	handleInDataArray();
}
		
//ADC Conversion Complete Interrupt
ISR(ADC_vect)
{	
	sei();
	decadc = ADCH;				//receive ADC value
	spanning = decadc*5/256;	//get rid of 5 V ref (convert signal to voltage)
	if(gyromode){				//if gryomode is true
		if( spanning < gyrovila + 0.08 && spanning > gyrovila - 0.05){	//do nothing if it's noise
		}
		else{
			angle = angle + spanning-gyrovila;			//add up angle
		}
	}
	ADCdone = true;				//Set ADCdone flag to true
}

//USART interrupt
ISR(USART0_RX_vect){
	indata=UDR0;		//receive data
		
	if(indata==startbit){
		RfidCount++;
		sensormodul.outDataArray[0] = 1;
		sensormodul.outDataArray[1] = 'R';
		sensormodul.SPI_Send();				//send outDataArray
		UCSR0B &= ~(1<<RXCIE0);				//disable USART interrups
	}
}

//Timer overflow interrupt
ISR(TIMER0_OVF_vect){
	asm("");
	ADCSRA |= 1<<ADSC;		//start ADC
}


int main(void)
{	
	sensormodul.SPI_Init();	//run SPI initialization
	Sensor_Init();			//run sensor initialization
	USART_Init(383);		//run USART initialization
	Timer_Init();			//run timer initialization
	
	sei();					// Enable Global Interrupts
	gyrocal();				//run gyro calibration
	ADCSRA |= 1<<ADSC;		// Start Conversion
	
	while(1){				// Wait forever
		
		while(gyromode){		//while gyromode is true
			ADMUX = 0x27;		//set ADMUX to take signal from gyro
			TIMSK0 = 0x01;		//tillåt tidsavbrott
			cli();				//don't allow interrups
			if((angle <= medurs) or (angle >= moturs)){			//if 90 degree turn is complete
				asm("");
				gyromode = false;	//done with gyroreadings
				angle = 0;			//reset angle
				TIMSK0 = 0x00;		//don't allow time interrupts
				savepos = 0;
				ADMUX = 0x20;
				sensormodul.outDataArray[0] = 1;
				sensormodul.outDataArray[1] = 'G';
				sensormodul.SPI_Send();		//send 90 degree turn is complete
			}
			sei();				//allow interrupts
		}
		
		if(ADMUX == 0x20){			//get distance from sensor A0 with conversion formula
			asm("");
			sensor0[savepos]	= round(45.64*pow(spanning,4)-320.2*pow(spanning,3)+830.3*pow(spanning,2)-984.9*spanning+524.4);
			asm("");
		}
		
		if(ADMUX == 0x21){		//get distance from sensor A1 with conversion formula
			asm("");
			sensor1[savepos]	= round(1.031*pow(spanning,4)-68*pow(spanning,3)+364.8*pow(spanning,2)-683.2*spanning+492.2);
		}
				
		if( (ADMUX == 0x22)){	//get distance from sensor A2 with conversion formula
			asm("");
			sensor2[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}
		
        if( (ADMUX == 0x23)){	//get distance from sensor A3 with conversion formula
			asm("");
			sensor3[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}
		
        if( (ADMUX == 0x24)){	//get distance from sensor A4 with conversion formula
			asm("");
			sensor4[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}
		
        if( (ADMUX == 0x25)){	//get distance from sensor A5 with conversion formula
			asm("");
			sensor5[savepos]	= round(12.5*pow(spanning,4)-100.7*pow(spanning,3)+291.4*pow(spanning,2)-367.2*spanning+189.6);
		}

		if(ADMUX == 0x26){		//get distance from sensor A6 with conversion formula
			asm("");
			sensor6[savepos]	= round(19.38*pow(spanning,4)-144.41*pow(spanning,3)+396.49*pow(spanning,2)-497.97*spanning+282.57);
			
		}

		if(ADMUX == 0x26){		//if ADMUX is 0x26
			ADMUX = 0x20;		//reset ADMUX
			savepos++;			//update savepos
		}
		else{
			ADMUX = ADMUX + 1;	//update ADMUX
		}
		
        if(savepos == numOfSamples){	//if all readings are done
            sen0 = average(sensor0);	//get average distance from sensors
            sen1 = average(sensor1);
            sen2 = average(sensor2);
            sen3 = average(sensor3);
            sen4 = average(sensor4);
            sen5 = average(sensor5);
            sen6 = average(sensor6);
            
            sendSensors();				//send sensordata
            
            savepos = 0;				//reset savepos
            
        }

		ADCdone = false;	//set ADCdone flag to false 
		ADCSRA |= 1<<ADSC;	// Start Conversion
		
		while(!ADCdone);	//wait for ADC to be done
	}
}