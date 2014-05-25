/******************************************************
*
*Code was produced as part of the project MapMaster2001
*
*File: bluetooth.cpp
*Purpose: Class for functions and buffers concerning bluetooth
*
*
********************************************************/
 
#include "bluetooth.h"
#include "spi.h"

/*
 *	Constructor
 */

Bluetooth::Bluetooth(){
	init();
}


/*
 *	Give class pointers to necessary objects. spi and map.
 */


void Bluetooth::setPointer(Spi* ptr, Map* inMap){
	spiPointer = ptr;
	mapPointer = inMap;
}


/*
 *	Initialization of USART on atmega1284p. And USART interrupt. 
 */


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



/*
 *	Send one byte as soon as transmit buffer is empty.
 */


void Bluetooth::transmitByte( unsigned char data ){
    
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    
    /* Put data into buffer, sends the data */
    UDR0 = data;
}


/*
 *	Read byte from Bluetooth modem. 
 */


unsigned char Bluetooth::receiveByte(){
    
    /* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
    
	/* Get and return received data from buffer */
	unsigned char readData = UDR0;
    
	return readData;
}


/*
 * 	Perform transmitByte for each element in outDataArray.
 */


void Bluetooth::sendArray(){
    
    for (unsigned int i=0; i<27; i++){
		transmitByte(outDataArray[i]);
	}
}


/*
 *	Send one row of map. Start timer and enable the interrupt. 
 *	this function will not be used again before that interrupt fires. 
 */


void Bluetooth::sendMap(){
	memcpy(outDataArray,mapPointer->mapArea[rowToSend],27);
    sendArray();
    rdyForRow = false;
    TCNT0 = 0x00;		//reset clk counter
    TIMSK0 = 0x01;		//enable time interrupts
    rowToSend++;
}


/*
 *	Handle data that has arrived.
 */


volatile void Bluetooth::handle(){
    //only execute rest of function if new data is available.
	if(!this->Btrec){
		return;
	}
	//copy data to another container while handling it.
	memcpy(pcHandle,inDataArray,27);
	this->Btrec = false;
	asm("");
	
	
	//commands that are purely passed on to steer.
	if(pcHandle[1] == 'f' || pcHandle[1] == 'r'|| pcHandle[1] == 'b' || pcHandle[1] == 'h' || pcHandle[1] == 'G'){
		asm("");
		memcpy(spiPointer->outDataArray,pcHandle,4);
		asm("");
		spiPointer->sendArray(1); //send data to module 1 (steer)
		asm("");
		
		//for the halt command also stop rotation by telling sensormodule to exit gyromode.
		if (pcHandle[1] == 'h'){
			spiPointer->outDataArray[0] = 2;
			spiPointer->outDataArray[1] = 'g';
			spiPointer->outDataArray[2] = 2;
			
			spiPointer->sendArray(0);
		}
	}
	//if a, switch to autonomous mode if in manual and tell steer.
	// if manual switch to manual and tell steer. Also notify sensor to halt gyromode.
	else if(pcHandle[1] == 'a'){
		if(!autonom){
			spiPointer->outDataArray[1] = 'a';
			autonom = true;
		}
		else{
			spiPointer->outDataArray[1] = 'q';
			autonom = false;
		}
		spiPointer->outDataArray[0] =  pcHandle[0];
		spiPointer->outDataArray[2] =  pcHandle[2];
		spiPointer->outDataArray[3] =  pcHandle[3];
		spiPointer->sendArray(1);
		spiPointer->outDataArray[0] = 2;
		spiPointer->outDataArray[1] = 'g';
		spiPointer->outDataArray[2] = 2;
		spiPointer->sendArray(0);
		
	}
     // Send fetch command to Steer so steer starts sending map.;

	else if(inDataArray[1] == 'F'){
		memcpy(spiPointer->outDataArray,pcHandle,(int)pcHandle[0]+1);
        spiPointer->sendArray(1);
	}
	
	// Send new parameters to steermodule.
	
    else if(inDataArray[1]=='P'){
        asm("");
		memcpy(spiPointer->outDataArray,pcHandle,(int)pcHandle[0]+1);
		asm("");
		spiPointer->sendArray(1); //send data to module 1 (steer)
		asm("");

        
    }
	//If neither is true reset Bluetooth incase of miscommunication
    else{
        position = 0;
    }
}

/*
 *  Receive one byte and place it in the corresponding array position. 
 *	if 27 bytes has been received the transmission is done and Btrec 
 *	is set to true for future handling of data.
 */

void Bluetooth::receiveArray(){
    cli();
	//Set CTS and RTS to 1
	PORTA |= (1<<PORTA2)|(1<<PORTA3);
	inDataArray[position] = receiveByte();
    
	
	//if any byte is x a reset command has been sent. Try and reset BT.
    if(inDataArray[position] == 'x'){
        position = 0;
        Btrec = false;
        PORTA &= ~((1<<PORTA2)|(1<<PORTA3));
        sei();
		return;
    }
	
	//increase position. if 27 end this receive cycle.
	position++;
	if(position == 27){
		Btrec = true;
		asm("");
		position = 0;
	}
	//Set CTS and RTS to 0
	PORTA &= ~((1<<PORTA2)|(1<<PORTA3));
	sei();
}
