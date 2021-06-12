#include "Planet.h"

Planet::Planet(double parent_mass, double parent_luminosity, double parent_age, int abundance_mod, double orbital_radius, bool inner_zone)
{
    Planet::orbit_radius = orbital_radius;
    Planet::inner_zone = inner_zone;
    Planet::parent_luminosity = parent_luminosity;
    Planet::parent_mass = parent_mass;
    Planet::age = parent_age;
    Planet::abundance = abundance_mod;

    //Roll for planet type
    Planet::planet_class = calc_planet_type(inner_zone, 0);
    //Get radius - tables 1.5.2 and 1.5.5, pg. 12
    int selection_roll = RNG::d10();
    switch (Planet::planet_class) 
    {
    case planet_type::asteroid_belt: //Asteroids
            Planet::radius = 0;
            Planet::density = 0;
            break;
    case planet_type::terrestrial: //Terrestrial
            Planet::radius = RNG::d10() * 1000.0 + RNG::d10() * 500.0;
            break;
    case planet_type::chunk: //Chunk
            Planet::radius = RNG::d10() * 200.0;
            break;
    case planet_type::gas_giant: //Gas giants
            if (selection_roll < 6) 
            {
                Planet::radius = 12000 + 3000.0 * selection_roll + 300.0 * RNG::d10();
            }
            else 
            {
                Planet::radius = 100000.0 * (selection_roll - 3) + RNG::d10() * 1000.0;
            }
            break;
    case planet_type::superjovian: //Superjovians
            Planet::radius = (60000 + (RNG::d10() - parent_age/2) * 2000);
            break;
    }
    //Calculate density (table 1.5.3 & 1.5.6, pg. 12)
    if (inner_zone) {
        switch(planet_class) 
        {
        case planet_type::asteroid_belt:
            Planet::density = 0.3 + abs(RNG::d10() + abundance) * 0.1;
            break;
        case planet_type::chunk:
        case planet_type::terrestrial:
            Planet::density = 0.3 + (float) abs(RNG::d10() + abundance) * 0.127 / pow((0.4 + (orbital_radius / sqrt(parent_luminosity))), 0.67);
            break;
        case planet_type::gas_giant:
            Planet::density = 0.1 + abs(RNG::d10() + abundance) * 0.025;
            break;
        }
    }
    else 
    {
        switch(planet_class) 
        {
        case planet_type::asteroid_belt:
        case planet_type::chunk:
        case planet_type::terrestrial:
            Planet::density = 0.1 + abs(RNG::d10() + abundance) * 0.05;
            break;
        case planet_type::gas_giant:
            Planet::density = 0.08 + abs(RNG::d10() + abundance) * 0.025;
            break;
        }
    }
    if (Planet::density > 1.5) { density = 1.5; } //Per 1.5.6 on page 12, "densities cannot be higher than 1.5"
    //Calculate mass (1.5.4, pg. 12)
    if (planet_class == planet_type::superjovian) //Superjovian generation (1.5.5)
    {
        int selection_value = RNG::d10();
        if (selection_value < 5) {mass = 500.0 + RNG::d10() * 50.0;}
        else if (selection_value < 8) {mass = 1000.0 + RNG::d10() * 100.0;}
        else if (selection_value < 10) {mass = 2000.0 + RNG::d10() * 100.0;}
        else {mass = 3000.0 + RNG::d10() * 100.0;}
        density = mass * pow(6380/radius, 3);
    }
    else if (planet_class == planet_type::asteroid_belt) //Asteroid stuff (TODO)
    {
        Planet::mass = 0.000492 * Planet::density; //Naive calculation based on the assumption that our solar system's asteroid belt has a similar density to Earth and that the mass doesn't depend on distance from the star
    }
    else 
    {
        Planet::mass = density * pow(radius/6380, 3);
    }
    //Calculate gravity and escape velocity (1.5.4)
    if (planet_class == planet_type::asteroid_belt) 
    {
        gravity = 0;
        escape_velocity = 0;
    }
    else
    {
        gravity = mass / pow(radius / 6380, 2);
        escape_velocity = sqrt(19600 * gravity * radius) / 11200;
    }
    //Life Zone calculation (pg. 14)
    if (orbital_radius >= 0.75 * sqrt(parent_luminosity) && orbital_radius <= 1.4 * sqrt(parent_luminosity)) {life_zone = true;}
    life_zone_optimum_distance = orbital_radius - sqrt(parent_luminosity);
    // Year (2.1.1, pg. 15)
    orbital_period = sqrt(pow(orbital_radius, 3) / parent_mass);
    // Tidal Forces & Locking (2.1.2, pg. 15)
    tidal_force = (parent_mass * 26640000)/pow(orbit_radius * 400, 3);
    if ((0.83 + RNG::d10() * 0.03) * tidal_force * parent_age / 6.6 > 1) {Planet::tidal_locked = true;}

    //Orbital Eccentricity (2.2.1, pg 16)
    switch (RNG::d10()) 
    {
        case 1: case 2: case 3: case 4: case 5:
        eccentricity = 0.005 * RNG::d10(); break;
        case 6: case 7:
        eccentricity = 0.05 + 0.01 * RNG::d10(); break;
        case 8: case 9:
        eccentricity = 0.15 + 0.01 * RNG::d10(); break;
        case 10:
        eccentricity = 0.25 + 0.04 * RNG::d10(); break;
    }
    //Aphelion & Perihelion
    aphelion = orbital_radius * (1 + eccentricity); perihelion = orbital_radius * (1 - eccentricity);

    //Rotational Period (2.2.2, pg 16)
    if (tidal_locked) {rotational_period = orbital_period * 365;}
    else 
    {
        int selection_roll = RNG::d10() + (int)(tidal_force * age);
        switch (planet_class) 
        {
        case planet_type::asteroid_belt:
                rotational_period = 0;
                break;
        case planet_type::terrestrial:
                rotational_period = pow(RNG::d10(), abs(RNG::d10()/3 - 1)) * 2;
                break;
        case planet_type::chunk:
                rotational_period = pow((int) RNG::d10()/2, abs((double)RNG::d10())/3.0 - 1.0);
                break;
        case planet_type::gas_giant:
        case planet_type::superjovian:
                rotational_period = 2.0 * RNG::d10() + RNG::d10();
                break;
        }
        int modifier = (int)(tidal_force * age);
        if (planet_class == planet_type::terrestrial && mass >= 4) {modifier -= 2;}
        if (planet_class == planet_type::gas_giant && mass < 50) {modifier += 2;}
        rotational_period *= 1 + modifier * 0.1;
    }
    
    //Axial Tilt (2.2.3, pg 16)
    if (tidal_locked) {axial_tilt = 0;}
    switch ((RNG::d10() + 1)/2) 
    {
        case 1:
            axial_tilt = RNG::d10(); break;
        case 2:
            axial_tilt = 10.0 + RNG::d10(); break;
        case 3:
            axial_tilt = 20.0 + RNG::d10(); break;
        case 4:
            axial_tilt = 30.0 + RNG::d10(); break;
        case 5:
            axial_tilt = 40 + 1.4 * RNG::d100(); break;
    }

    //Moon generation (pg. 18)
    int lunar_orbits = 0; //Generate number of orbits - 2.3.1
    if (!tidal_locked && planet_class != planet_type::asteroid_belt) 
    {
        int selection_roll = RNG::d10();
        if (!inner_zone) { selection_roll += 5; } //Outer zone planets have no moons
        switch (planet_class) 
        {
        case planet_type::chunk:
            if (selection_roll > 9) { lunar_orbits = 1; }
            break;
        case planet_type::terrestrial:
            if (selection_roll < 6) { lunar_orbits = 0; }
            else if (selection_roll < 8) { lunar_orbits = 1; }
            else if (selection_roll < 10) { lunar_orbits = RNG::d10() / 5; }
            else if (selection_roll < 14) { lunar_orbits = RNG::d10() / 2; }
            else { lunar_orbits = RNG::d10(); }
            break;
        case planet_type::gas_giant:
        case planet_type::superjovian:
            if (selection_roll < 6) { lunar_orbits = RNG::d10() / 2; }
            else if (selection_roll < 8) { lunar_orbits = RNG::d10(); }
            else if (selection_roll < 10) { lunar_orbits = RNG::d10() + 5; }
            else if (selection_roll < 14) { lunar_orbits = RNG::d10() + 10; }
            else { lunar_orbits = RNG::d10() + 20; }
            break;
        }
    }
    //Determine orbit distances - 2.3.2
    std::vector<double> lunar_orbit_list;
    for (int i = 0; i < lunar_orbits; i++) 
    {
        selection_roll = RNG::d10();
        if (selection_roll < 5) { lunar_orbit_list.push_back(radius * 1.0 + RNG::d10() * 0.5); } //Close
        else if (selection_roll < 7) { lunar_orbit_list.push_back(radius * 6.0 + RNG::d10()); } //Average
        else if (selection_roll < 9) { lunar_orbit_list.push_back(radius * 16.0 + RNG::d10()*3.0); } //Distant
        else { lunar_orbit_list.push_back(radius * 45.0 + RNG::d100() * 3.0); } //Very Distant
    }

    //Generate moons from orbits
    for (unsigned i = 0; i < lunar_orbit_list.size(); i++) 
    {
        moons.push_back(Moon::Moon(mass, density, lunar_orbit_list[i], orbital_radius, abundance, parent_luminosity, age, inner_zone));
    }

    //Composition
    calculate_composition(density, inner_zone);
    //Tectonics
    calculate_tectonics(mass, density, age, lunar_orbits > 0, body_composition, rotational_period);
    //Magnetic Field
    calculate_magnetic_field(body_composition, mass, density, rotational_period, age);
    //Surface Temp
    calculate_surface_temp(orbital_radius, parent_luminosity);
    //Hydrosphere
    calculate_hydrosphere(inner_zone, temp, (int) radius);
    //Atmosphere
    calculate_atmosphere(temp, mass, escape_velocity, body_tectonics);
    //Albedo
    calculate_albedo(inner_zone, total_pressure, hydrosphere_type, hydrosphere_extensiveness);
    //Recalc Temp
    recalc_surface_temp(total_pressure, gases, vapor_factor, albedo, temp);
}

