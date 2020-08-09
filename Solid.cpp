#include "Solid.h"

void Solid::calculate_composition(double density, bool inner_zone) //From 3.1.1, pg 20
{

    if (inner_zone) 
    {
        if (density <= 0.7) {body_composition = Solid::composition::silicate_metal_core;}
        else if (density <= 1.0) {body_composition = Solid::composition::iron_nickel_medium_core;}
        else {body_composition = Solid::composition::iron_nickel_large_core;}
    }
    else
    {
        if (density <= 0.3) {body_composition = Solid::composition::ice;}
        else if (density <= 0.45) {body_composition = Solid::composition::ice_silicate_core;}
        else {body_composition = Solid::composition::silicate_metal_core;}
    }
}

void Solid::calculate_tectonics(double mass, double density, double age, bool has_major_moons, Solid::composition comp, double rotational_period) //From 3.2.1 & 3.2.2, pg. 21
{

    //Calculate tectonic factor
    double tectonic_factor = (5.0 + RNG::d10()) * sqrt(mass) / age;
    if (has_major_moons) {tectonic_factor *= 1.25;}
    if (body_composition == composition::ice || body_composition == composition::ice_silicate_core) {tectonic_factor *= density;}
    if (rotational_period <= 0.75) {tectonic_factor *= 1.25;}
    if (rotational_period >= 4.25) {tectonic_factor *= 0.75;}
    //Roll on table 3.2.2
    int selection_roll = RNG::d10();
    if (tectonic_factor < 0.5) {body_tectonics = Solid::tectonics::dead;}
    else if (tectonic_factor < 1.0) 
    {
        if (selection_roll < 8) {body_tectonics = Solid::tectonics::dead;}
        else if (selection_roll < 10) {body_tectonics = Solid::tectonics::hot_spot;}
        else {body_tectonics = Solid::tectonics::plastic;}
    }
    else if (tectonic_factor < 2) 
    {
        if (selection_roll < 2) {body_tectonics = Solid::tectonics::dead;}
        else if (selection_roll < 6) {body_tectonics = Solid::tectonics::hot_spot;}
        else if (selection_roll < 10) {body_tectonics = Solid::tectonics::plastic;}
        else {body_tectonics = Solid::tectonics::plate_tectonic;}
    }
    else if (tectonic_factor < 3) 
    {
        if (selection_roll < 3) {body_tectonics = Solid::tectonics::hot_spot;}
        else if (selection_roll < 7) {body_tectonics = Solid::tectonics::plastic;}
        else {body_tectonics = Solid::tectonics::plate_tectonic;}
    }
    else if (tectonic_factor < 5) 
    {
        if (selection_roll < 2) {body_tectonics = Solid::tectonics::hot_spot;}
        else if (selection_roll < 4) {body_tectonics = Solid::tectonics::plastic;}
        else if (selection_roll < 9) {body_tectonics = Solid::tectonics::plate_tectonic;}
        else {body_tectonics = Solid::tectonics::platelet_tectonic;}
    }
    else 
    {
        if (selection_roll < 2) {body_tectonics = Solid::tectonics::plastic;}
        else if (selection_roll < 3) {body_tectonics = Solid::tectonics::plate_tectonic;}
        else if (selection_roll < 8) {body_tectonics = Solid::tectonics::platelet_tectonic;}
        else {body_tectonics = Solid::tectonics::extreme;}
    }
}

