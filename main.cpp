/* Worldgen
Programmed by Evan Landau in the year 2020
Based on the text World Generation: Generic System & Planet Building Resources, by Tyge Sjostrand, published in 2000
Available from http://sol.trisen.com/downloads/wg.pdf
The original document was for non-commercial use only, as is this program.
Any non-commercial use, modification, or sharing of this software is permitted.*/

#include <iostream>
#include "Star.h"
#include <time.h>

int main() 
{
	std::srand((unsigned)time((NULL)));
	std::cout << Star::Star().describe_star(); //Generate a new star, and output it
	return 0;
}