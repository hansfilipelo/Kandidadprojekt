//
//  node.cpp
//  
//
//  Created by Niklas Ericson on 2014-05-16.
//
//

#include "queueAtmel.h"

using namespace std;

queueAtmel::queueAtmel()
{
	for(unsigned int i=0; i<100; i++){
		prioArray[i] = NULL;
	}
}

// ----------------------

void queueAtmel::Apush(node& nod)
{
    // If empty, add first
    if (size == 0) {
        prioArray[0] = &nod;
    }
    else {
        int pos = 0;
        // Lowest "priority" == highest priority and put last
        while( (prioArray[pos]->getPriority() > nod.getPriority()) && pos < size ){
            pos++;
        }
        
        // If highest prio, put last
        if (pos == size) {
            prioArray[size] = &nod;
        }
        else {
            // Else push_back to right place
            for (int i = size; i > pos; i--)
            {
                prioArray[i] = prioArray[i-1];
            }
            prioArray[pos] = &nod;
        }
    }
    
    size++;
}

// ----------------------

bool queueAtmel::Aempty()
{
	if(size==0){
		return true;
	}
	return false;
}

node& queueAtmel::Atop()
{
	return *prioArray[0];
}

// ----------------------

void queueAtmel::Apop()
{
    delete prioArray[0];
	for (int i = 0; i < size - 1; i++) {
        prioArray[i] = prioArray[i+1];
    }
	size--;
}

// ----------------------

int queueAtmel::Asize()
{
	return size;
}

