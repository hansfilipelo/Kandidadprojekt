#ifndef _Map_h
#define _Map_h

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
	
protected:
	MapSection* mapArea[32][17];
};

#endif
