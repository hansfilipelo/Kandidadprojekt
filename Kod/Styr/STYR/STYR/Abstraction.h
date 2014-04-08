#ifndef _Abstraction_h
#define _Abstraction_h

#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG == 1

#include <iostream>

#endif

void pushBackChar(char* inArray, char inChar);
char* intToChar(int inInt);
int charToInt(char* inArray);
int getValueFromArray(char* inArray, int valueNr);

#endif