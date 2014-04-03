#ifndef _MapSection_h
#define _MapSection_h

#include <string.h>

class Map;

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
	
	virtual void setTop(MapSection*);
	virtual void setBottom(MapSection*);
	virtual void setLeft(MapSection*);
	virtual void setRight(MapSection*);
	
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
    Robot(int xPos, int yPos, Map* inMom);
    
	// Driving
    void changeDirection(char direction);
    void drive(int speed);
    void moveTo(int xPos, int yPos);
    
	// SLAM (mapping, positioning)
	void sensorValuesIn(double fwd, double bwd, double left, double leftLong, double right, double phiDot);
    
    
protected:
    double fwdSensor[100];
    double bwdSensor[100];
    double leftSensor[100];
    double rightSensor[100];
    double phiDot[100];
    
    };

#endif