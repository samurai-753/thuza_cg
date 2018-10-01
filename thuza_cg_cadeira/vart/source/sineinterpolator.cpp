/// \file sineinterpolator.cpp
/// \brief Implementation file for V-ART class "SineInterpolator".
/// \version $Revision: 1.2 $

#include "vart/sineinterpolator.h"

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <cmath>

//#include <iostream>
//using namespace std;

VART::SineInterpolator::SineInterpolator()
{
}

//~ float VART::SineInterpolator::GetValue(float n) const
//~ // virtual method
//~ {
    //~ return (sinf((n+1.5)*M_PI)+1)/2.0f;
//~ }

float VART::SineInterpolator::GetValue(float linearIndex, float initialPos, float range) const
// virtual method
{
    return (sinf((linearIndex+1.5)*3.1415926535897932384626433832795)+1) * range/2.0f + initialPos;
}
