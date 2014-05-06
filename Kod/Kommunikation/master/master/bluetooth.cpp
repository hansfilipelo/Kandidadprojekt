/*
 * bluetooth.cpp
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917 davha227
 */ 

#include "bluetooth.h"
#include "spi.h"

Bluetooth::Bluetooth(){
	init();
}

void Bluetooth::setPointer(Spi* ptr, Map* inMap){
	spiPointer = ptr;
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
    
    for (unsigned int i=0; i<27; i++){
		transmitByte(outDataArray[i]);
	}
}

void Bluetooth::sendMap(){
	for(unsigned int i = 0; i<32;i++){
		memcpy(outDataArray,mapPointer->mapArea[i],27);
		sendArray();
		_delay_ms(27);
	}
}

volatile void Bluetooth::handle(){
    if(!this->Btrec){
		return;
	}
	memcpy(pcHandle,inDataArray,27);
	this->Btrec = false;
	asm("");
	
	if(pcHandle[0]==30 || pcHandle[1] == 30){
		position = 0;
		return;
	}
    
	
	if(pcHandle[1] == 'f' || pcHandle[1] == 'r'|| pcHandle[1] == 'b' || pcHandle[1] == 'h' || pcHandle[1] == 'G'){
		asm("");
		spiPointer->outDataArray[0] = pcHandle[0];
		spiPointer->outDataArray[1] = pcHandle[1];
		spiPointer->outDataArray[2] = pcHandle[2];
		spiPointer->outDataArray[3] = pcHandle[3];
		asm("");
		spiPointer->sendArray(1); //send data to module 1 (steer)
		asm("");
	}

	if(pcHandle[1] == 'a'){
		if(autonom){
			spiPointer->outDataArray[0] = 1;
			spiPointer->outDataArray[1] = 'q';
			autonom = false;
		}
		else{
			spiPointer->outDataArray[0] = 1;
			spiPointer->outDataArray[1] = 'a';
			autonom = true;
		}
		spiPointer->sendArray(1);
	}

	if(inDataArray[1] == 'F'){
		getMap = true;
	}
    if(inDataArray[1]=='P'){
        asm("");
		spiPointer->outDataArray[0] = pcHandle[0];
		spiPointer->outDataArray[1] = pcHandle[1];
		spiPointer->outDataArray[2] = pcHandle[2];
		spiPointer->outDataArray[3] = pcHandle[3];
        spiPointer->outDataArray[4] = pcHandle[4];
		spiPointer->outDataArray[5] = pcHandle[5];
		spiPointer->outDataArray[6] = pcHandle[6];
		spiPointer->outDataArray[7] = pcHandle[7];
        spiPointer->outDataArray[8] = pcHandle[8];
		spiPointer->outDataArray[9] = pcHandle[9];
		spiPointer->outDataArray[10] = pcHandle[10];
		spiPointer->outDataArray[11] = pcHandle[11];
        spiPointer->outDataArray[12] = pcHandle[12];
        spiPointer->outDataArray[13] = pcHandle[13];
        
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
	if(position == 27){
		Btrec = true;
		asm("");
		position = 0;
	}
	PORTA &= ~((1<<PORTA2)|(1<<PORTA3));
	sei();
}
