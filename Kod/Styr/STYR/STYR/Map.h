#include <vector>
#include <string>

class MapSection;

class Map 
{
public:
    Map();
    ~Map();
    
    setSection(int, int, MapSection*);
    
    getPos(int,int);
    
protected:
    mapArea std::vector<std::vector<MapSection*>>;
};