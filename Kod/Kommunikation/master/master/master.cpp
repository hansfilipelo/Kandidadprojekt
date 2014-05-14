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

void handleDataFromSteer(){
	
	ReceiveFromSteer=false;
	memcpy(Bus.buffer, Bus.inDataArray,27);
	
	if(Bus.buffer[1] == 'T'){
		memcpy(Firefly.outDataArray,Bus.buffer,27);
		Firefly.sendArray();
	}
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
		Bus.outDataArray[0] = 2;
		Bus.outDataArray[1] = 'g';
		Bus.outDataArray[2] = 1;
		Bus.sendArray(0);	
	}
	if(Bus.buffer[1]=='r'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'r';
		Bus.sendArray(0);
	}
	if(Bus.buffer[1]=='d'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'd';
		Bus.sendArray(0);
	}
}	

void handleDataFromSensor(){
	ReceiveFromSensor=false;
	memcpy(Bus.buffer, Bus.inDataArray,27);
	
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
	if(Bus.buffer[1] == 'G'){
		memcpy(Bus.outDataArray, Bus.buffer,2);
		Bus.sendArray(1);
	}
	if(Bus.buffer[1] == 'R'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'R';
		Bus.sendArray(1);
	}
	if(Bus.buffer[1] == 'D'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'D';
		Bus.sendArray(1);
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

#endif

/*
 *  MainLoop, checks which receive bool that is true and handles the data. 
 *  also once per iteration updates the display (either position or character, see
 *  lcd.cpp for details). If the bool mapDone = true the mainloop also sends one 
 *  row of map-data to PC.
 */
int main(void)
{
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
				In that time the master might miss to relay for instance the stopRotation command.
		
		if(Firefly.mapDone){
			Firefly.sendMap();
			if(Firefly.rowToSend > 31){
				Firefly.mapDone = false;
				Firefly.rowToSend = 0;
				Firefly.rdyForRow =true;
			}
		}
		
		*/
		
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