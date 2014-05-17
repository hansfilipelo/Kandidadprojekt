//
//  node.h
//
//
//  Created by Niklas Ericson on 2014-05-16.
//
//

#ifndef NODE
#define NODE

#if TESTING == 1
#include <iostream>
#include <iomanip>
#include <ctime>
#include <queue>
#endif

#include <string.h>
#include <math.h>


class node
{    
    // current position
    int xPos;
    int yPos;
    // total distance already travelled to reach the node
    int level;
    // priority=level+remaining distance estimate
    int priority;  // smaller: higher priority
    int dir = 4;
    
public:
    node(int xp, int yp, int d, int p){xPos=xp; yPos=yp; level=d; priority=p;}
    
    int getxPos() const {return xPos;}
    int getyPos() const {return yPos;}
    int getLevel() const {return level;}
    int getPriority() const {return priority;}
    void updatePriority(const int & xDest, const int & yDest);
    void nextLevel(const int & i);
    const int & estimate(const int & xDest, const int & yDest);
    
};



#endif
