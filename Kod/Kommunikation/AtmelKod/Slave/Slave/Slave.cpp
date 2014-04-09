/*
 * Slave.cpp
 *
 * Created: 4/1/2014 2:06:34 PM
 *  Author: jened502
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 14.7456E6
#include <util/delay.h>
unsigned char inDataArray[25]; // Data arrives in order, datalength (data.length + codeword + type), codeword, type, data.
unsigned char outDataArray[25];
bool send;
double p = 0;
void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB = (1<<DDB6);
	/* Enable SPI */
	SPCR = (1<<SPE)|(1<<SPIE)|(1<<SPR0);
	SPCR &= ~(1<<MSTR);
	
	//PORTB |= (1<<PORTB4); //Enable pull-up resistor
	
	//dummy läsning för att rensa registret
	SPDR=0x00;
}

void SPIReceiveArray(){
	cli();
	 
	unsigned int length;
	if(send){
	length = outDataArray[0];
	}else{
	length = SPDR;
	inDataArray[0] = length;	
	}
	for(unsigned int i=1 ; i <= length; i++){
		
		while(!(SPSR & (1<<SPIF)));
		
		inDataArray[i] = SPDR;
		SPDR = outDataArray[i];
	}
	sei();
	return;
}


ISR(SPI_STC_vect){
	cli();
	if(send){
	SPDR = outDataArray[1]; //måste göras tidigt av någon anledning annars missas denna.
	}
	SPIReceiveArray();	
	sei();
}



int main(void)
{
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01)|(1<<ISC00);
	
	DDRB |= (1<<DDB2);
	DDRD &= ~(1<<DDD7);
	
	outDataArray[0] = 0x04;
	outDataArray[1] = 'B';
	outDataArray[2] = 'B';
	outDataArray[3] = 'B';
	outDataArray[4] = 'B';
	send =true;
	sei();
	SPI_SlaveInit();
    bool loop = true;
	
	
	while(1)
	{
		if ((PIND==0x80))
		{
			loop = false;
			send =true;
			SPDR = outDataArray[0];
			PORTB |= (1<<PORTB2); //interupt to master
			send = false; 
			_delay_ms(4);
			
		}	
	}
}