void Solid::calculate_magnetic_field(Solid::composition body_composition, double mass, double density, double rotational_period, double age) //Calculates magnetic field strength using 3.3.1 & 3.3.2 (pg. 23)
{
    //Calculate magnetic field factor on table 3.3.1
    double magnetic_factor = 10 * 1 / sqrt(rotational_period) * pow(density, 2) * sqrt(mass) / age;
    if ((body_composition == Solid::composition::ice) || (body_composition == Solid::composition::ice_silicate_core)) {magnetic_factor *= 0.5;}
    //Roll on field strength table 3.3.2
    int selection_value = RNG::d10();

    double magnetic_field;
    if (magnetic_factor < 0.05) {magnetic_field = 0;}
    else if (magnetic_factor < 0.5)
    {
        if (selection_value < 6) {magnetic_field = 0;}
        else if (selection_value < 8) {magnetic_field = RNG::d10() * 0.001;}
        else if (selection_value < 10) {magnetic_field = RNG::d10() * 0.002;}
        else {magnetic_field = RNG::d10() * 0.01;}
    }
    else if (magnetic_factor < 1)
    {
        if (selection_value < 4) {magnetic_field = 0;}
        else if (selection_value < 6) {magnetic_field = RNG::d10() * 0.001;}
        else if (selection_value < 8) {magnetic_field = RNG::d10() * 0.002;}
        else if (selection_value < 10) {magnetic_field = RNG::d10() * 0.01;}
        else {magnetic_field = RNG::d10() * 0.05;}
    }
    else if (magnetic_factor < 2)
    {
        if (selection_value < 4) {magnetic_field = RNG::d10() * 0.001;}
        else if (selection_value < 6) {magnetic_field = RNG::d10() * 0.002;}
        else if (selection_value < 8) {magnetic_field = RNG::d10() * 0.01;}
        else if (selection_value < 10) {magnetic_field = RNG::d10() * 0.05;}
        else {magnetic_field = RNG::d10() * 0.1;}
        
    }
    else if (magnetic_factor < 4)
    {
        if (selection_value < 4) {magnetic_field = RNG::d10() * 0.05;}
        else if (selection_value < 6) {magnetic_field = RNG::d10() * 0.1;}
        else if (selection_value < 8) {magnetic_field = RNG::d10() * 0.2;}
        else if (selection_value < 10) {magnetic_field = RNG::d10() * 0.3;}
        else {magnetic_field = RNG::d10() * 0.5;}
    }
    else 
    {
        if (selection_value < 4) {magnetic_field = RNG::d10() * 0.1;}
        else if (selection_value < 6) {magnetic_field = RNG::d10() * 0.2;}
        else if (selection_value < 8) {magnetic_field = RNG::d10() * 0.3;}
        else if (selection_value < 10) {magnetic_field = RNG::d10() * 0.5;}
        else {magnetic_field = RNG::d10() * 1.0;}
    }

    Solid::magnetic_field = magnetic_field;
}

void Solid::calculate_surface_temp(double orbital_distance, double luminosity) //Luminosity from 4.1.1, pg. 27
{
    temp = (int) (255 / sqrt(orbital_distance / sqrt(luminosity)));
}

void Solid::calculate_hydrosphere(bool inner_zone, int surface_temp, int radius) //Hydrosphere from 4.2.1, 4.2.2, & 4.2.3 (pg. 28) 
{

    //Determine hydrosphere type (4.2.1)
    enum Solid::hydrosphere hydrosphere_type;
    int hydrosphere_extensiveness = 0;
    if (inner_zone) 
    {
        if (surface_temp > 500) {hydrosphere_type = Solid::hydrosphere::none;}
        else if (surface_temp > 370) {hydrosphere_type = Solid::hydrosphere::vapor;}
        else if (surface_temp > 245) {hydrosphere_type = Solid::hydrosphere::liquid;}
        else {hydrosphere_type = Solid::hydrosphere::ice_sheet;}
    }
    else {hydrosphere_type = Solid::hydrosphere::crustal;}
    //Calculate hydrosphere extensiveness
    if (hydrosphere_type != Solid::hydrosphere::none)
    {
        if (radius < 2000) 
        {
            switch(RNG::d10()) 
            {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    hydrosphere_type = Solid::hydrosphere::none;
                    break;
                case 6:
                case 7:
                    hydrosphere_extensiveness = RNG::d10();
                    break;
                case 8:
                    hydrosphere_extensiveness = 10 + RNG::d10();
                    break;
                case 9:
                    hydrosphere_extensiveness = 5 * RNG::d10();
                    break;
                case 10:
                    hydrosphere_extensiveness = 10 * RNG::d10() + 10;
                    break;
            }
        }
        else if (radius < 4000) 
        {
            switch(RNG::d10()) 
            {
                case 1:
                case 2:
                    hydrosphere_type = Solid::hydrosphere::none;
                    break;
                case 3:
                case 4:
                    hydrosphere_extensiveness = RNG::d10();
                    break;
                case 5:
                    hydrosphere_extensiveness = 10 + RNG::d10();
                    break;
                case 6:
                    hydrosphere_extensiveness = 10 + RNG::d10();
                    break;
                case 7:
                    hydrosphere_extensiveness = 10 + RNG::d10();
                    break;
                case 8:
                    hydrosphere_extensiveness = 10 + RNG::d10();
                    break;
                case 9:
                    hydrosphere_extensiveness = 10 + RNG::d10();
                    break;
                case 10:
                    hydrosphere_extensiveness = 10 + 10 * RNG::d10();
                    break;
            }
        }
        else if (radius < 7000) 
        {
            switch(RNG::d10()) 
            {
                case 1:
                    hydrosphere_type = Solid::hydrosphere::none;
                    break;
                case 2:
                    hydrosphere_extensiveness = 2 * RNG::d10();
                    break;
                case 3:
                    hydrosphere_extensiveness = 20 + RNG::d10();
                    break;
                case 4:
                    hydrosphere_extensiveness = 30 + RNG::d10();
                    break;
                case 5:
                    hydrosphere_extensiveness = 40 + RNG::d10();
                    break;
                case 6:
                    hydrosphere_extensiveness = 50 + RNG::d10();
                    break;
                case 7:
                    hydrosphere_extensiveness = 60 + RNG::d10();
                    break;
                case 8:
                    hydrosphere_extensiveness = 70 + RNG::d10();
                    break;
                case 9:
                    hydrosphere_extensiveness = 80 + 2 * RNG::d10();
                    break;
                case 10:
                    hydrosphere_extensiveness = 100;
                    break;
            }
        }
        else
        {
            switch(RNG::d10()) 
            {
                case 1:
                    hydrosphere_type = Solid::hydrosphere::none;
                    break;
                case 2:
                    hydrosphere_extensiveness = 2 * RNG::d10();
                    break;
                case 3:
                    hydrosphere_extensiveness = 20 + 2 * RNG::d10();
                    break;
                case 4:
                    hydrosphere_extensiveness = 40 + 2 * RNG::d10();
                    break;
                case 5:
                    hydrosphere_extensiveness = 60 + RNG::d10();
                    break;
                case 6:
                    hydrosphere_extensiveness = 70 + RNG::d10();
                    break;
                case 7:
                    hydrosphere_extensiveness = 80 + RNG::d10();
                    break;
                case 8:
                    hydrosphere_extensiveness = 90 + RNG::d10();
                    break;
                case 9:
                    hydrosphere_extensiveness = 100;
                    break;
                case 10:
                    hydrosphere_extensiveness = 100;
                    break;
            }
        }
    }
    if (hydrosphere_type == Solid::hydrosphere::none) {hydrosphere_extensiveness = 0;}
    if (hydrosphere_extensiveness > 100) {hydrosphere_extensiveness = 100;}

    Solid::hydrosphere_type = hydrosphere_type;
    Solid::hydrosphere_extensiveness = hydrosphere_extensiveness;
}

