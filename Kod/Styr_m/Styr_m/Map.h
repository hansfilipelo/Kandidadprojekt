#ifndef _Map_h
#define _Map_h

#define F_CPU 14745600

#ifndef __AVR_ATmega1284P__
#define TESTING 1
#else
#define TESTING 0
#endif

#if TESTING == 1
#include <iostream>
#endif

#include "node.h"
#include <string.h>
#include <stdio.h>
#include "queueAtmel.h"

class MapSection;
class Robot;

class Map
{
public:
    // Construct
    Map();
    ~Map();
	
	void convertToPathFinding();
	void setSection(int, int, MapSection*);
	void convertSection(int,int,char);
	//char* getColAsChar(int);
	void getColAsChar(int);
	MapSection* getPos(int,int);
    void fillClosedArea();
    bool withinMap(int,int);
    int pathMap[32][17];
    char pathArray[100];
    void aStar(int, int, int, int);
	char colArray[25];
	
	void fillUnexplored();
	
	bool firstTimeMapping = true;
    bool getVisited(int,int);
    void setVisited(int,int,bool);
    
#if TESTING == 1
    void printMap();
    void printPathMap();
    
    void initMap();
#endif
    
protected:
	MapSection* mapArea[32][17];
};

#endif
