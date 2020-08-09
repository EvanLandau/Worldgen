#pragma once
#include <string>
class Gas {
public:
    enum class gases { n2, co2, ch4, h2, he, no2, so2, co, nh3, ne } gas; //Different gases to add
    
    int mol_weights[10] = { 28, 44, 16, 2, 4, 46, 64, 28, 17, 20 }; //Molecular weights of different gases (corresponds to above)- from chart 4.3.3
    int molecular_weight;
    Gas(enum class gases type);
    double partial_pressure = 0;
};