void Solid::calculate_atmosphere(int surface_temp, double mass, double escape_velocity, Solid::tectonics tectonics) 
{
    
    //Atmospheric Gases (4.3.1)
    std::vector <Gas> atmosphere;
    if (surface_temp > 400) 
    {
        switch (RNG::d10())
        {
        case 1:
        case 2:
        case 3:
        case 4:
            atmosphere.push_back(Gas(Gas::gases::n2));
            atmosphere.push_back(Gas(Gas::gases::co2));
            break;
        case 5:
        case 6:
            atmosphere.push_back(Gas(Gas::gases::co2));
            break;
        case 7:
        case 8:
            atmosphere.push_back(Gas(Gas::gases::no2));
            atmosphere.push_back(Gas(Gas::gases::so2));
            break;
        case 9:
        case 10:
            atmosphere.push_back(Gas(Gas::gases::so2));
            break;
        }
    }
    else if (surface_temp > 240) 
    {
        switch (RNG::d10())
        {
        case 1:
        case 2:
        case 3:
        case 4:
            atmosphere.push_back(Gas(Gas::gases::n2));
            atmosphere.push_back(Gas(Gas::gases::co2));
            break;
        case 5:
        case 6:
            atmosphere.push_back(Gas(Gas::gases::co2));
            break;
        case 7:
        case 8:
            atmosphere.push_back(Gas(Gas::gases::n2));
            atmosphere.push_back(Gas(Gas::gases::ch4));
            break;
        case 9:
        case 10:
            atmosphere.push_back(Gas(Gas::gases::co2));
            atmosphere.push_back(Gas(Gas::gases::ch4));
            atmosphere.push_back(Gas(Gas::gases::nh3));
            break;
        }
    }
    else if (surface_temp > 150) 
    {
        switch (RNG::d10())
        {
        case 1:
        case 2:
        case 3:
        case 4:
            atmosphere.push_back(Gas(Gas::gases::n2));
            atmosphere.push_back(Gas(Gas::gases::co2));
            break;
        case 5:
        case 6:
            atmosphere.push_back(Gas(Gas::gases::co2));
            break;
        case 7:
        case 8:
            atmosphere.push_back(Gas(Gas::gases::n2));
            atmosphere.push_back(Gas(Gas::gases::ch4));
            break;
        case 9:
        case 10:
            atmosphere.push_back(Gas(Gas::gases::h2));
            atmosphere.push_back(Gas(Gas::gases::he));
            break;
        }
    }
    else if (surface_temp > 50) 
    {
        switch (RNG::d10())
        {
        case 1:
        case 2:
        case 3:
        case 4:
            atmosphere.push_back(Gas(Gas::gases::n2));
            atmosphere.push_back(Gas(Gas::gases::ch4));
            break;
        case 5:
        case 6:
            atmosphere.push_back(Gas(Gas::gases::h2));
            atmosphere.push_back(Gas(Gas::gases::he));
            atmosphere.push_back(Gas(Gas::gases::n2));
            break;
        case 7:
        case 8:
            atmosphere.push_back(Gas(Gas::gases::n2));
            atmosphere.push_back(Gas(Gas::gases::co));
            break;
        case 9:
        case 10:
            atmosphere.push_back(Gas(Gas::gases::he));
            atmosphere.push_back(Gas(Gas::gases::h2));
            break;
        }
    }
    else 
    {
        switch (RNG::d10())
        {
        case 1:
        case 2:
        case 3:
        case 4:
            atmosphere.push_back(Gas(Gas::gases::h2));
            break;
        case 5:
        case 6:
            atmosphere.push_back(Gas(Gas::gases::he));
            break;
        case 7:
        case 8:
            atmosphere.push_back(Gas(Gas::gases::he));
            atmosphere.push_back(Gas(Gas::gases::h2));
            break;
        case 9:
        case 10:
            atmosphere.push_back(Gas(Gas::gases::ne));
            break;
        }
    }

    //Gas Retentiion Check (4.3.2)
    double min_molecular_weight = 0.02783 * surface_temp / pow(escape_velocity, 2);
    for (unsigned i = 0; i < atmosphere.size(); i++) 
    {
        if (atmosphere[i].molecular_weight > min_molecular_weight) 
        {
            Solid::gases.push_back(atmosphere[i]);
        }
    }

    //Atmospheric Modifications (4.3.4)
    //TODO: Add UV infall
    //TODO: Volcanism
    //Life should perhaps be done elsewhere???

    //Roll Total Pressure (4.3.5)
    int selection_roll = RNG::d10();
    if (body_tectonics == Solid::tectonics::dead) { selection_roll -= 1; }
    if (body_tectonics == Solid::tectonics::extreme) { selection_roll += 1; }
    if (atmosphere.size() > gases.size()) { selection_roll -= 1; }
    if (selection_roll < 3) { total_pressure = RNG::d10() * 0.01; }
    else if (selection_roll < 5) { total_pressure = RNG::d10() * 0.1; }
    else if (selection_roll < 8) { total_pressure = RNG::d10() * 0.2; }
    else if (selection_roll == 8) { total_pressure = RNG::d10() * 0.5; }
    else if (selection_roll == 9) { total_pressure = RNG::d10() * 2.0; }
    else { total_pressure = RNG::d10() * 20.0; }
    if (gases.size() == 0) { total_pressure = 0; } //IF there are no gases in the atmosphere, set pressure to zero
    Solid::total_pressure = total_pressure;

    //Calculate Partial Pressure of Each Component
    for (unsigned i = 0; i < gases.size(); i++) 
    {
        double portion;
        selection_roll = RNG::d10(); //Table 4.3.6
        if (selection_roll < 6) { portion = 0.5 + 0.04 * RNG::d10(); }
        else if (selection_roll < 9) { portion = 0.75 + 0.02 * RNG::d10(); }
        else { portion = 0.95 + 0.005 * RNG::d10(); }
        if (i != gases.size() - 1) //If it is not the final gas, give it a partial pressure equal to the percentage it makes up of the remaining part of hte atmosphere multiplied by the remaining atmosphere.
        {
            gases[i].partial_pressure = total_pressure * portion;
            total_pressure -= total_pressure * portion;
        }
        else //If it is the last gas, give it the remaining atmosphere.
        {
            gases[i].partial_pressure = total_pressure;
        }
    }
}

