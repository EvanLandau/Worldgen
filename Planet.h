#pragma once
#include <vector>
#include <random>
#include <functional>
#include "Moon.h"
#include "Solid.h"

class Planet : public Solid //TODO: Use ENUMS
{
public:
    //Star/Orbit Values
    double parent_mass; //compared to Sol
    double parent_luminosity; //compared to Sol
    int abundance; //compared to Sol
    double orbit_radius; //in AU
    double orbital_period; //in Earth years
    bool inner_zone;
    bool life_zone = false;
    double life_zone_optimum_distance; //How far is it from the optimum life zone distance (negative means it is closer than the optimum, larger means farther)
    double tidal_force;
    double age; //In GY, age is same as star
    double eccentricity;
    double aphelion;
    double perihelion;
    //Planet Values
    enum class planet_type { asteroid_belt, terrestrial, chunk, gas_giant, superjovian } planet_class; //0 - Asteroid Belt, 1 - Terrestrial, 2 - Chunk, 3 - Gas Giant, 4 - Superjovian
    long double radius; //in km
    double density; //in earth densities
    double mass; //in earth masses
    double gravity; //in earth gravities
    double escape_velocity; //in earth escape velocities
    bool tidal_locked = false;
    double rotational_period; //in days
    double axial_tilt; //in degrees
    std::vector<Moon> moons; //List of moons
    //Methods
    planet_type calc_planet_type(bool zone, int special_roll);
    Planet(double parent_mass, double parent_luminosity, double parent_age, int abundance_mod, double orbital_radius, bool inner_zone);
    std::string describe_planet();
};