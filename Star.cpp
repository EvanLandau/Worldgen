#include "Star.h"

Star::Star() {
    //Generate star type (pg. 4)
    int class_roll = RNG::d100(); //Determine spectral class from roll
    if (class_roll == 1)
    {
        spectral_class = star_class::A;
        size_code = 5;
    }
    else if (class_roll < 5) 
    {
        spectral_class = star_class::F;
        size_code = 5;
    }
    else if (class_roll < 13)
    {
        spectral_class = star_class::G;
        size_code = 5;
    }
    else if (class_roll < 27) 
    {
        spectral_class = star_class::K;
        size_code = 5;
    }
    else if (class_roll < 37) //White dwarf code- currently non-functional
    {
        spectral_class = star_class::white_dwarf;
        size_code = 7;
    }
    else if (class_roll < 86) 
    {
        spectral_class = star_class::M;
        size_code = 5;
    }
    else if (class_roll < 99) //Brown dwarf code- currently non-functional
    {
        spectral_class = star_class::brown_dwarf;
        size_code = -1;
    }
    else 
    {
        size_code = 3;
        //Roll spectral class for giant stars
        switch (RNG::d10())
        {
            case 1: //Roll of 1, F class
                spectral_class = star_class::F;
                break;
            case 2: //Roll of 2, G class
                spectral_class = star_class::G;
                break;
            case 3: //Roll of 3-7,  K class
            case 4:
            case 5:
            case 6:
            case 7:
                spectral_class = star_class::K;
                break;
            case 8: //Roll of 8-10, K/subgiant class
            case 9:
            case 10:
                spectral_class = star_class::K;
                size_code = 4;
        }
    }
    subclass = RNG::d10() - 1;
    //Table 1.1.3 - I'm not implementing this as a table for sanity reasons.
    //Turn star type and size into a selection (from 0 to 14)
    //Since special stars are not implemented yet, no B-class stars can be rolled, so 0 is never needed
    int selection_value;
    switch(spectral_class) 
    {
        case star_class::A: //A Class stars
            if (size_code == 5) { selection_value = 1; }
            else if (size_code == 4) {selection_value = 6; }
            else { selection_value = 10; }
            break;
        case star_class::F: //F Class stars
            if (size_code == 5) { selection_value = 2; }
            else if (size_code == 4) {selection_value = 7; }
            else { selection_value = 11; }
            break;
        case star_class::G: //G Class stars
            if (size_code == 5) { selection_value = 3; }
            else if (size_code == 4) {selection_value = 8; }
            else { selection_value = 12; }
            break;
        case star_class::K: //K Class stars
            if (size_code == 5) { selection_value = 4; }
            else if (size_code == 4) {selection_value = 9; }
            else { selection_value = 13; }
            break;
        case star_class::M: //M Class stars
            if (size_code == 5) { selection_value = 5; }
            else { selection_value = 14; }
            break;
        case star_class::white_dwarf: //White Dwarves
        case star_class::brown_dwarf: //Brown Dwarves
            selection_value = 15;
    }
    if (selection_value < 15) {
        //These arrays list the value in the 0 column for each star type
        //long double zero_luminosity[15] = { 13000.0, 80, 6.4, 1.36, 0.46, 0.12, 156.0, 19.0, 6.2, 4.0, 280.0, 53.0, 50.0, 120.0, 470.0 }; //Deprecated in favor of mass-based calculation on page 6
        double zero_mass[15] = { 17.5, 3.0, 1.6, 1.08, 0.82, 0.46, 6.0, 2.5, 1.6, 1.4, 12.0, 8.0, 2.5, 3.0, 6.2 };
        int zero_temp[15] = { 28000, 10000, 7500, 6000, 5000, 3500, 9700, 7300, 5900, 4700, 9500, 7200, 5800, 4500, 3400 };
        //double zero_radius [15] = {4.9, 3, 1.5, 1.1, 0.9, 0.8, 4.5, 2.7, 2.4, 3.0, 6.2, 4.7, 7.1, 18.2, 63.0}; //Deprecated in favor of luminosity calculation on page 6
        //Luminosity is calculated using an exponentia
        //long double luminosity_ratio[15] = { 0.67, 0.75, 0.86, 0.89, 0.5, 0.81, 0.89, 0.95, 1, 0.86, 0.96, 1.1, 1.4 };
        //Mass is calculated linearly
        double mass_difference[15] = { -1.4, -0.13, -0.06, -0.03, -0.02, -0.05,  -0.3, -0.1, -0.02, 0, -0.5, -0.52, 0.03, 0.3, 0.2 };
        Star::mass = zero_mass[selection_value] + mass_difference[selection_value] * subclass;
        //Luminosity is calculated using equation listed about halfway down page 6
        Star::luminosity = pow(mass, 4);
        //Temperature is calculated linearly
        int temp_difference[15] = { -3000, -250, -150, -100, -150, -150, -250, -100, -150, 0, -250, -150, -100, -100, -100 };
        Star::temp = zero_temp[selection_value] + temp_difference[selection_value] * subclass;
        /**Radius is calculated linearly (this is not a particularly good approximation but I don't have an obviously better one) //Deprecated in favor of luminosity calculation on page 6
        double radius_difference [15] = {-0.07, -0.14, -0.04, -0.02, -0.01, -0.07, -0.16, -0.03, 0.06, 0, -0.13, 0.21, 0.92, 3.58, 29.7};
        radius = zero_radius [selection_value] + radius_difference [selection_value] * subclass; **/
        Star::radius = pow(luminosity, 0.5) * pow((5800 / temp), 2);
    }
    else 
    {
        int selection_value = RNG::d10() - 1;
        double white_dwarf_mass_table[10] = { 1.3, 1.1, 0.9, 0.7, 0.6, 0.55, 0.50, 0.45, 0.40, 0.35 }; //Data from table 1.1.4, on page 7
        double white_dwarf_radius_table[10] = { 0.004, 0.007, 0.009, 0.010, 0.011, 0.012, 0.013, 0.014, 0.015, 0.016 };
        double white_dwarf_temp_table[10] = { 30000, 25000, 20000, 16000, 14000, 12000, 10000, 8000, 6000, 4000 };
        double brown_dwarf_mass_table[10] = { 0.070, 0.064, 0.058, 0.052, 0.046, 0.040, 0.034, 0.026, 0.020, 0.014 }; //Data from table 1.1.5, on page 7
        double brown_dwarf_radius_table[10] = { 0.07, 0.08, 0.09, 0.10, 0.11, 0.12, 0.12, 0.12, 0.12, 0.12 };
        double brown_dwarf_temp_table[10] = { 2200, 2000, 1800, 1600, 1400, 1200, 1000, 900, 800, 700 };
        switch (spectral_class)
        {
        case star_class::white_dwarf: //White Dwarves
            Star::mass = white_dwarf_mass_table[selection_value];
            Star::radius = white_dwarf_radius_table[selection_value];
            Star::temp = white_dwarf_temp_table[selection_value];
            Star::luminosity = pow(radius, 2) * pow(temp, 4) / pow(5800, 4);
            break;
        case star_class::brown_dwarf: //Brown Dwarves
            Star::mass = brown_dwarf_mass_table[selection_value];
            Star::radius = brown_dwarf_radius_table[selection_value];
            Star::temp = brown_dwarf_temp_table[selection_value];
            Star::luminosity = pow(radius, 2) * pow(temp, 4) / pow(5800, 4);
            break;
        }
    }
    //Modify Subgiants
    if (size_code == 4)
    {
        Star::mass = (1 + 0.1 * (RNG::d10() - 5.0)); //Modifies mass between -50% and 40%
    }
    //Modify Giants (Table 1.1.4 on Page 6)
    if (size_code == 3) 
    {
        Star::mass = (0.2 + 0.1 * RNG::d10()) * Star::mass;
        Star::luminosity = (0.3 + 0.1 * RNG::d10()) * Star::mass;
    }

    //Radius is calculated using the euqation on page 6
    if (selection_value < 15) {
        Star::radius = sqrt(Star::luminosity) * pow((5800 / Star::temp), 2);
    }
    //Flare star calculation (pg. 7)
    if ((spectral_class == star_class::M) && (10 > subclass) && (subclass > 2) && (RNG::d10() < 6)) 
    {
        Star::flare_ratio = (float) (1 + 0.5 * RNG::d10());
    }

    //Age calculation from table 1.2.1 (pg 8)
    double year_multiplier [14] = {0.01, 0.01, 0.1, 0.2, 0.3, 0.5, 1, 1, 1, 1, 0.01, 0.01, 1, 1}; //Includes white dwarf stars as type 4
    double lum_multiplier [14] = {0.01, 0.01, 0.01, 0.5, 1, 1, 1, 1, 0.25, 0.1, 0.01, 0.01, 0.01, 0.01};
    double lifespan [14] = {0.1, 0.1, 0.6, 1.3, 3.2, 5.6, 10, 14, 23, 42, 0.01, 0.01, 50, 50};
    selection_value =  (int) spectral_class * 2 + subclass/5;

    selection_value = RNG::d10();

    switch (size_code) 
    {
    case -1: //Special stars (currently only brown dwarves)
        Star::age = selection_value;
        break;
    case 2: //Giant stars
        Star::age = (float)(lifespan[selection_value] * 1.2);
        break;
    case 4: //Subgiants
        Star::age = (float)(lifespan[selection_value] * 1.1);
        break;
    case 5: //Main sequence
        Star::age = (float)(RNG::d10() * year_multiplier[selection_value]);
        //Star::luminosity = Star::luminosity * age_roll * lum_multiplier[selection_value];
        break;
    case 7: //White dwarves
        Star::age = selection_value;
        break;
    }


    //Omitted lifespan (1.2.2, pg. 8)

    //Abundance modifier calculation (1.2.3, pg. 8)
    selection_value = RNG::d10() + RNG::d10() + (int) Star::age;
    if (selection_value < 10) {Star::abundance = 2;}
    else if (selection_value < 13) {Star::abundance = 1;}
    else if (selection_value < 19) {Star::abundance = 0;}
    else if (selection_value < 22) { Star::abundance = -1;}
    else {Star::abundance = -3;}

    //Planetary Orbit Generation (pg. 11)
    //Number of orbits (1.4.1, pg. 11)
    selection_value = RNG::d10() - Star::abundance;
    int orbit_number = 0;
    if (Star::spectral_class == star_class::K && Star::subclass > 4) {selection_value += 1;}
    else if (Star::spectral_class == star_class::M && Star::subclass < 5) {selection_value += 2;}
    else if (Star::spectral_class == star_class::M && Star::subclass > 4) {selection_value += 3;}
    else if (Star::spectral_class == star_class::brown_dwarf) { selection_value += 5; }
    if (selection_value == 1) {orbit_number = RNG::d10() + 10;}
    else if (selection_value < 6) {orbit_number = RNG::d10() + 5;}
    else if (selection_value < 8) {orbit_number = RNG::d10();}
    else if (selection_value < 10) {orbit_number = RNG::d10()/2;}
    //Generate orbits (1.4.2, pg. 11)
    std::vector <double> orbits;
    std::vector <double> final_orbits;
    if (orbit_number > 0) {
        orbits.push_back(0.05 * Star::mass * Star::mass * RNG::d10()); //Generate first orbit
        if (orbits[0] <= 0)
        {
            orbits[0] = 0.0005;
        }
        for (int i = 1; i < orbit_number; i++)
        {
            orbits.push_back(orbits.back() * (1.1 + 0.1 * RNG::d10()) + 0.1); //Generate subsequent orbits
        }
        //Remove orbits destroyed by the sun (1.4.4, pg 11)
        for(unsigned i = 0; i < orbits.size(); i++)
        {
            if (orbits[i] >= 0.025 * sqrt(luminosity))
            {
                final_orbits.push_back(orbits[i]);
            }
        }
        //If white dwarf, remove certain orbits
        if (Star::spectral_class == star_class::white_dwarf) 
        {
            selection_value = RNG::d10();
            int removal_limit = 0;
            if (Star::mass > 0.9) { selection_value += 4; }
            else if (Star::mass > 0.6) { selection_value += 2; }
            if (selection_value > 11) { removal_limit = 10; }
            else if (selection_value > 8) { removal_limit = 6; }
            else if (selection_value > 4) { removal_limit = 4; }
            else { removal_limit = 2; }
            for (unsigned i = 0; i < orbits.size(); i++) 
            {
                if (orbits[i] > removal_limit)
                {
                    final_orbits.push_back(orbits[i]);
                }
            }
        }
        //Create planets
        for (unsigned i = 0; i < final_orbits.size(); i++) 
        {
            Star::planets.push_back(Planet::Planet(mass, luminosity, age, abundance, final_orbits[i], final_orbits[i] < 4 * sqrt(luminosity)));
        }
    }
} 

