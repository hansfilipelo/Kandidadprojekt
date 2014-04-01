#include "Map.h"
#include "MapSection.h"

using namespace std;

// ------------- Construct -----------------

Map::Map(){
    for (i = 0; i < 32; i++) {
        for (it = 0; it < 17; i++) {
            mapArea[i][it] = new MapSection(it,i);
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

void setSection(int xPos, int yPos, MapSection* inSection){
    mapArea.at(yPos).at(xPos) = inSection;
}