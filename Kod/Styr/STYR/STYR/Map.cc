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
            delete mapArea[i][it];
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

// ---------------- getRowAsChar ------------

char Map::getRowAsChar(int row)
{
	// Char sent to communications unit
	char output;
	// Appending to output
	char temp;
	// String telling type of the object we are interested in.
	string currentType;

	for (it = 0; it < 17; i++)
	// Type of the block we are looking at
	currentType = this->getPos(it,row).getType();

	{
		if (currentType == "unexplored")
		{
			temp = u;
		}
		else if (currentType == "empty")
		{
			temp = e;
		}
		else if (currentType == "closed")
		{
			temp = c;
		}
		else if (currentType == "robot")
		{
			temp = r;
		}
		else if (currentType == "fire")
		{
			temp = f;
		}

		// Append temp char to output
		strcat(output,temp)
	}
}