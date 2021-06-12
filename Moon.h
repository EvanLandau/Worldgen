#pragma once
#include <random>
#include <functional>
#include "Solid.h"

class Moon : public Solid
{
public:
    //Define enum
    enum class body_type { tiny_chunk, chunk, large_chunk, small_terrestrial, terrestrial, ring };
    //Moon Values
    int radius; //in km
    double density; //in earth densities
    long double mass; //in earth masses
    double gravity; //in g
    double solar_radius; //In relation to the sun, in AU
    double orbital_radius; //(in relation to parent) in km
    double orbital_period; //in days
    enum body_type moon_type;
    double escape_velocity;
    //Parent Values
    double parent_mass; //in earth masses
    //Methods
    Moon(double parent_mass, double parent_density, double orbital_radius, double solar_orbital_radius, int abundance, double star_luminosity, double age, bool inner_zone);
    std::string describe_moon();
};