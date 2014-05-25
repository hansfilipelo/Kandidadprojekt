/******************************************************
*
*Code was produced as part of the project MapMaster2001
*
*File: master.cpp
*Purpose: main code for the communication-module.
*
*Created by: Niklas Ericson (niker917), David Habrman (davha227)
*	Jens Edhammer (jened502), Hans-Filip Elo (hanel742), 
*	Tobias Grundström (tobgr602), Erik Ekelund (eriek984)
*
********************************************************/

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
    TCCR0B = 0x05;			//pre-scalar 256 pŒ timer
    TIMSK0 = 0x00;			//dont't allow time-interrups
}



/*
 *	Handeling data from modules
 */

void handleDataFromSteer(){
	
	ReceiveFromSteer=false;
	memcpy(Bus.buffer, Bus.inDataArray,27);
	
	//Maprow from steer.
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

	//used for time measurment. Pass on to PC.
	if(Bus.buffer[1]=='t'){
		memcpy(Firefly.outDataArray, Bus.buffer,27);
		Firefly.sendArray();
	}
	
	//request gyromeasurment.
	if(Bus.buffer[1]=='g'){
			Bus.outDataArray[0] = 2;
			Bus.outDataArray[1] = 'g';
			Bus.outDataArray[2] = 1;
			Bus.sendArray(0);
			Bus.gyroActive = true;
	}
	
	//Ask sensormodule to reactivate the rfid sensor.
	if(Bus.buffer[1]=='r'){
		if ( !Bus.rfidActive ){
			Bus.outDataArray[0] = 1;
			Bus.outDataArray[1] = 'r';
			Bus.sendArray(0);
			Bus.rfidActive = true;
		}
	}
	
	//Ask sensormodule to reactivate wheelsensor.
	if(Bus.buffer[1]=='w'){
			Bus.outDataArray[0] = 1;
			Bus.outDataArray[1] = 'w';
			Bus.waitingForW = true;
			Bus.sendArray(0);
	}
}

/*
 *	Handeling data from sensormodule.
 */
 
void handleDataFromSensor(){
	ReceiveFromSensor=false;
	memcpy(Bus.buffer, Bus.inDataArray,27);
	
	//Wheelsensor indicating we traveled 40 cm. Notify steer.
	if(Bus.buffer[1] == 'W'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'W';
		wheelCount++;
        if(Bus.waitingForW){
		Bus.sendArray(1);
            Bus.waitingForW = false;
        }
		
	}
	
	//RFID detected. Notify steer.
	if(Bus.buffer[1] == 'R'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'R';
		Bus.sendArray(1);
	}
	
	//Sensordata. Send to steer, PC and Display buffer if it is ready.
	if(Bus.buffer[1] == 'S'){
		// copy data to Bus outDataArray
		memcpy(Bus.outDataArray, Bus.buffer,27);
		Bus.sendArray(1);
		memcpy(Firefly.outDataArray, Bus.buffer,27);
		Firefly.sendArray();
		//inserts data from all sensors into the Display-buffer
		if (Display.bufferWritten)
		{
			Display.insertSensorValuesToBuffer(Bus.buffer);
			Display.bufferWritten = false;
		}
	}
	
	//Gyro says 90 degree turn done. Notify steer.
	if(Bus.buffer[1] == 'G'){
		if (Bus.gyroActive)
		{
			memcpy(Bus.outDataArray, Bus.buffer,2);
			Bus.sendArray(1);
			Bus.gyroActive = false;
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
	asm("");
	Bus.receiveArray(0);
	asm("");
	ReceiveFromSensor = true;
	asm("");
}

ISR(PCINT2_vect){
//deal with gyro and wheel and rfid	
//this function has been moved to SPI-bus. 
}


//Handle auto/manual button event
ISR(INT0_vect){
	cli();
	if(!Firefly.autonom){
		
		//Send auto with speed 50 to steer.
		Bus.outDataArray[1]= 'a';
		Bus.outDataArray[3]= 50;
		Firefly.autonom = true;
		Bus.sendArray(1);
		
		//notify steer to terminate gyromode.
		Bus.outDataArray[0] = 2;
		Bus.outDataArray[1] = 'g';
		Bus.outDataArray[2] = 2;
		Bus.sendArray(0);
		
	}
	else{
		//send manual with speed 0 to steer.
		Bus.outDataArray[1]= 'q';
		Bus.outDataArray[3] = 0;
		Firefly.autonom = false;
		Bus.sendArray(1);
		
		//notify steer to terminate gyromode.
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
	//allow another maprow to be sent.
    Firefly.rdyForRow = true;
    TIMSK0 = 0x00;		
}
#endif


int main(void)
{

	DDRA |= (1<<PORTA4); //set port A4 as output for LED.
    Firefly.setPointer(&Bus,&buffer); //construct bluetooth class.
	Timer_Init(); // enable the timer.
    sei();
    
	for(;;){
		asm("");
		if(ReceiveFromSensor){ //if new data exists handle it.
			handleDataFromSensor();
			continue;
		}
		if (ReceiveFromSteer){ //if new data exists handle it.
			handleDataFromSteer();
			continue;
		}
		
		Display.update(); //try and update the display if it is ready. 
		Firefly.handle(); //if new data exists handle it.
		
		
		
        //LED lights when robot is exploring autonmously. 
		if(Firefly.autonom){
			PORTA |=(1<<PORTA4);
		}
		else{
			PORTA &= ~(1<<PORTA4);
		}
       
        if(Firefly.mapDone && Firefly.rdyForRow){ //if new map available and timer has fired since last transmission. Send another row.
			Firefly.sendMap();
			if(Firefly.rowToSend > 31){
			
			Firefly.mapDone=false;
			Firefly.rowToSend=0;
			Firefly.rdyForRow=true;
			}
		}
	}
	
}
	