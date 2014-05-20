//
//  node.cpp
//  
//
//  Created by Niklas Ericson on 2014-05-16.
//
//

#include "queueAtmel.h"

using namespace std;

//queueAtmel::queueAtmel()
//{
//	for(unsigned int i=0; i<10000; i++){
//		prioArray[i] = NULL;
//	}
//}

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
        while( (pos < size) && (prioArray[pos]->getPriority() > nod.getPriority()) ){
//            cout << "Pos: " << pos << endl;
//            cout << "Size: " << size << endl;
            pos++;
        }
        
        // Highest prio - put last
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
	return *prioArray[size - 1];
}

// ----------------------

void queueAtmel::Apop()
{
    delete prioArray[size - 1];
    prioArray[size - 1] = NULL;
	size--;
}

// ----------------------

int queueAtmel::Asize()
{
	return size;
}

