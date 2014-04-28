#ifndef _MapSection_h
#define _MapSection_h

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

#if DEBUG == 1
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
    
    /*
    virtual void setTop(MapSection*);
    virtual void setBottom(MapSection*);
    virtual void setLeft(MapSection*);
    virtual void setRight(MapSection*);
     */
    
    virtual bool isUnexplored();
    virtual int findUnexplored();
	
protected:
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
	Map* mom = NULL;
    char next;
};


// ------------------------------------------------
// Subclass Robot

class Robot : protected MapSection
{
public:
    Robot(int xPos, int yPos, Map* inMom, Communication* inComm);
    ~Robot();
    
	// Driving
    void changeGear(char direction);
    void drive(int speed);
    void driveBackward(int speed);
    void moveTo(int xPos, int yPos);
	void rotateLeft();
	void rotateRight();
    void stopRotation();
	
	// Putting sensordata in array
	void fwdValueIn(char*);
	void bwdValueIn(char*);
	void leftFrontValueIn(char*);
    void leftBackValueIn(char*);
    void leftLongValueIn(char*);
	void rightFrontValueIn(char*);
    void rightBackValueIn(char*);
	void phiDotValueIn(char*);
    
	// SLAM (mapping, positioning)
	void setFwdClosed();
	void setBwdClosed();
    void setRightClosed();
    void setLeftClosed();
    
    void changeDirection(char);
    char* getColAsChar(int col);
    
    int meanValueArray(char* inputArray, int iterations);
    void updateRobotPosition();
    
    // Automatic control
    void turn(int pd); //Positive or negative value will decide left or right
    int getRightDifference();
    int getLeftDifference();
    void adjustPosition();
    
    void setControlParameters(double, double, int);
    
    volatile double Kd = 0; //Differentiation coeff.
    volatile double Kp = 0; //Proportional coeff.
	
    
protected:
    int getRightDistance();
    
	char* fwdSensor = new char[100];
    char* bwdSensor = new char[100];
    char* leftFrontSensor = new char[100];
    char* leftBackSensor = new char[100];
    char* rightFrontSensor = new char[100];
    char* rightBackSensor = new char[100];
    char* phiDotSensor = new char[100];
    char* leftLongSensor = new char[100];
	
	int fwdDiff;
	int bwdDiff;
	
	bool rotateActive;
	
	char direction = 'f';
    char gear = 'f';
    
    int fwdReference;
    int bwdReference;
    int leftFrontReference;
    int leftBackReference;
    int rightFrontReference;
    int rightBackReference;
    
    //Values concerning automatic control
    
    int movementSpeed;
    int Ref=5; //Reference value for control
    int previousRightError = 0;
    int previousLeftError = 0;
    
    
    MapSection* previousSection = NULL;
    Communication* commObj = NULL;
    
    };

#endif