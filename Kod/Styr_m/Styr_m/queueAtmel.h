//
//  node.h
//
//
//  Created by Niklas Ericson on 2014-05-16.
//
//

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
	node* prioArray[10000];
};



#endif
