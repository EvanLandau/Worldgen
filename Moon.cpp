#include "Moon.h"

Moon::Moon(double parent_mass, double parent_density, double orbital_radius, int abundance, double star_luminosity, double age, bool inner_zone) //TODO: Add more params
{

    //Set values passed down from parent
    Moon::orbital_radius = orbital_radius;
    Moon::parent_mass = parent_mass;

    //Roll type, radius (2.3.3, pg. 18)
    int selection_value = RNG::d100();
    if (abundance < 0) {selection_value += abundance * 2;} //Apply the system abundance modifier- double it if negative
    else {selection_value += abundance;}
    if (selection_value < 65) 
    {
        Moon::moon_type = body_type::tiny_chunk;
        Moon::radius = RNG::d10() * 10;
    }
    else if (selection_value < 85) 
    {
        Moon::moon_type = body_type::chunk;
        Moon::radius = RNG::d10() * 100;
    }
    else if (selection_value < 94) 
    {
        Moon::moon_type = body_type::large_chunk;
        Moon::radius = RNG::d10() * 100 + 1000;
    }
    else if (selection_value < 100) 
    {
        Moon::moon_type = body_type::small_terrestrial;
        Moon::radius = RNG::d10() * 200 + 2000;
    }
    else 
    {
        Moon::moon_type = body_type::terrestrial;
        Moon::radius = RNG::d10() * 400 + 4000;
    }

    //Roll density (2.3.3, pg 18)
    if (inner_zone)
    {
        Moon::density = 0.3 + RNG::d10() * 0.1;
    }
    else
    {
        Moon::density = 0.1 + RNG::d10() * 0.05;
    }

    //Calculate Mass, Gravity, and Year (2.3.4, pg 18)
    Moon::mass = pow(Moon::radius/6380, 3) * Moon::density;
    Moon::gravity = Moon::mass / pow(Moon::radius/6380, 2);
    Moon::orbital_period = sqrt(pow(Moon::orbital_radius/400000, 3) * 793.64/ Moon::parent_mass);
    Moon::escape_velocity = sqrt(19600 * Moon::gravity * Moon::radius) / (11200);

    if (isnan(Moon::gravity)) { Moon::gravity = 0; }
    if (isnan(Moon::escape_velocity)) { Moon::escape_velocity = 0; }

    //Roche Limit & Ring Formation (2.3.5, pg. 18)
    if (orbital_radius < 2.456 * pow(parent_density / density, 0.333)) 
    {
        Moon::moon_type = Moon::body_type::ring;
        Moon::gravity = 0;
        Moon::mass -= Moon::mass * age * 0.01; //Decrease mass to simulate material falling into the planet
    }

    //Composition
    calculate_composition(density, inner_zone);
    //Surface Temp
    calculate_surface_temp(orbital_radius, star_luminosity);

    if (moon_type != Moon::body_type::ring) 
    {
        //Tectonics
        calculate_tectonics(mass, density, age, false, Solid::body_composition, orbital_period); //Using orbital period because a large number of moons are tidal locked (and the rule book doesn't have rules for rotational period)
        //Magnetic Field
        calculate_magnetic_field(body_composition, mass, density, orbital_period, age);
        //Hydrosphere
        calculate_hydrosphere(inner_zone, temp, radius);
        //Atmosphere
        calculate_atmosphere(temp, mass, escape_velocity, body_tectonics);
        //Albedo
        calculate_albedo(inner_zone, total_pressure, hydrosphere_type, hydrosphere_extensiveness);
    }
    else
    {
        //Hydrosphere
        if (Solid::body_composition == Solid::composition::ice)
        {
            Solid::hydrosphere_type = Solid::hydrosphere::crustal;
            Solid::hydrosphere_extensiveness = 100;
        }
        else
        {
            Solid::hydrosphere_type = Solid::hydrosphere::none;
            Solid::hydrosphere_extensiveness = 0;
        }
        //Albedo
        calculate_albedo(inner_zone, 0.0, Solid::hydrosphere_type, Solid::hydrosphere_extensiveness);
    }
    //Recalc Temp
    recalc_surface_temp(total_pressure, gases, vapor_factor, albedo, temp);
}

std::string Moon::describe_moon() 
{
    //Stat block
    std::string return_string = "- MOON -\n";
    return_string += "Radius (km): " + std::to_string(Moon::radius) + "\n";
    return_string += "Mass (Earth Masses): " + std::to_string(Moon::mass) + "\n";
    return_string += "Density (Earth Relative): " + std::to_string(Moon::density) + "\n";
    return_string += "Surface Gravity (g): " + std::to_string(Moon::gravity) + "\n";
    return_string += "Escape Velocity (Earth Relative): " + std::to_string(Moon::escape_velocity) + "\n";
    return_string += "Orbital Radius (km): " + std::to_string(Moon::orbital_radius) + "\n";
    return_string += "Orbital Period (days): " + std::to_string(Moon::orbital_period) + "\n";
    return_string += Solid::describe_solid();
    //Textual Description
    switch (moon_type)
    {
    case body_type::tiny_chunk:
        return_string += "This moon is about the size of a small asteroid.\n "; break;
    case body_type::chunk:
        return_string += "This moon is the size of a large asteroid.\n "; break;
    case body_type::large_chunk:
        return_string += "This moon is the size of a very large asteroid.\n "; break;
    case body_type::small_terrestrial:
        return_string += "This moon is a small planetoid, like Earth's moon.\n "; break;
    case body_type::terrestrial:
        return_string += "This moon is the size of a normal rocky planet.\n "; break;
    case body_type::ring:
        return_string += "This moon failed to form, and is now just a ring made of many small bodies.\n "; break;
    }
    return_string += "\n";
    return return_string;
}