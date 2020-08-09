#include "RNG.h"

int RNG::d100() 
{
	return rand() % 100 + 1;
}

int RNG::d10()
{
	return rand() % 10 + 1;
}