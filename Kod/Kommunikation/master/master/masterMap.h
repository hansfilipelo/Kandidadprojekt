#ifndef _Map_h
#define _Map_h

#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG == 1
#include <iostream>
#endif

#include "../../../Styr_m/Styr_m/Abstraction.h"
#include <string.h>
#include <stdio.h>

class Map
{
public:
	char mapArea[32][27];
};

#endif
