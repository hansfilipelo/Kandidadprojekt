#include "Map.h"
#include "MapSection.h"

using namespace std;

// ------------- Construct -----------------

Map::Map(){
    mapArea = new vector<vector>;
    
    for (i = 0; i < 32; i++) {
        mapArea.push_back(new vector<MapSection*>);
        
        for (it = 0; it < 17; i++) {
            mapArea.push_back(new MapSection(it,i));
        }
    }
}

// -----------------------------------------

Map::~Map(){
    for (i = 0; i < 32; i++) {
        for (it = 0; it < 17; i++) {
            delete mapArea.at(it).at(i);
        }
    }
}

// ------------------------------------------

MapSection* getPos(int x, int y){
    return mapArea.at(y).at(x);
}

// ------------------------------------------