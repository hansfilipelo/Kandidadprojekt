#ifndef _MapSection_h
#define _MapSection_h

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

#if TESTING == 1
#include <iostream>
#endif

#include <stdio.h>
#include <string.h>
#include "Abstraction.h"
#include "Communication.h"

class Map;
class Communication;

class MapSection{
public:
    MapSection(int x,int y, Map*);
    MapSection(int x,int y, Map*, char createType);
    
    virtual ~MapSection();
    
    virtual void setX(int coord);
    virtual void setY(int coord);
    virtual void setType(char newType);
    
    virtual int getX();
    virtual int getY();
    virtual char getType();
    
    virtual MapSection* getTop();
    virtual MapSection* getBottom();
    virtual MapSection* getLeft();
    virtual MapSection* getRight();
    
    virtual bool isClosed(int origX, int origY, int fwdCounter, int bwdCounter);
    virtual void cancer();
    
    /*
    virtual void setTop(MapSection*);
    virtual void setBottom(MapSection*);
    virtual void setLeft(MapSection*);
    virtual void setRight(MapSection*);
     */
    
    virtual bool isUnexplored();
    virtual int findUnexplored();
	
protected:
    bool visitedByRobot =false;
	int xCoord;
	int yCoord;
	int step;
    char type='u';
/* Availables types are:
     u = unexplored
     c = closed
	 r = robot
	 e = emptysection
	 f = fire
*/
	Map* mom;
    char next;
    
    volatile bool hasBeenClosed = false;
};


// ------------------------------------------------
// Subclass Robot

class Robot : protected MapSection
{
public:
    Robot(int xPos, int yPos, Map* inMom, Communication* inComm);
    ~Robot();
    
	// RFID
	void setRFID();
	
	// Driving
    void changeGear(char direction);
    void drive();
	void setSpeed(int);
	void setUserSpeed(int);
    void driveBackward(int speed);
    void moveTo(int xPos, int yPos);
	void rotateLeft();
	void rotateRight();
    void stopRotation();
	void robotRotated();
	void setRotateRightActive();
	bool getRotateRightActive();
	void setRotateLeftActive();
	bool getRotateLeftActive();
	
	// Putting sensordata in array
	void fwdLongValueIn(char fwd[3]);
	void bwdLongValueIn(char bwd[3]);
	void fwdShortValueIn(char fwdShort[3]);
    void bwdShortValueIn(char bwdShort[3]);
    void leftLongValueIn(char midLeft[3]);
	void rightFrontValueIn(char right[3]);
    void rightBackValueIn(char right[3]);
	void phiDotValueIn(char phi[3]);
    
	// SLAM (mapping, positioning)
	void setFwdClosed();
	void setBwdClosed();
    void setRightClosed();
    void setLeftClosed();
    
    bool isWallRight();
    bool isCornerRight();
    bool isWallFwd();
    bool isWallLeft();
	bool isWallFwdClose();
    
    void changeDirection(char);
    //char* getColAsChar(int col); colArray
    void getColAsChar(int col);
	char colArray[25];
	
	
    int meanValueArray(char* inputArray, int iterations);
	bool isCornerPassed();
    void updateRobotPosition();
	void setFwdReference();
	void setBwdReference();
	void backToStart();
	bool startExplore = false;
    
    int getRightDifference();
    int getUserSpeed();
	
	bool wheelHasTurned = false;
    
    // Automatic control
    void turn(int pd); //Positive or negative value will decide left or right
    void adjustPosition();
    
	// wait for new data before doing stuff
	void waitForNewData();
	
    void setControlParameters(double, double, int, int, int);
	
	volatile bool newData = false;
	volatile bool okayToClose = true;
    
protected:
    int getRightDistance();
	int getLeftDistance();
	int getFwdDistance();
	int getBwdDistance();
	
    
	int fwdLongSensor;
    int bwdLongSensor;
    int fwdShortSensor;
    int bwdShortSensor;
	
    int rightFrontSensor;
    int rightBackSensor;
    int leftMidSensor;
	
	int fwdDiff;
	int bwdDiff;
	
	bool rotateActive;
	
	char direction = 'f';
    char gear = 'f';
    char validSensor = 'N'; //No valid from start.
    
    int fwdReference;
    int bwdReference;
    char determineValidSensor();
    int leftFrontReference;
    int leftBackReference;
    int rightFrontReference;
    int rightBackReference;
    
    
    //Values concerning automatic control
    
    int movementSpeed;
    int Ref; //Reference value for control
    volatile double Kd; //Differentiation coeff.
    volatile double Kp; //Proportional coeff.
    
    int previousRightError = 0;
    int previousLeftError = 0;
	long int robotTempPd;
    
    MapSection* previousSection = NULL;
    Communication* commObj = NULL;
	
	// Parameters for detecting map sections
	int fwdRefLong;
	int bwdRefLong;
	int fwdRefShort;
	int bwdRefShort;
	
	//Paramater that determines if robot should stop after one segment.
	bool haltAfterSection = false;
	
	//Paramaters for wall and corner detection
	int rightCornerFront = 30;
	int rightCornerBack = 30;
	int rightWallFront = 30;
	int rightWallBack = 30;
	
	int speed = 0;
	int userSpeed = 0;
    int trimLeft;
    int trimRight;
    char currentGear = 'f';
	
	bool rotateRightActive;
	bool rotateLeftActive;
	
    };

#endif