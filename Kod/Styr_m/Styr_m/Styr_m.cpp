/*
 * STYR.c
 *
 * Created: 3/27/2014 11:58:55 AM
 *  Author: hanel742 och tobgr602
 */

#define F_CPU 14745600
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "MapSection.h"
#include "Map.h"
#include "Abstraction.h"
#include "Communication.h"
#include "slave.h"

// Intiating global variables
// -----------------------------
// Chooses direction
//static int gear = 0;
//static int speed = 0;
Slave steerModuleSlave;
Slave* slavePointer = &steerModuleSlave;
Communication* abstractionObject = new Communication(slavePointer);
Map* mapPointer = new Map();
Robot* robotPointer = new Robot(16,1,mapPointer,abstractionObject);


#if DEBUG == 0

// Interreupt for bus comm
// -----------------------------

ISR(SPI_STC_vect){
	steerModuleSlave.position++;
	SPDR = steerModuleSlave.outDataArray[steerModuleSlave.position];
	steerModuleSlave.inDataArray[steerModuleSlave.position - 1] = SPDR;
	
	if ((steerModuleSlave.position == (steerModuleSlave.inDataArray[0]+1))&&(steerModuleSlave.inDataArray[0]!= 0)){
		steerModuleSlave.position = 0;
		PORTC |= (1<<PORTC0);
		PORTC &=~(1<<PORTC0);
	}
	if((steerModuleSlave.slaveSend && (steerModuleSlave.position == steerModuleSlave.outDataArray[0]+1))){ //When slave sends, position is not set to zero.
		steerModuleSlave.slaveSend = false;
		steerModuleSlave.position = 0;
	}
}

ISR(PCINT2_vect){
	abstractionObject->handleData();
}

// ---------------------------------

void pwm_init()
{
	OCR2A = 0;
	OCR2B = 0;
	
	TCCR2A |= (1<<WGM21)| (1<<WGM20) | (1<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (0<<COM2B0);
	TCCR2B |= (0<<WGM22) | (0<<CS22) | (1<<CS21) | (1<<CS20);
	
	OCR2A = 0;
	OCR2B = 0;
	// Set Port 20 and 21 as outputs (for PWM)
	// Set port 18 and 19 as outputs (for choosing direction)
	// Set port 16 and 17 as inputs (for manual controllers)
	DDRD = 0xF0;
	
	// Initiate gear as 00
	PORTD |= (0<<PORTD4) | (0<<PORTD5);
}

/*
 // Gearbox, port 17
 ISR(INT0_vect){
 cli();
 if (gear == 0){
 PORTD |= 0x10;
 PORTD &= ~0x20;
 gear = gear + 1;
 }
 else if (gear == 1){
 PORTD &= ~0x10;
 PORTD |= 0x20;
 gear = gear + 1;
 }
 else if (gear == 2){
 PORTD |= 0x10;
 PORTD |= 0x20;
 gear = gear + 1;
 }
 else {
 PORTD &= ~0x10;
 PORTD &= ~0x20;
 gear = 0;
 }
 
 sei();
 }
 */

/*
 // Drive, port 16
 ISR(INT1_vect){
 cli();
 if (speed == 0){
 speed = 25;
 }
 else{
 speed = 0;
 }
 int output = floor(speed * 255 / 100);
 
 OCR2A = output;
 OCR2B = output;
 
 sei();
 }
 */
#endif

// ----------------------------------------
// Main

int main(void)
{
#if DEBUG == 0
    // Set up interrupts
	cli();
	MCUCR = 0b00000000;
	EIMSK = 0b00000011;
	EICRA = 0b00001111;
	SMCR = 0x01;
	
    // Initiates PWM
	pwm_init();
    
    // Set up bus comm
    steerModuleSlave.SPI_Init();
	volatile bool watch = abstractionObject->sendMapNow;
    
	sei();
	
#endif
	
	abstractionObject->setRobot(robotPointer);
	
	
    // counter
    int i = 0;
    
    for (;;) {
        // Drive
		if (abstractionObject->manual)
		{
			robotPointer->drive(0);
		}
		else{
			robotPointer->drive(25);
        }
        // Steer along wall
        robotPointer->adjustPosition();
        
        // Look for walls every 500th turn of main loop
        if (i == 500) {
            robotPointer->setFwdClosed();
            robotPointer->setBwdClosed();
            robotPointer->setLeftClosed();
            robotPointer->setRightClosed();
            
            // Update position in map
            robotPointer->updateRobotPosition();
            
            i = 0;
        }
        i++;
        
        if(abstractionObject->sendMapNow){
            asm("");
            abstractionObject->sendMapNow=false;
            abstractionObject->sendMap();
            asm("");
        }
    }
	
	return 0;
}