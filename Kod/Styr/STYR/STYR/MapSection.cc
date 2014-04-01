#include "MapSection.h"
using namespace std;

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

int MapSection::getSize(){
	return size;
}

bool MapSection::isRobot(){
	return containsRobot;
}

//---------------------------------------------------------

MapSection* MapSection::getTop(){
	return top;
}

MapSection* MapSection::getBottom(){
	return bottom;
}

MapSection* MapSection::getLeft(){
	return left;
}

MapSection* MapSection::getRight(){
	return right;
}

//-----------------------------------------------------------

void setTop(MapSection* ptr){
	top = ptr;
}

void setBottom(MapSection* ptr){
	bottom = ptr;
}

void setLeft(MapSection* ptr){
	left = ptr;
}

void setRight(MapSection* ptr){
	right = ptr;
}

//----------------------------------------------------------

bool isUnexplored(){
	if (type == "unexplored") {
		return true;
	}
	else{
		return false
	}
}

int findUnexplored(){
	int topCounter;
	int botCounter;
	int leftCounter;
	int rightCounter;
	
	if (top->isUnexplored()){
		topCounter=1;
	}
	else{
		topCounter= top->findUnexplored()+1;
	}
	
	if (bottom->isUnexplored()){
		botCounter=1;
	}
	else{
		botCounter= bottom->findUnexplored()+1;
	}
	
	if (left->isUnexplored()){
		leftCounter=1;
	}
	else{
		leftCounter= left->findUnexplored()+1;
	}
	
	if (right->isUnexplored()){
		rightCounter=1;
	}
	else{
		rightCounter= right->findUnexplored()+1;
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