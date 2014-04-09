#ifndef _Abstraction_h
#define _Abstraction_h

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG == 1

#include <iostream>

void* operator new(size_t objsize);
void operator delete(void* obj);

#endif

void pushBackChar(char* inArray, char inChar);
char* intToChar(int inInt);
int charToInt(char* inArray);
int getValueFromArray(char* inArray, int valueNr);

#endif