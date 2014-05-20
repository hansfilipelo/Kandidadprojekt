//
//  node.cpp
//  
//
//  Created by Niklas Ericson on 2014-05-16.
//
//

#include "queueAtmel.h"


queueAtmel::queueAtmel()
{
	for(unsigned int i=0; i<100; i++){
		prioArray[i] = NULL;
	}
}

void queueAtmel::Apush(node& nod)
{
	int pos = 0;
	while(prioArray[pos]->getPriority() < nod.getPriority()){
		pos++;
	}
	for (int i = size; i > pos; i--)
	{
		prioArray[i] = prioArray[i-1];
	}
	size++;
	prioArray[pos] = &nod;
}

bool queueAtmel::Aempty()
{
	if(size==0){
		return true;
	}
	return false;
}

node& queueAtmel::Atop()
{
	return *prioArray[size];
}

void queueAtmel::Apop()
{
	prioArray[size] = NULL;
	size--;
}

int queueAtmel::Asize()
{
	return size;
}
