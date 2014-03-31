#include "MapSection.h"
using namespace std;

void MapSection::setX(int coord){
	xCoord=coord;
}

void MapSection::setY(int coord){
	yCoord=coord;
}

int MapSection::getX(){
	return xCoord;
}

int MapSection::getY(){
	return yCoord;
}