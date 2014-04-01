#include "MapSection.h"
#include "Map.h"

using namespace std;

MapSection::MapSection(int x,int y){
    xCoord=x;
    yCoord=y;
    type="unexplored";
}

MapSection::MapSection(int x,int y,string createType){
    xCoord=x;
    yCoord=y;
    type=createType;
}

void MapSection::setX(int coord){
	xCoord=coord;
}

void MapSection::setY(int coord){
	yCoord=coord;
}

void MapSection::setType(string newType){
	type=newType;
}

void MapSection::setRobot(bool status){
	containsRobot = status;
}

//---------------------------------------------------------
int MapSection::getX(){
	return xCoord;
}

int MapSection::getY(){
	return yCoord;
}

string MapSection::getType(){
	return type;
}

bool MapSection::isRobot(){
	return containsRobot;
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
	if (type == "unexplored") {
		return true;
	}
	else{
		return false;
	}
}

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
		next = "top";
		return topCounter;
	}
	else if (botCounter < leftCounter || botCounter < rightCounter){
		next = "bottom";
		return botCounter;
	}
	else if (leftCounter < rightCounter){
		next = "left";
		return leftCounter;
	}
	else {
		next = "right";
		return rightCounter;
	}
}