#pragma once

#include <random>
#include <functional>
#include <cmath>
#include <vector>
#include "Planet.h"

class Star { //Class for a solar system //TODO: Use ENUMS
public:
    //Basic astronomical classification
    enum class star_class { A, F, G, K, white_dwarf, M, brown_dwarf } spectral_class;  //0 - A, 1 - F, 2 - G, 3 - K, 4 - White Dwarf, 5 - M, 6- Brown Dwarf
    int subclass; //Same as numbers used in actual notation. Should be from 0 to 9.
    int size_code; //Usually denoted with roman numerals. Same as number used in actual notation, except that -1 is used for non-standard stars
    //Star values in relation to Sol
    long double luminosity;
    double mass;
    int temp; //In Kelvin
    double radius;
    //Other values
    float flare_ratio = 1;
    float age; //In GY
    int abundance; //Abundance modifier (pg 8)
    //Planet Relevant Values
    std::vector <Planet> planets;
    //Methods
    Star();
    std::string describe_star();
};