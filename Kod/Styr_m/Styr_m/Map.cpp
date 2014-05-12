#include "Map.h"
#include "MapSection.h"

using namespace std;

// ------------- Construct -----------------

Map::Map(){
	
	// Create 32 X by 17 Y
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            mapArea[i][it] = new MapSection(i,it,this); // it, i to i, it 
        }
    }
}

// -----------------------------------------

Map::~Map(){
    
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
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
	this->getPos(xPos,yPos)->setType(inType);
}

// ---------------- getColAsChar ------------

char* Map::getColAsChar(int col)
{
	// Char sent to communications unit
	char* output = new char[25];
	// String telling type of the object we are interested in.
    
    // Abstraction for buss communications
    // Sending 19 positions of interest
    int crap = 19;
    output[0] = crap;
    // Sending Map data command
    output[1] = 'M';
    // Sending column number
    output[2] = col;

	for (int it = 0; it < 17; it++)
	{
        // Type of the block we are looking at
        output[it+3] = this->getPos(col,it)->getType();
	}
    
    return output;
}

void Map::fillClosedArea()
{
	int countc = 0;
	// fill map with closed area from left
	 for (int y = 0; y < 17; y++) {
		for (int x = 0; x < 32; x++) {
			if((this->getPos(x,y)->getType() != 'c') && (countc==0)){
				 convertSection(x,y,'c');
			}
			else{
				countc++;
			}
			
			if(this->getPos(x,y)->getType() == 'e'){
				countc = 0;
			}
			if((countc >1) && (this->getPos(x,y)->getType() == 'u'))
				convertSection(x,y,'c');
				countc = 0;
        }
    }
}

// -------------------- DEBUG --------------------

#if DEBUG == 1
void Map::printMap(){
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            cout << this->getPos(i,it)->getType() << " ";
        }
        cout << endl;
    }
}

void Map::initMap(){
    char tempMap[32][17]={
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','c','c','c','c','c','c','c','c','c','c','c','c','c','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','u','u','u','u','u','u','u','u','u','u','u','u','c','u','u'},
        {'u','c','c','c','c','c','c','c','c','c','c','c','c','c','c','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'}
    };
    
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            mapArea[i][it] = new MapSection(i,it,this); // it, i to i, it
            
            if (tempMap[i][it] == 'c') {
                this->getPos(i,it)->setType('c');
            }
        }
    }
}

#endif