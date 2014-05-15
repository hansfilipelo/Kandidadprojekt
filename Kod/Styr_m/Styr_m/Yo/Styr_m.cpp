/*
 * STYR.c
 *
 * Created: 3/27/2014 11:58:55 AM
 *  Author: hanel742 och tobgr602
 */

#define F_CPU 14745600

#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG == 0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#endif

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
/*
 *  When the interrupt fires put your current outData on the SPDR and read from it to the inData.
 *  If we receieved as many bytes as the length specifies fire another interrupt for data handling.
 *  When the slave sends to master it needs to check the outDataArray's length instead.
 */


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

/*
 *  Datahandling interrupt fired from within SPI interrupt.
 */


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
    
	sei();
	
#endif
	
	abstractionObject->setRobot(robotPointer);
	robotPointer->changeGear('f');
	
    // Wait for sensordata before mapping stuff.
    robotPointer->waitForNewData();
	robotPointer->setFwdReference();
	robotPointer->setBwdReference();
    
	// Iterator for mapping
	int i = 0;
    //-----------------------------------------------------
    //right wall following loop
    robotPointer->setFwdClosed();
    robotPointer->setBwdClosed();
    robotPointer->setRightClosed();
    robotPointer->setLeftClosed();
    for (;;) {
        // Manual mode
        if (abstractionObject->getManual()) {
            asm("");
			
			if (robotPointer->getRotateRightActive())
			{
				robotPointer->rotateRight();
				
			}
			else if ( robotPointer->getRotateLeftActive() ){
				robotPointer->rotateLeft();
			}
        }
        // Automatic mode
        else {            
			//----------------------Om kortdistans flyttas fram----------
			if(robotPointer->isCornerRight()){
				while ( !(robotPointer->isCornerPassed()) && !(abstractionObject->getManual())) {
					robotPointer->changeGear('f');
					robotPointer->setSpeed(25);
					robotPointer->drive();
				}
				_delay_ms(200); // This delay ensures that we enter next segment.
				robotPointer->rotateRight();
				while ( !robotPointer->isWallRight() && !(abstractionObject->getManual())) {
					robotPointer->changeGear('f');
					robotPointer->setSpeed(25);
					robotPointer->drive();
				}
			}
            else if(robotPointer->isWallFwd()){
				robotPointer->setSpeed(20);
				robotPointer->changeGear('f');
				while (!robotPointer->isWallFwdClose() && !(abstractionObject->getManual()))
				{
					robotPointer->drive();
				}
				robotPointer->setSpeed(0);
				robotPointer->drive();
				
				if(!robotPointer->isWallRight())
				{
					robotPointer->rotateRight();
                    //Drive forward untill robot has entered
					while (!robotPointer->isWallRight() && !(abstractionObject->getManual())) {
						robotPointer->changeGear('f');
						robotPointer->setSpeed(25);
						robotPointer->drive();
					}
				}
				
				else
				{
					robotPointer->rotateLeft();
				}
                
            }
			else
			{
				if(!robotPointer->isWallRight())
				{
					robotPointer->rotateRight();
				}
				else
				{
					robotPointer->setSpeed(robotPointer->getUserSpeed());
					robotPointer->changeGear('f');
					robotPointer->drive();
					robotPointer->adjustPosition();
				}
			}
		}
    
        // Look for walls every 250th turn of main loop
        if (i == 250) {
			abstractionObject->time1();
            // Update position in map
            robotPointer->updateRobotPosition();
            
            i = 0;
        }
		
		if(abstractionObject->wheelHasTurned){
			robotPointer->moveRobot();
			abstractionObject->wheelHasTurned=false;
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