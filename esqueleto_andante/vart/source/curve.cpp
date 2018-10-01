/// \file curve.cpp
/// \brief Implementation file for V-ART class "Curve".
/// \version $Revision: 1.1 $

#include "vart/curve.h"

void VART::Curve::ApplyTransform(const VART::Transform& t)
{
    unsigned int numCtrlPoints = controlPoints.size();
    for (unsigned int i=0; i < numCtrlPoints; ++i)
        t.ApplyTo(&controlPoints[i]);
}
