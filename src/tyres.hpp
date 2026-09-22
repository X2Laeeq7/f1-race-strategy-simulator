#pragma once
#include "types.hpp"
#include <cmath>

inline double tyreTimeLoss(const TyreCompound& c, int tyreAge){
    double x = c.cliffSteepness * (tyreAge - c.cliffLap);
    double sigmoid_x = 1.0/(1.0 + std::exp(-x));
    return c.linearDeg * tyreAge + c.cliffMagnitude * sigmoid_x;
};