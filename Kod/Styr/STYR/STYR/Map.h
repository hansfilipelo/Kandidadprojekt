#include <vector>
#include <string>

class MapSection;

class Map 
{
public:
    Map();
    ~Map();
    
    void setSection(int, int, MapSection*);
    
    MapSection* getPos(int,int);
    
protected:
    std::vector<std::vector<MapSection*>> mapArea;
};