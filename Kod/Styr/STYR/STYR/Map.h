#include <string.h>

class MapSection;

class Map 
{
public:
    Map();
    ~Map();
    
    void setSection(int, int, MapSection*);
    
    MapSection* getPos(int,int);
    
protected:
     MapSection* mapArea [32][17];
};