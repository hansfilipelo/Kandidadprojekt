#ifndef _Abstraction_h
#define _Abstraction_h

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifndef __AVR_ATmega1284P__
#define TESTING 1
#else
#define TESTING 0
#endif

#if TESTING == 1

#include <iostream>

#endif

#if TESTING == 0

// Operators new and delete does not exist in AVR version of C++
void* operator new(size_t objsize);
void operator delete(void* obj);
void* operator new[](unsigned int x);
void operator delete[](void *);

#endif

void pushBackChar(char* inArray, char inChar[3]);
char* intToChar(int inInt);
int charToInt(char* inArray);
int getValueFromArray(char* inArray, int valueNr);

#endif