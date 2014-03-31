#include <vector>
#include <string>
#include "MapSection.h"

class Map 
{
public:
    Map();
    ~Map();
    
    setSection(int, int, MapSection*);
    
protected:
    mapArea std::vector<std::vector<MapSection*>>;
};