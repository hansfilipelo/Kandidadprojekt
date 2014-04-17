#include "masterMap.h"

using namespace std;

// ------------------------------------------

// ------------------------------------------
/*
char* Map::getPos(int x, int y){
    return mapArea[x][y];
}

// ---------------- getColAsChar ------------


// --------- SET COL ------------
void Map::setColAsChar(char* inCol)
{
	char* ptr = mapArea[inCol[2]][1];
	memcpy(ptr, inCol,25);
}

// --------- SET POS ------------

void Map::setSection(int xPos, int yPos, char* inSection){
    mapArea[xPos][yPos] = inSection;
}

// --------- DEBUG --------------
*/
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
/*
char* Map::getColAsChar(int col)
{
	// Char sent to computer
	char* output = new char[25];
	
	for (int it = 0; it < 25; it++){
		// Type of the block we are looking at
		output[it] = *this->getPos(col,it);
	}
	
	return output;
	
}
*/