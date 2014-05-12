#include "masterMap.h"

using namespace std;

// ------------------------------------------

#if DEBUG == 1
void Map::printMap(){    
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            cout << *this->getPos(i,it) << " ";
        }
        cout << endl;
    }
}

#endif