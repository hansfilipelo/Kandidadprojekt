#ifndef _Map_h
#define _Map_h

#ifdef __APPLE__
#define DEBUG 1
#endif

#ifdef DEBUG
//#include <iostream>
#endif

#include "Abstraction.h"
#include <string.h>
#include <stdio.h>

class MapSection;
class Robot;

class Map
{
public:
    // Construct
    Map();
    ~Map();
	
	void setSection(int, int, MapSection*);
	void convertSection(int,int,char);
	char* getRowAsChar(int);
	
	MapSection* getPos(int,int);
    
#ifdef DEBUG
    void printMap();
#endif
	
protected:
	MapSection* mapArea[32][17];
};

#endif
