/******************************************************
*
*Code was produced as part of the project MapMaster2001
*
*File: masterMap.cpp
*Purpose: container for the mapbuffer on Com-module
*  awaiting to be sent to PC.
*
*
********************************************************/

#include "masterMap.h"

// --------- DEBUG --------------

#if DEBUG == 1

/*
 *	Function for printing the map while debugging in 
 * 	terminal on other platforms than avr.
 */

void Map::printMap(){    
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            cout << *this->getPos(i,it) << " ";
        }
        cout << endl;
    }
}

#endif
