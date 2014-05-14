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
    
	/* Set baud rate at 115200 */
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

/*Send the 27 elements of the array */
void Bluetooth::sendArray(){
    
    for (unsigned int i=0; i<27; i++){
		transmitByte(outDataArray[i]);
	}
}


/*Send the next row inline to PC, 22 ms delay to ensure data is handled properly
 *before proceeding (avoids overflow in PC-buffer). 
 */

void Bluetooth::sendMap(){
	
		memcpy(outDataArray,mapPointer->mapArea[rowToSend],27);
		sendArray();
		rdyForRow = false;
		TCNT0 = 0x00;		//reset clk counter
		TIMSK0 = 0x01;		//enable time interrupts
		rowToSend++;
}


/*The datahandling function of data recieved from Bluetooth. This should never be
 *called from within a interrupt since datahandling can take some time.
 *asm(""); is used to ensure the precompiler doesn't optimize away code that is
 *critical.
 *pcHandle is a buffer making sure that no data is overwritten. this can happen 
 *since this function does not block interrupts.
 *
 *spiPointer->sendArray(i) sends data to processor i. 
 *i = 0 is the sensormodule and i = 1 is the steermodule.
 *
 *The handling can be read on a case-by-case basis. And an explaination of the
 *different codewords can be found in the document kodord.txt
 */


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
		memcpy(spiPointer->outDataArray,pcHandle,4);
		asm("");
		spiPointer->sendArray(1);
		asm("");
	}
	
	if (pcHandle[1] == 'h'){
		spiPointer->outDataArray[0] = 2;
		spiPointer->outDataArray[1] = 'g';
		spiPointer->outDataArray[2] = 2; //tell sensormodule to stop gyromode.
		
		spiPointer->sendArray(0);
	}
	
	if(pcHandle[1] == 'Y'){
		rdyForRow = true;
	}
	
	if(pcHandle[1] == 'a'){
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
		spiPointer->sendArray(1); //Pass on information to steer.
		spiPointer->outDataArray[0] = 2;
		spiPointer->outDataArray[1] = 'g';
		spiPointer->outDataArray[2] = 2;
		spiPointer->sendArray(0); //tell sensormodule to stop gyromode.
		
	}

	if(inDataArray[1] == 'F'){
		spiPointer->outDataArray[0] = 1;
		spiPointer->outDataArray[1] = 'F';
		spiPointer->sendArray(1);
	}
    if(inDataArray[1]=='P'){
        asm("");
		memcpy(spiPointer->outDataArray,pcHandle,(int)pcHandle[0]+1);
		asm("");
		spiPointer->sendArray(1);
		asm("");

        
    }
}

/*Do not accept interrupts during receiveArray, and do not accept new
 *transmissions on BT while still processing.
 */

void Bluetooth::receiveArray(){
    cli();
	//Set CTS and RTS to 1 ie do not receive new data untill done.
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
