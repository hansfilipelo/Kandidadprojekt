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

bool toggle = false;
bool ReceiveFromSteer = false;
bool ReceiveFromSensor = false;

char errorArray1[27];
char errorArray2[27];
char errorArray3[27];

Map buffer;
Bluetooth Firefly;
Spi Bus(&Firefly,&buffer);
Lcd Display;

/*
 *	Handeling data from modules
 *  The specific codewords are specified in the document kodord.txt.
 *  sendArray(i) sends to the i:th module. 
 *  i=0 is the sensorModule and i=1 is the steerModule.
 *  These functions are not called on from interrupts.
 */

//Timer initialization
void Timer_Init()
{
	TCNT0 = 0x00;			//set timer to 0
	TCCR0B = 0x04;			//pre-scalar 256 på timer
	TIMSK0 = 0x00;			//dont't allow time-interrups
}

//Timer overflow interrupt
ISR(TIMER0_OVF_vect){
	asm("");
	Firefly.rdyForRow = true;
	TIMSK0 = 0x00;		//don't allow time-interrups
}

void handleDataFromSteer(){
	ReceiveFromSteer=false;
	memcpy(Bus.bufferSteer, Bus.inDataArray,27);
	
	if(Bus.bufferSteer[1] == 'T'){
		memcpy(Firefly.outDataArray,Bus.bufferSteer,27);
		Firefly.sendArray();
	}
	if(Bus.bufferSteer[1]=='M'){
		memcpy(buffer.mapArea[Bus.bufferSteer[2]],Bus.bufferSteer,27);
		Bus.latestRow = Bus.bufferSteer[2];
		//Confirm received map section
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'm';
		Bus.sendArray(1);
		//If last row, start sending it to PC.
		if(Bus.latestRow == 31){
			Firefly.mapDone = true;
		}
	}
	if(Bus.bufferSteer[1]=='g'){
		Bus.outDataArray[0] = 2;
		Bus.outDataArray[1] = 'g';
		Bus.outDataArray[2] = 1;
		Bus.sendArray(0);	
	}
	if(Bus.bufferSteer[1]=='r'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'r';
		Bus.sendArray(0);
	}
	if(Bus.bufferSteer[1]=='d'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'd';
		Bus.sendArray(0);
	}
}	

void handleDataFromSensor(){
	ReceiveFromSensor=false;
	memcpy(Bus.bufferSensor, Bus.inDataArray,27);
	
	if(Bus.notSensorData && ((Bus.bufferNonSensor[1]=='G') || (Bus.bufferNonSensor[1]=='D') || (Bus.bufferNonSensor[1]=='R'))){
		memcpy(Bus.outDataArray, Bus.bufferNonSensor,Bus.bufferNonSensor[0]+1);
		Bus.sendArray(1);
		Bus.notSensorData = false;
	}
	else if(!Bus.notSensorData && Bus.bufferSensor[1] == 'S'){
		// copy data to Bus outDataArray
		memcpy(Bus.outDataArray, Bus.bufferSensor,27);
		Bus.sendArray(1);
		
		memcpy(Firefly.outDataArray, Bus.bufferSensor,27);
		Firefly.sendArray();

		//inserts data from all sensors into the Display-buffer
		if (Display.bufferWritten)
		{
			Display.insertSensorValuesToBuffer(Bus.bufferSensor);
			Display.bufferWritten = false;
		}
	}
	else{
		memcpy(errorArray1,Bus.bufferSensor,27);
		memcpy(errorArray2,Bus.bufferNonSensor,27);
		asm("");
	}	
}

#if DEBUG == 0
/*
*	INTERUPTS
*/


/*
 *  Interrupt fires when data from BT-module arrive.
 */


ISR(USART0_RX_vect)
{
	Firefly.receiveArray();
}

/*
 *  Interrupt from steermodule telling us to fetch data from them.
 *  Sets bool fro receiving true so data can be handled in main.
 */

ISR(INT2_vect){
	cli();
	Bus.receiveArray(1);
	ReceiveFromSteer = true;
	sei();
}
/*
 *  Interrupt from sensormodule telling us to fetch data from them.
 *  Sets bool fro receiving true so data can be handled in main.
 */

ISR(INT1_vect){
	cli();
	Bus.receiveArray(0);
	if(Bus.inDataArray[1] != 83){
		if(Bus.inDataArray[1] != 'G' && Bus.inDataArray[1] != 'D' && Bus.inDataArray[1] != 'R'){
		memcpy(errorArray3,Bus.inDataArray,27);
		asm("");
		return;
		}
		memcpy(Bus.bufferNonSensor,Bus.inDataArray,Bus.inDataArray[0]+1);
		Bus.notSensorData = true;
	}
	ReceiveFromSensor = true;
	sei();
}

/*
 *  Interrupt from manual/auto button. Switches mode.
 */

ISR(INT0_vect){
	cli();
	if(!Firefly.autonom){
		Bus.outDataArray[1]= 'a';
		Bus.outDataArray[3]= 50;
		Firefly.autonom = true;
		Bus.sendArray(1);
		//get out from gyromode
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
		//get out from gyromode
		Bus.outDataArray[0] = 2;
		Bus.outDataArray[1] = 'g';
		Bus.outDataArray[2] = 2;
		Bus.sendArray(0);
	}
	sei();
}

#endif

/*
 *  MainLoop, checks which receive bool that is true and handles the data. 
 *  also once per iteration updates the display (either position or character, see
 *  lcd.cpp for details). If the bool mapDone = true the mainloop also sends one 
 *  row of map-data to PC.
 */
int main(void)
{
	Timer_Init();
	DDRA |= (1<<PORTA4);
    Firefly.setPointer(&Bus,&buffer);
	sei();

	for(;;){
		asm("");
		Firefly.handle();
		if (ReceiveFromSteer){
			handleDataFromSteer();		
		}
		if(ReceiveFromSensor){
			handleDataFromSensor();
		}
		
		/* In its current implementation this function is delaying the master by 25  ms. 
				In that time the master might miss to relay for instance the stopRotation command.*/
		
		if(Firefly.mapDone && Firefly.rdyForRow ){
			Firefly.sendMap();
			
			// When all sent - do this
			if(Firefly.rowToSend > 31){
				Firefly.mapDone = false;
				Firefly.rowToSend = 0;
				Firefly.rdyForRow = true;
			}
		}
		
		
		Display.update();
		
		
		
		 // this turns on a LED when in auto-mode.
		if(Firefly.autonom){
			PORTA |=(1<<PORTA4);
		}
		else{
			PORTA &= ~(1<<PORTA4);
		}
	}
	
}