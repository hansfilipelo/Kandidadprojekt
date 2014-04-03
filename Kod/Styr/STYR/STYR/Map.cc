#include "Map.h"
#include "MapSection.h"

using namespace std;

// ------------- Construct -----------------

Map::Map(){
    for (int i = 0; i < 32; i++) {
        for (int it = 0; it < 17; it++) {
            mapArea[i][it] = new MapSection(it,i,this);
        }
    }
}

// -----------------------------------------

Map::~Map(){
    for (int i = 0; i < 32; i++) {
        for (int it = 0; it < 17; it++) {
            delete mapArea[i][it];
        }
    }
}

// ------------------------------------------

MapSection* Map::getPos(int x, int y){
    return mapArea[y][x];
}

// ------------------------------------------

void Map::setSection(int xPos, int yPos, MapSection* inSection){
    mapArea[yPos][xPos] = inSection;
}

// ---------------- getRowAsChar ------------

char* Map::getRowAsChar(int row)
{
	// Char sent to communications unit
	char* output = new char[25];
	// Appending to output
	char temp;
	// String telling type of the object we are interested in.
	char currentType;
    
    // Abstraction for buss communications
    // Sending 19 (0x13) positions of interest
    strcat(output, 0x13);
    // Sending Map data
    strcat(output, "M");
    // Sending row
    strcat(output, row);

	for (int it = 0; it < 17; it++)
	{
        // Type of the block we are looking at
        currentType = this->getPos(it,row)->getType();
        
		if (currentType == "unexplored")
		{
            strcat(output, "u");
		}
		else if (currentType == "empty")
		{
			strcat(output, "e");
		}
		else if (currentType == "closed")
		{
			strcat(output   , "c");
		}
		else if (currentType == "robot")
		{
			strcat(output, "r");
		}
		else if (currentType == "fire")
		{
			strcat(output, "f");
		}
	}
    
    return output;
}