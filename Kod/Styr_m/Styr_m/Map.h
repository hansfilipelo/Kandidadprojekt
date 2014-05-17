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
#include "Abstraction.h"
#include <string.h>
#include <stdio.h>
#include <queue>

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
	//char* getColAsChar(int);
	void getColAsChar(int);
	MapSection* getPos(int,int);
    void fillClosedArea();
    bool withinMap(int,int);
    void convertToPathFinding();
    int pathMap[32][17];
    char pathArray[100];
    void aStar(const int &, const int &,const int &, const int &);
	char colArray[25];
    
#if TESTING == 1
    void printMap();
    void printPathMap();
    
    void initMap();
#endif
    
protected:
	MapSection* mapArea[32][17];
};

#endif
