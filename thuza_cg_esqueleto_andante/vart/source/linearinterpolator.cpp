/// \file linearinterpolator.cpp
/// \brief Implementation file for V-ART class "LinearInterpolator".
/// \version $Revision: 1.2 $

#include "vart/linearinterpolator.h"
#include <cmath>

//#include <iostream>
using namespace std;

VART::LinearInterpolator::LinearInterpolator()
{
}

//~ float VART::LinearInterpolator::GetValue(float n) const
//~ // virtual method
//~ {
    //~ return fabsf(fmodf(n + 1.0f,2.0f) - 1.0f);
//~ }

float VART::LinearInterpolator::GetValue(float linearIndex, float initialPos, float range) const
// virtual method
{
    return linearIndex * range + initialPos;
}
