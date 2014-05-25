/*
 File: queueAtmel.h
 Purpose: Class helping A-star algorithm. Not implemented on robot. 
    Re-implementation of C++ class queue.
 */


#ifndef queueATMEL_H
#define queueATMEL_H

#include "Abstraction.h"
#include "node.h"

class queueAtmel
{     
public:
	//queueAtmel();
	
	void Apush(node&);
	void Apop();
	bool Aempty();
	node& Atop();
	int Asize();
	
private:
	int size = 0;
	node* prioArray[1000];
};



#endif
