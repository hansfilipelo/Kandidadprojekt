//
//  node.cpp
//  
//
//  Created by Niklas Ericson on 2014-05-16.
//
//

#include "node.h"



void node::updatePriority(const int & xDest, const int & yDest){
    priority=level+estimate(xDest, yDest)*10; //A*
}

// give better priority to going strait instead of diagonally
// i: direction
void node::nextLevel(const int & i){
    level += (dir==8?(i%2==0?10:14):10);
}

// Estimation function for the remaining distance to the goal.
const int & node::estimate(const int & xDest, const int & yDest){
    static int xd, yd, d;
    xd=xDest-xPos;
    yd=yDest-yPos;
    
    // Euclidian Distance
    d=static_cast<int>(sqrt(xd*xd+yd*yd));
    
    // Manhattan distance
    //d=abs(xd)+abs(yd);
    
    // Chebyshev distance
    //d=max(abs(xd), abs(yd));
    
    return(d);
}

// -------------------