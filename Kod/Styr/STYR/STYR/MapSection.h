#ifndef _MapSection_h
#define _MapSection_h

#include <string.h>

class Map;

class MapSection{
public:
    MapSection(int x,int y, Map*);
    MapSection(int x,int y, Map*, std::string createType);
    
    virtual ~MapSection();
    
	virtual void setX(int coord);
	virtual void setY(int coord);
	virtual void setType(std::string newType);
	
	virtual int getX();
	virtual int getY();
	virtual std::string getType();
	
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
    std::string type="undefined";
/* Availables types are:
     unexplored
     closed
	 robot
	 emptysection
	 emptysection
	 fire
*/
	Map* mom = nullptr;
    std::string next;
};


// ------------------------------------------------
// Subclass Robot

class Robot : protected MapSection
{
public:
    Robot(int xPos, int yPos, Map* inMom);
    
    void changeDirection(std::string direction);
    void drive(int speed);
    void moveTo(int xPos, int yPos);
    void sensorValuesIn(double fwd, double bwd, double left, double leftLong, double right, double phiDot);
    
    
protected:
    double fwdSensor[10];
    double bwdSensor[10];
    double leftSensor[10];
    double rightSensor[10];
    double phiDot[10];
    
    };

#endif