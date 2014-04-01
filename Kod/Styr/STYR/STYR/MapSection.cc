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