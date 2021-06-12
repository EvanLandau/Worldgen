#pragma once
#include <functional>
#include <vector>
#include <string>
#include "Gas.h"
#include "RNG.h"

class Solid
{
public:
    //Composition (pg. 20)
    enum class composition { silicate_metal_core, iron_nickel_medium_core, iron_nickel_large_core, ice, ice_silicate_core } body_composition = composition::ice;
    void calculate_composition(double density, bool inner_zone);
    //Tectonics (pg. 21)
    enum class tectonics { dead, hot_spot, plastic, plate_tectonic, platelet_tectonic, extreme } body_tectonics = tectonics::dead;
    void calculate_tectonics(double mass, double density, double age, bool has_major_moons, enum composition comp, double rotational_period);
    //TODO?: Add extra geophysical data from page 22
    //Magnetic Field (pg. 23)
    double magnetic_field = 0.0; //In Earth Magnetic Fields
    void calculate_magnetic_field(composition body_composition, double mass, double density, double rotational_period, double age);
    //Surface Temp (pg. 27)
    int temp = 0; //In Kelvin
    void calculate_surface_temp(double orbital_distance, double luminosity);
    //Hydrosphere (pg. 28)
    enum class hydrosphere { none, vapor, liquid, ice_sheet, crustal } hydrosphere_type = hydrosphere::none;
    int hydrosphere_extensiveness = 0;
    double vapor_factor = 0.0;
    void calculate_hydrosphere(bool inner_zone, int surface_temp, int radius);
    //Atmosphere (pg. 29)
    double total_pressure = 0;
    std::string gas_names[10] = { "N2", "CO2", "CH4", "H2", "He", "NO2", "SO2", "CO", "NH3", "Ne" };
    std::vector <Gas> gases;
    void calculate_atmosphere(int surface_temp, double mass, double escape_velocity, Solid::tectonics tectonics);
    //Calculate Albedo (pg. 32)
    double albedo = 1; //Notably, this is actually albedo FACTOR, which is the opposite of albedo- higher albedo factor represents lower proper albedo
    void calculate_albedo(bool inner_zone, double total_pressure, hydrosphere hydrosphere_type, int hydrosphere_extensiveness);
    //Recalc Surface Temp (pg. 33)
    void recalc_surface_temp(double total_pressure, std::vector <Gas> gases, double vapor_factor, double albedo, int surface_temp);
    //TODO: Implement life?
    //Return values
    std::string describe_solid();
    std::string describe_non_solid();
};