void Solid::calculate_albedo(bool inner_zone, double total_pressure, hydrosphere hydrosphere_type, int hydrosphere_extensiveness)
{

    //Calculate Albedo Factor (4.4.1, pg. 32)
    double albedo;
    int selection_roll = RNG::d10();
    if (inner_zone) 
    {
        if (total_pressure == 0) { selection_roll += 2; }
        else if (total_pressure >= 5) { selection_roll -= 2; }
        else if (total_pressure >= 50) { selection_roll -= 4; }
        if (hydrosphere_type == Solid::hydrosphere::ice_sheet && hydrosphere_extensiveness >= 90) { selection_roll -= 2; }
        else if (hydrosphere_type == Solid::hydrosphere::ice_sheet && hydrosphere_extensiveness >= 50) { selection_roll -= 4; }
        
        if (selection_roll < 2) { albedo = 0.75 + RNG::d10() * 0.01; }
        else if (selection_roll < 4) { albedo = 0.85 + RNG::d10() * 0.01; }
        else if (selection_roll < 7) { albedo = 0.95 + RNG::d10() * 0.01; }
        else if (selection_roll < 10) { albedo = 1.05 + RNG::d10() * 0.01; }
        else { albedo = 1.15 + RNG::d10() * 0.01; }
    }
    else 
    {
        if (total_pressure >= 1) { selection_roll += 1; }
        //Density separated moons are ignored here

        if (selection_roll < 4) { albedo = 0.75 + RNG::d10() * 0.01; }
        else if (selection_roll < 6) { albedo = 0.85 + RNG::d10() * 0.01; }
        else if (selection_roll < 8) { albedo = 0.95 + RNG::d10() * 0.01; }
        else if (selection_roll < 10) { albedo = 1.05 + RNG::d10() * 0.01; }
        else { albedo = 1.15 + RNG::d10() * 0.01; }
    }

    Solid::albedo = albedo;
}

