#ifndef _MapSection_h
#define _MapSection_h

#include "Abstraction.cc"
#include <string.h>
#include <stdio.h>

class MapSection;
class Robot;

class Map
{
	public:
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
