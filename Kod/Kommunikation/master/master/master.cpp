/*
 * master.cpp
 *
 * Created: 4/7/2014 9:13:57 AM
 *  Author: jened502
 */

// Precompiler flags
#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG == 0

#include <avr/io.h>
#include <avr/interrupt.h>

#endif

#include "masterMap.h"
#include <string.h>
#include "bluetooth.h"
#include "spi.h"
#include "lcd.h"

volatile unsigned int wheelCount=0;
bool toggle = false;
bool ReceiveFromSteer = false;
bool ReceiveFromSensor = false;

Map buffer;
Bluetooth Firefly;
Spi Bus(&Firefly,&buffer);
Lcd Display;


//Timer initialization
void Timer_Init()
{
    TCNT0 = 0x00;			//set timer to 0
    TCCR0B = 0x05;			//pre-scalar 256 p� timer
    TIMSK0 = 0x00;			//dont't allow time-interrups
}



/*
 *	Handeling data from modules
 */

void handleDataFromSteer(){
	
	ReceiveFromSteer=false;
	memcpy(Bus.buffer, Bus.inDataArray,27);
	if(Bus.buffer[1]=='M'){
		memcpy(buffer.mapArea[Bus.buffer[2]],Bus.buffer,27);
		Bus.latestRow = Bus.buffer[2];
		//Confirm received map section
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'm';
		Bus.sendArray(1);
		//If last row, start sending it to PC.
		if(Bus.latestRow == 31){
			Firefly.mapDone = true;
		}
	}
	if(Bus.buffer[1]=='g'){
		if ( !Bus.gyroActive )
		{
			Bus.outDataArray[0] = 2;
			Bus.outDataArray[1] = 'g';
			Bus.outDataArray[2] = 1;
			Bus.sendArray(0);
			Bus.gyroActive = true;
		}
	}
	if(Bus.buffer[1]=='r'){
		if ( !Bus.rfidActive ){
			Bus.outDataArray[0] = 1;
			Bus.outDataArray[1] = 'r';
			Bus.sendArray(0);
			Bus.rfidActive = true;
		}
	}
	if(Bus.buffer[1]=='w'){
		if (!Bus.waitingForW)
		{
			Bus.outDataArray[0] = 1;
			Bus.outDataArray[1] = 'w';
			Bus.waitingForW = true;
			Bus.sendArray(0);
		}
	}
}

void handleDataFromSensor(){
	ReceiveFromSensor=false;
	memcpy(Bus.buffer, Bus.inDataArray,27);
	
	if(Bus.buffer[1] == 'W'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'W';
		wheelCount++;
        if(Bus.waitingForW){
		Bus.sendArray(1);
            Bus.waitingForW = false;
        }
		
	}
	if(Bus.buffer[1] == 'S'){
		// copy data to Bus outDataArray
		memcpy(Bus.outDataArray, Bus.buffer,27);
		Bus.sendArray(1);
		memcpy(Firefly.outDataArray, Bus.buffer,27);
		/*
		Firefly.outDataArray[6] = (wheelCount/100); //plats 4
		Firefly.outDataArray[7] = ((wheelCount/10) %10); // plats 5
		Firefly.outDataArray[8] = (wheelCount % 10); // plats 6
		*/
		Firefly.sendArray();
		//inserts data from all sensors into the Display-buffer
		if (Display.bufferWritten)
		{
			Display.insertSensorValuesToBuffer(Bus.buffer);
			Display.bufferWritten = false;
		}
	}
	if(Bus.buffer[1] == 'G'){
		if (Bus.gyroActive)
		{
			memcpy(Bus.outDataArray, Bus.buffer,2);
			Bus.sendArray(1);
			Bus.gyroActive = false;
		}
	}
	if(Bus.buffer[1] == 'R'){
		if (Bus.rfidActive){
			Bus.outDataArray[0] = 1;
			Bus.outDataArray[1] = 'R';
			Bus.sendArray(1);
			Bus.rfidActive = false;
		}
	}
}

#if DEBUG == 0
/*
 *	INTERUPTS
 */
//bluetooth interupt
ISR(USART0_RX_vect)
{
	Firefly.receiveArray();
}

//Steer module wants to send data
ISR(INT2_vect){
	cli();
	Bus.receiveArray(1);
	ReceiveFromSteer = true;
	sei();
}
//Sensor module wants to send data
ISR(INT1_vect){
	cli();
	Bus.receiveArray(0);
	ReceiveFromSensor = true;
	sei();
}
//Handle auto/manual button event
ISR(INT0_vect){
	cli();
	if(!Firefly.autonom){
		Bus.outDataArray[1]= 'a';
		Bus.outDataArray[3]= 50;
		Firefly.autonom = true;
		Bus.sendArray(1);
		
		Bus.outDataArray[0] = 2;
		Bus.outDataArray[1] = 'g';
		Bus.outDataArray[2] = 2;
		Bus.sendArray(0);
		
	}
	else{
		Bus.outDataArray[1]= 'q';
		Bus.outDataArray[3] = 0;
		Firefly.autonom = false;
		Bus.sendArray(1);
		
		Bus.outDataArray[0] = 2;
		Bus.outDataArray[1] = 'g';
		Bus.outDataArray[2] = 2;
		Bus.sendArray(0);
	}
	sei();
}


//Timer overflow interrupt
ISR(TIMER0_OVF_vect){
    asm("");
    Firefly.rdyForRow = true;
    TIMSK0 = 0x00;		//disable time-interrups
}







#endif


int main(void)
{
	DDRA |= (1<<PORTA4);
    Firefly.setPointer(&Bus,&buffer);
	Timer_Init();
    sei();
    
	for(;;){
		asm("");
		if(ReceiveFromSensor){
			handleDataFromSensor();
			continue;
		}
		if (ReceiveFromSteer){
			handleDataFromSteer();
			continue;
		}
		
		Display.update();
		Firefly.handle();
		
		
		
        // lampan t�nds vid autonom k�rning
		if(Firefly.autonom){
			PORTA |=(1<<PORTA4);
		}
		else{
			PORTA &= ~(1<<PORTA4);
		}
       
        if(Firefly.mapDone && Firefly.rdyForRow){
			Firefly.sendMap();
			if(Firefly.rowToSend > 31){
			
			Firefly.mapDone=false;
			Firefly.rowToSend=0;
			Firefly.rdyForRow=true;
			}
		}
	}
	
}
	