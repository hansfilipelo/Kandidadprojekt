/*
 * STYR.c
 *
 * Created: 3/27/2014 11:58:55 AM
 *  Author: hanel742 och tobgr602
 */

#define F_CPU 14745600

#ifndef __AVR_ATmega1284P__
#define TESTING 1
#else
#define TESTING 0
#endif

#if TESTING == 0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include "./MapSection.h"
#include "./Map.h"
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

#if TESTING == 0

// Interreupt for bus comm
// -----------------------------

ISR(BADISR_vect){
	robotPointer->setSpeed(0);
	robotPointer->drive();
	
	volatile int p;
	p++;
	asm("");
	
}

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

#endif

// ----------------------------------------
// Main

int main(void)
{
#if TESTING == 0
    // Set up interrupts
	cli();
	//MCUCR = 0b00000000;
	//EIMSK = 0b00000011;
	//EICRA = 0b00001111;
	//SMCR = 0x01;
	
	
    // Initiates PWM
	pwm_init();
	
	
	abstractionObject->setRobot(robotPointer);
    robotPointer->changeDirection('f');
	robotPointer->changeGear('f');
	robotPointer->setSpeed(0);
	robotPointer->drive();
    // Set up bus comm
    steerModuleSlave.SPI_Init();
    
	sei();
	
#endif
	
	robotPointer->waitForNewData(); 
	robotPointer->waitForNewData();
	robotPointer->setFwdReference();
	robotPointer->setBwdReference();
    
	// Iterator for mapping
	int i = 0;
    //-----------------------------------------------------
    //right wall following loop
	if((robotPointer->RFIDmode)&&(robotPointer->getBwdDistance() < 30)){
		robotPointer->setBwdClosed();
	}
	if((robotPointer->RFIDmode)&&(robotPointer->getFrontRightDistance() < 20)){
		robotPointer->setRightClosed();
	}
	else{
		robotPointer->setFwdClosed();
	    robotPointer->setBwdClosed();
	    robotPointer->setRightClosed();
		robotPointer->setLeftClosed();
	}
	bool tst = true;
    
    abstractionObject->sendMap();
    
	abstractionObject->reactivateWheelSensor();
	
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
			if(!robotPointer->startExplore){
				/*------------------ HÖGERFÖLJNNG ---------------------- 
                 -------------------------------------------------------
                 -------------------------------------------------------*/
				
				if(tst){
					// get the route
		
					mapPointer->convertToPathFinding();
					int xStart = robotPointer->getX();
					int yStart = robotPointer->getY();
					robotPointer->findFinishPos();
					int xFinish = robotPointer->getFinishX();
					int yFinish = robotPointer->getFinishY();					
					
					//mapPointer->aStar(1,16,5,16);
	
					//abstractionObject->sendAStar(mapPointer->pathArray);
 					tst = false;
				}
				
				//lets try with only ifs
				if(robotPointer->isCornerRight()){
					while ( !(robotPointer->isCornerPassed()) && !(abstractionObject->getManual())) {
						robotPointer->changeGear('f');
						robotPointer->setSpeed(20);
						robotPointer->drive();
					}
					//_delay_ms(25); // This delay ensures that we enter next segment.
					robotPointer->rotateRight();
					//said !iswallright lets try iscornerpassed
					while ( robotPointer->isCornerPassed() && !(abstractionObject->getManual())) {
						robotPointer->changeGear('f');
						robotPointer->setSpeed(20);
						robotPointer->drive();
					}
				}
			
				//was elseif before
				if(robotPointer->isWallFwd()){
					robotPointer->setSpeed(25);
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
					
						// stod robotPointer->getUserSpeed() ist för 35
						robotPointer->setSpeed(25);
						robotPointer->changeGear('f');
						robotPointer->drive();
						robotPointer->adjustPosition();
					}
				}
			}
			else{
				/*--------------KARTLÄGGNING----------------------
                -------------------------------------------------*/
				if(mapPointer->firstTimeMapping){
					mapPointer->firstTimeMapping = false;
					//Sets start and finnish coordinates
					int xStart = robotPointer->getX();
					int yStart = robotPointer->getY();
					robotPointer->findFinishPos();
					int xFinish = robotPointer->getFinishX();
					int yFinish = robotPointer->getFinishY();
				
					// get the route
					mapPointer->convertToPathFinding();
					mapPointer->aStar(xStart,yStart,xFinish,yFinish);
					abstractionObject->sendAStar(mapPointer->pathArray);
					robotPointer->goToAStar();
					
				}//drive around this island 
				if(robotPointer->foundIsland){
					robotPointer->handleIsland();
				}
			}
		}
		// Look for walls every 500th turn of main loop
		if (i == 3000) {
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