Planet::planet_type Planet::calc_planet_type(bool zone, int special_roll) //Based on table 1.5.1 (pg. 12) Special value is for reroll stuff- zero means it is a normal roll
{
    int selection_roll = RNG::d100();
    if (inner_zone) 
    {
        if (selection_roll < 19 && special_roll == 0) //Asteroid Belt - type 0
        {
            return planet_type::asteroid_belt;
        }
        else if (selection_roll < 63) //Terrestrial Planet - type 1
        {
            return planet_type::terrestrial;
        }
        else if (selection_roll < 72) //Chunk - type 2
        {
            return planet_type::chunk;
        }
        else if (selection_roll < 83) //Gas Giant - type 3
        {
            return planet_type::gas_giant;
        }
        else if (selection_roll < 87 && special_roll != 1) //Superjovian - type 4
        {
            return planet_type::superjovian;
        }
        else if (selection_roll < 97 && special_roll == 0) //Empty Orbit - reroll
        {
            return calc_planet_type(zone, special_roll);
        }
        else if (selection_roll == 97 && special_roll == 0) //Interloper -  special value of 1
        {
            return calc_planet_type(!zone, 1);
        }
        else if (selection_roll == 98 && special_roll == 0) //Trojan - reroll
        {
            return calc_planet_type(zone, 1);
        }
        else if (selection_roll = 99 && special_roll == 0) //Double Planet - reroll
        {
            return calc_planet_type(zone, special_roll);

        }
        else if (selection_roll == 100 && special_roll == 0)  //Captured Body - special value of 2
        {
            return calc_planet_type(zone, 2);
        }
        else //Catch case - returns chunk
        {
            return planet_type::chunk;
        }
    }
    else 
    {
        if (selection_roll < 16 && special_roll == 0) //Asteroid Belt - type 0
        {
            return planet_type::asteroid_belt;
        }
        else if (selection_roll < 24) //Terrestrial Planet - type 1
        {
            return planet_type::terrestrial;
        }
        else if (selection_roll < 36) //Chunk - type 2
        {
            return planet_type::chunk;
        }
        else if (selection_roll < 75) //Gas Giant - type 3
        {
            return planet_type::gas_giant;
        }
        else if (selection_roll < 85 && special_roll != 1) //Superjovian - type 4
        {
            return planet_type::superjovian;
        }
        else if (selection_roll < 95 && special_roll == 0) //Empty Orbit - reroll
        {
            return calc_planet_type(zone, special_roll);
        }
        else if (selection_roll == 95 && special_roll == 0) //Interloper -  special value of 1
        {
            return calc_planet_type(!zone, 1);
        }
        else if (selection_roll < 98 && special_roll == 0) //Trojan - reroll
        {
            return calc_planet_type(!zone, 1);
        }
        else if (selection_roll < 99 && special_roll == 0) //Double Planet - reroll
        {
            return calc_planet_type(zone, special_roll);

        }
        else if (selection_roll == 100 && special_roll == 0)  //Captured Body - special value of 2
        {
            return calc_planet_type(zone, 2);
        }
        else //Catch case - returns gas giant
        {
            return planet_type::gas_giant;
        }
    }
}