std::string Star::describe_star() 
{
    std::string return_string = "";
    //Stat block
    return_string += "Spectral Class Code: ";
    switch (spectral_class) 
    {
        case star_class::A:
            return_string += "A"; break;
        case star_class::F:
            return_string += "F"; break;
        case star_class::G:
            return_string += "G"; break;
        case star_class::K:
            return_string += "K"; break;
        case star_class::M:
            return_string += "M"; break;
        case star_class::brown_dwarf:
        case star_class::white_dwarf:
            return_string += "*";
            break;
    }
    return_string += std::to_string(subclass);
    switch (size_code) 
    {
        case -1:
            return_string += "*"; break;
        case 1:
            return_string += "I"; break;
        case 2:
            return_string += "II"; break;
        case 3:
            return_string += "III"; break;
        case 4:
            return_string += "IV"; break;
        case 5:
            return_string += "V"; break;
        case 6:
            return_string += "VI"; break;
        case 7:
            return_string += "VII"; break;
    }
    return_string += "\n";
    return_string += "Luminosity (Sol Relative): " + std::to_string(Star::luminosity) + "\n";
    return_string += "Mass (Sol Masses): " + std::to_string(Star::mass) + "\n";
    return_string += "Radius (Sol Radii): " + std::to_string(Star::radius) + "\n";
    return_string += "Surface Temperature (K): " + std::to_string(Star::temp) + "\n";
    return_string += "Age (GY): " + std::to_string(Star::age) + "\n";
    return_string += "Flare Ratio: " + std::to_string(Star::flare_ratio) + "\n";
    //Textual Description
    if (size_code != 7) //Calculate color from surface temp (using data from Table 1 on this page: https://courses.lumenlearning.com/astronomy/chapter/colors-of-stars/)
    {
        if (temp < 1000)
        {
            return_string += "This is a dully glowing ";
        }
        else if (temp < 2000)
        {
            return_string += "This is a dim red ";
        }
        else if (temp < 3500)
        {
            return_string += "This is a red ";
        }
        else if (temp < 5000)
        {
            return_string += "This is an orange ";
        }
        else if (temp < 8000)
        {
            return_string += "This is a yellow ";
        }
        else if (temp < 18000)
        {
            return_string += "This is a white ";
        }
        else
        {
            return_string += "This is a blue ";
        }
    }
    switch (size_code) 
    {
        case -1:
            if (spectral_class == star_class::brown_dwarf) { return_string += "brown dwarf \"star\". This object is smaller than a true star, and has not achieved fusion. "; break; }
            else { return_string += "anomalous star. It does not fit into normal stellar classifications. "; break; }
        case 1:
            return_string += "supergiant star. It is many times larger than Sol, although it likely has a much lower density. "; break;
        case 2:
            return_string += "supergiant star. It is many times larger than SOl, although it likely has a much lower density. "; break;
        case 3:
            return_string += "giant star. It is much larger than Sol, with a lower density. "; break;
        case 4:
            return_string += "subgiant. It is on the cusp of being a giant star, and is several times larger than Sol. "; break;
        case 5:
            return_string += "main sequence star. It has a similar size to Sol, and acts in a normal manner. "; break;
        case 6:
            return_string += "subdwarf. It is somewhat smaller than Sol, but is not quite a dwarf star. "; break;
        case 7:
            return_string += "This is a white dwarf. It is a small, incredibly dense ball of matter left over from some ancient star. "; break;
    }
    if (flare_ratio != 1) 
    {
        return_string += "This is a flare star, which occasionally increases its output by " + std::to_string((int)(flare_ratio * 100)) + "% with massive solar flares. This may cause problems for life on nearby planets. ";
    }
    switch (abundance) 
    {
        case -3:
            return_string += "This system is almost totally lacking in heavier elements, like carbon, oxygen, and metals. This makes it much more difficult to form planets. "; break;
        case -1:
            return_string += "This system is somewhat lacking in heavier elements, like oxygen, carbon, and metals. This makes it more difficult for planets to form. "; break;
        case 0:
            return_string += "This system has an average amount of heavier elements, meaning elements other than hydrogen and helium. Planets can form normally. "; break;
        case 1:
            return_string += "This system is rich in elements heavier than hydrogen and helium, like metals. This makes it easier for planets to form. "; break;
        case 2:
            return_string += "This system is extremely rich in heavy elements, like carbon, oxygen, and metals. It is much easier for planets to form here. "; break;
    }
    return_string += "\n\n";
    //Display planets
    for (unsigned i = 0; i < planets.size(); i++) 
    {
        return_string += planets[i].describe_planet();
    }
    return return_string;
}