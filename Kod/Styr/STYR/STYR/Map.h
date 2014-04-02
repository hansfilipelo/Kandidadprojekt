#include <string.h>
#include <stdio.h>

class MapSection;

class Map 
{
public:
    Map();
    ~Map();
    
    void setSection(int, int, MapSection*);
	char* getRowAsChar(int);
    
    MapSection* getPos(int,int);
    
protected:
     MapSection* mapArea[32][17];
};
