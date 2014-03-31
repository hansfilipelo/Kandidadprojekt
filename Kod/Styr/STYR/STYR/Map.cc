#include "Map.h"

using namespace std;

Map::Map(){
    mapArea = new vector<vector>;
    
    for (i = 0; i < 32; i++) {
        mapArea.push_back(new vector<MapSection*>);
        
        for (it = 0; it < 17; i++) {
            mapArea.push_back(new MapSection);
        }
    }
}