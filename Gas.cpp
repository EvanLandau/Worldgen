#include "Gas.h"

Gas::Gas(enum class gases type)
{
    gas = type;
    molecular_weight = mol_weights [(int) gas];
};