/// \file rangesineinterpolator.cpp
/// \brief Implementation file for V-ART class "Xml".
/// \version $Revision: 1.3 $

#include <iostream>
#include "vart/rangesineinterpolator.h"
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>


using namespace std;

VART::RangeSineInterpolator::RangeSineInterpolator()
{
    minLimit = 0.0;
    maxLimit = 1.0;
}

void VART::RangeSineInterpolator::SetMinimunLimit(float minLim)
{
    minLimit = minLim;
}

void VART::RangeSineInterpolator::SetMaximunLimit(float maxLim)
{
    maxLimit = maxLim;
}

//~ float VART::RangeSineInterpolator::GetValue(float n) const
//~ // virtual method
//~ {
    //~ return (((sinf((n+1.5)*M_PI)+1)/2.0f)*(maxLimit-minLimit))+minLimit;
//~ }

float VART::RangeSineInterpolator::GetValue(float linearIndex, float initialPos, float range) const
// virtual method
{
    return ((((sinf((linearIndex+1.5)*M_PI)+1)/2.0f)*(maxLimit-minLimit))+minLimit)*range+initialPos;
}
