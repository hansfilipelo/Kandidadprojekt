#define DEBUG 1

#include <stdio.h>
#include "MapSection.h"
#include "Map.h"
#include "Abstraction.h"

int main(){
    
    //Initiate
    Map* mom = new Map();
    
    Robot* testis = new Robot(10,10,mom);
    
    
    delete testis;
    delete mom;
    
	return 0;
}
