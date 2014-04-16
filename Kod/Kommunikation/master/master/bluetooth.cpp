/*
 * bluetooth.cpp
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917 davha227
 */ 

#include "bluetooth.h"
#include "spi.h"

void Bluetooth::setPointer(Spi* ptr, Map* inMap){
	spiPointer=ptr;
	mapPointer = inMap;
}

void Bluetooth::init(){
    
    unsigned int baud=7;
    
    DDRA |= (1<<PORTA2)|(1<<PORTA3);
	PORTA &= ~((1<<PORTA2)|(1<<PORTA3));
    
	/* Set baud rate */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
    
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
    
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

void Bluetooth::transmitByte( unsigned char data ){
    
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    
    /* Put data into buffer, sends the data */
    UDR0 = data;
}
unsigned char Bluetooth::receiveByte(){
    
    /* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
    
	/* Get and return received data from buffer */
	unsigned char readData = UDR0;
    
	return readData;
}

void Bluetooth::sendArray(){
    
    for (unsigned int i=0; i<25; i++){
		transmitByte(outDataArray[i]);
	}
}

void Bluetooth::sendMap(){
	for(unsigned int i = 0; i<32;i++){
		memcpy(outDataArray,this->mapPointer->getColAsChar(i),25);
		sendArray();
		_delay_ms(2);
	}
}

volatile void Bluetooth::handle(){
    if(!this->Btrec){
		return;
	}
	memcpy(pcHandle,inDataArray,25);
	this->Btrec = false;
	asm("");
	
	if(inDataArray[1] == 'f' || inDataArray[1] == 'r'|| inDataArray[1] == 'b' || inDataArray[1] == 'h' || inDataArray[1] == 'F'){
		asm("");
		outDataArray[0] = pcHandle[0];
		outDataArray[1] = pcHandle[1];
		outDataArray[2] = pcHandle[2];
		outDataArray[3] = pcHandle[3];
		asm("");
		spiPointer->sendArray(1); //send data to module 1 (steer)
		asm("");
	}
}

void Bluetooth::receiveArray(){
    cli();
	//Set CTS and RTS to 1
	PORTA |= (1<<PORTA2)|(1<<PORTA3);
	inDataArray[position] = receiveByte();
	position++;
	if(position == 25){
		Btrec = true;
		asm("");
		position = 0;
	}
	PORTA &= ~((1<<PORTA2)|(1<<PORTA3));
	sei();
}
