#include "Map.h"
#include "MapSection.h"

using namespace std;

// ------------- Construct -----------------

Map::Map(){
	
	// Create 32 X by 17 Y
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
    return mapArea[x][y];
}

// ------------------------------------------

void Map::setSection(int xPos, int yPos, MapSection* inSection){
    mapArea[xPos][yPos] = inSection;
}

// ------------------------------------------

void Map::convertSection(int xPos, int yPos, char inType){
	mapArea[xPos][yPos]->setType(inType);
}

// ---------------- getRowAsChar ------------

char* Map::getRowAsChar(int row)
{
	// Char sent to communications unit
	char* output = new char[25];
	// String telling type of the object we are interested in.
	char currentType;
    
    // Abstraction for buss communications
    // Sending 34 (0x22) positions of interest
    int crap = 34;
    strcat(output, intToChar(crap));
    // Sending Map data
    strcat(output, "M");
    // Sending row
    strcat(output, intToChar(row));

	for (int it = 0; it < 32; it++)
	{
        // Type of the block we are looking at
        currentType = this->getPos(it,row)->getType();
        
		if (currentType == 'u')
		{
            strcat(output, "u");
		}
		else if (currentType == 'e')
		{
			strcat(output, "e");
		}
		else if (currentType == 'c')
		{
			strcat(output   , "c");
		}
		else if (currentType == 'r')
		{
			strcat(output, "r");
		}
		else if (currentType == 'f')
		{
			strcat(output, "f");
		}
	}
    
    return output;
}

#if DEBUG == 1
void Map::printMap(){
    for (int i = 0; i < 32; i++) {
        cout << endl;
        for (int it = 0; it < 17; it++) {
            cout << mapArea[i][it]->getType() << " ";
        }
    }
    cout << endl;
}
#endif