void Solid::recalc_surface_temp(double total_pressure, std::vector <Gas> gases, double vapor_factor, double albedo, int surface_temp)
{

    //Calculate Greenhouse Factor (4.5.1)
    double greenhouse_gas_pressure = 0;
    for (unsigned i = 0; i < gases.size(); i++) 
    {
        if (gases[i].gas == Gas::gases::co2 || gases[i].gas == Gas::gases::ch4 || gases[i].gas == Gas::gases::so2 || gases[i].gas == Gas::gases::no2) 
        {
            greenhouse_gas_pressure += gases[i].partial_pressure;
        }
    }

    double greenhouse_factor = 1 + sqrt(total_pressure) * 0.01 * RNG::d10() * sqrt(greenhouse_gas_pressure) * 0.1 * vapor_factor * 0.1;

    //Calculate Surface Temp (4.5.2)
    Solid::temp = (int) ((double)surface_temp * albedo * greenhouse_factor);
}

std::string Solid::describe_solid()
{
    std::string return_string = "";
    //Stat block
    return_string += "Magnetic Field Strength (Earth Relative): " + std::to_string(magnetic_field) + "\n";
    return_string +="Surface Temperature (K): " + std::to_string(temp) + "\n";
    return_string += "Atmospheric Pressure (Earth Relative): " + std::to_string(total_pressure) + "\n";
    return_string += "Albedo: " + std::to_string(albedo) + "\n";

    //Composition
    switch (body_composition)
    {
    case composition::silicate_metal_core:
        return_string += "This is a rocky body, with a silicate-based crust and a small metallic core. "; break;
    case composition::iron_nickel_medium_core:
        return_string += "This body is mostly composed of iron-nickel rocks, with a medium sized metallic core. "; break;
    case composition::iron_nickel_large_core:
        return_string += "This body has iron-nickel outer layers, with a large core of denser metals. "; break;
    case composition::ice:
        return_string += "This body is entirely composed of various types of ice. "; break;
    case composition::ice_silicate_core:
        return_string += "This object is mostly ice, with a silicate core. "; break;
    }
    //Tectonics
    switch (body_tectonics)
    {
    case tectonics::dead:
        return_string += "This object has little to no tectonic activity. "; break;
    case tectonics::hot_spot:
        return_string += "This world has tectonic activity in a few small areas, where there are large volcanoes or geysers. "; break;
    case tectonics::plastic:
        return_string += "This world's thick crust shifts and deforms, creating some higher \"continents\", but without proper plates. Volcanoes are concentrated in high-elevation hot spots. "; break;
    case tectonics::plate_tectonic:
        return_string += "This body has a crust split into different plates, that shift on the surface. They have varying thicknesses, volcanoes erupt through them here and there, and their collisions form great mountain ranges. "; break;
    case tectonics::platelet_tectonic:
        return_string += "This body has a very thin crust with small plates, that are constantly being pulled and pushed over and under the surface, accompanied by intense volcanism. "; break;
    case tectonics::extreme:
        return_string += "This body's tectonic activity is chaotic and intense. The surface is constantly in flux, with huge eruptions, and may be oddly shaped due to tidal forces. "; break;
    }
    //Magnetic Field
    if (magnetic_field == 0)
    {
        return_string += "This world lack's a magnetic field entirely. ";
    }
    else if (magnetic_field <= 0.1)
    {
        return_string += "This world has a very weak magnetic field, less than 10% of Earth's. It provides little protection to the surface. ";
    }
    else if (magnetic_field <= 0.75)
    {
        return_string += "This body has a weak magnetic field, substantially less than Earth. It provides some protection from solar radiation. ";
    }
    else if (magnetic_field <= 1.25)
    {
        return_string += "This body has an Earth-like magnetic field. It provides strong protection from radiation to the surface, and aurorae can be seen near the poles. ";
    }
    else if (magnetic_field <= 3)
    {
        return_string += "This body has a strong magnetic field. The surface is well-protected and the planet exhibits strong aurorae, but it also has dangerous radiation belts. ";
    }
    else
    {
        return_string += "This world has a very strong magnetic field. This field is very effective at trapping radiation, creating dangerous radiation belts and incredible aurorae. ";
    }
    //Surface Temp
    if (temp < 25) 
    {
        return_string += "The temperatures here are absolutely frigid- many substances that are normally found as gases are liquids or solids on the surface. ";
    }
    else if (temp < 100) 
    {
        return_string += "The temperature of this world is very cold and inhospitable. ";
    }
    else if (temp < 200) 
    {
        return_string += "This body is freezing, but may support some unusual forms of life, or a few warm oases. ";
    }
    else if (temp < 350) 
    {
        return_string += "This world is relatively hospitable, and may support hardy water-based life. ";
    }
    else if (temp < 500) 
    {
        return_string += "This body is too hot for most forms of life, but some exotic life forms may be able to survive. ";
    }
    else if (temp < 1000) 
    {
        return_string += "The surface of this object is scorching, unsuitable for nearly any kind of life. ";
    }
    else
    {
        return_string += "The hellish surface of this body is so hot that it glows! ";
    }
    //Hydrosphere
    switch (hydrosphere_type) 
    {
        case hydrosphere::none:
            return_string += "It lacks any sort of hydrosphere- any water that once was here has since been lost to space. "; break;
        case hydrosphere::vapor:
            return_string += "All its water has become a part of its atmosphere. "; break;
        case hydrosphere::liquid:
            return_string += "It has liquid water, covering approximately " + std::to_string(hydrosphere_extensiveness) + " percent of the surface, and cycling through the atmosphere. "; break;
        case hydrosphere::ice_sheet:
            return_string += "Its water is in the form of an ice sheet on the surface, covering approximately " + std::to_string(hydrosphere_extensiveness) + " percent of the surface. "; break;
        case hydrosphere::crustal:
            return_string += "The only water here makes up a sizeable portion of the crust, about " + std::to_string(hydrosphere_extensiveness) + " percent of the crust by mass. "; break;
    }
    //Atmosphere
    if (total_pressure == 0) 
    {
        return_string += "This body has only an insignificant trace atmosphere. ";
    }
    else
    {
        if (total_pressure < 0.1)
        {
            return_string += "This world has a thin atmosphere, providing little protection against radiation and impacts. Its main components are: ";
        }
        else if (total_pressure < 0.5)
        {
            return_string += "This body has a somewhat thin atmosphere, which provides substantial protection against radiation and impacts. It is composed of: ";
        }
        else if (total_pressure < 1.5)
        {
            return_string += "This world has a thick, eartlike atmosphere, which protects the surface, and mainly consists of: ";
        }
        else if (total_pressure < 5)
        {
            return_string += "This world has a very thick atmosphere, which limits even visible light reaching the surface and creates intense pressures there. It contains: ";
        }
        else
        {
            return_string += "This world has an incredibly thick, crushing atmosphere. Very little light can reach the surface, and almost nothing can survive. It is made up of: ";
        }
        for (unsigned i = 0; i < gases.size() - 1; i++)
        {
            return_string += gas_names[(int)gases[i].gas] + ", ";
        }
        return_string += "and " + gas_names[(int)gases[gases.size() - 1].gas] + ". ";
    }
    //Albedo
    //Albedo should probably not be described here, in my opinion

    return return_string;
}