std::string Planet::describe_planet() 
{
    //Get planet stat block
    std::string return_string = "#-- PLANET --#\n";
    return_string += "Radius (km): " + std::to_string(radius) + "\n";
    return_string += "Mass (Earth Masses): " + std::to_string(mass) + "\n";
    return_string += "Density (Earth Relative): " + std::to_string(density) + "\n";
    return_string += "Surface Gravity (g): " + std::to_string(gravity) + "\n";
    return_string += "Escape Velocity (Earth Relative): " + std::to_string(escape_velocity) + "\n";
    return_string += "Orbital Radius (AU): " + std::to_string(orbit_radius) + "\n";
    return_string += "Orbital Period (years): " + std::to_string(orbital_period) + "\n";
    return_string += "Orbital Eccentricity: " + std::to_string(eccentricity) + "\n";
    return_string += "Perihelion (AU): " + std::to_string(perihelion) + "\n";
    return_string += "Aphelion (AU): " + std::to_string(aphelion) + "\n";
    return_string += "Inner Zone: ";
    if (Planet::inner_zone) { return_string += "True\n"; }
    else { return_string += "False\n"; }
    return_string += "Tidally Locked: ";
    if (Planet::tidal_locked) { return_string += "True\n"; }
    else { return_string += "False\n"; }
    return_string += "Rotational Period (days): " + std::to_string(rotational_period) + "\n";
    return_string += "Axial Tilt (degrees): " + std::to_string((int)axial_tilt) + "\n";
    //Describe planet
    if (planet_class == planet_type::terrestrial or planet_class == planet_type::chunk)
    {
        return_string += Solid::describe_solid();
    }
    else 
    {
        return_string += Solid::describe_non_solid();
    }
    //Describe planet type, in text
    switch (planet_class)
    {
    case planet_type::asteroid_belt:
        return_string += "This world failed to form, and is just an asteroid belt- the values in the stat block above are either averages or totals over the whole belt, or do not apply.\n "; break;
    case planet_type::terrestrial:
        return_string += "This is a rocky, terrestrial planet, of substantial size.\n "; break;
    case planet_type::chunk:
        return_string += "This is a dwarf planet, or \"chunk\", which is smaller than a proper planet and which has substantial clutter in its orbit.\n "; break;
    case planet_type::gas_giant:
        return_string += "This is a gas giant planet, like the outer planets of our solar system.\n "; break;
    case planet_type::superjovian:
        return_string += "This is a supersized gas giant, much larger than any in our solar system.\n "; break;
    }
    return_string += "\n";
    //Describe moons
    for (unsigned i = 0; i < moons.size(); i++) 
    {
        return_string += moons[i].describe_moon();
    }
    return return_string;
}