#include "MapSection.h"
#include "Map.h"

using namespace std;

// Construct -------------------

MapSection::MapSection(int x,int y, Map* inMom){
    xCoord=x;
    yCoord=y;
    mom = inMom;
    type='u';
}

MapSection::MapSection(int x,int y, Map* inMom, char createType){
    xCoord=x;
    yCoord=y;
    mom = inMom;
    type=createType;
}

// Destruct

MapSection::~MapSection(){};

// ----------------------------
// Set stuff

void MapSection::setX(int coord){
	xCoord=coord;
}

void MapSection::setY(int coord){
	yCoord=coord;
}

void MapSection::setType(char newType){
	type=newType;
}


//---------------------------------------------------------
int MapSection::getX(){
	return xCoord;
}

int MapSection::getY(){
	return yCoord;
}

char MapSection::getType(){
	return type;
}

//---------------------------------------------------------

MapSection* MapSection::getTop(){
	return mom->getPos(xCoord, yCoord + 1);
}

MapSection* MapSection::getBottom(){
	return mom->getPos(xCoord, yCoord - 1);
}

MapSection* MapSection::getLeft(){
	return mom->getPos(xCoord - 1, yCoord);
}

MapSection* MapSection::getRight(){
	return mom->getPos(xCoord + 1, yCoord);
}

//-----------------------------------------------------------



//----------------------------------------------------------

bool MapSection::isUnexplored(){
	if (type == 'u') {
		return true;
	}
	else{
		return false;
	}
}

// SLAM algorithms -------------------------------------

int MapSection::findUnexplored(){
	int topCounter;
	int botCounter;
	int leftCounter;
	int rightCounter;
	
	if (getTop()->isUnexplored()){
		topCounter=1;
	}
	else{
		topCounter= getTop()->findUnexplored()+1;
	}
	
	if (getBottom()->isUnexplored()){
		botCounter=1;
	}
	else{
		botCounter= getBottom()->findUnexplored()+1;
	}
	
	if (getLeft()->isUnexplored()){
		leftCounter=1;
	}
	else{
		leftCounter= getLeft()->findUnexplored()+1;
	}
	
	if (getRight()->isUnexplored()){
		rightCounter=1;
	}
	else{
		rightCounter= getRight()->findUnexplored()+1;
	}
	
	if(topCounter < botCounter || topCounter < leftCounter || topCounter < rightCounter){
		next = 't';
		return topCounter;
	}
	else if (botCounter < leftCounter || botCounter < rightCounter){
		next = 'b';
		return botCounter;
	}
	else if (leftCounter < rightCounter){
		next = 'l';
		return leftCounter;
	}
	else {
		next = 'r';
		return rightCounter;
	}
}