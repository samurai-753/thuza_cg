/// \file bezier.cpp
/// \brief Implementation file for V-ART class "Bezier".
/// \version $Revision: 1.1 $

#include "vart/bezier.h"
#include <cassert>

VART::Bezier::Bezier() {
    // Allocate memory
    controlPoints.reserve(4);
    // Set the number of control points. It seems to exist no way of setting the
    // number of control points without initializing all of them.
    controlPoints.assign(4,VART::Point4D::ORIGIN());
}

VART::Point4D VART::Bezier::GetControlPoint(int i) {
    assert (i < 4);
    assert (i >= 0);
    return controlPoints[i];
}

void VART::Bezier::SetControlPoint(int i, const VART::Point4D& point) {
    assert (i < 4);
    assert (i >= 0);
    controlPoints[i] = point;
}

void VART::Bezier::GetPoint(double t, VART::Point4D* result) {
    double invT = 1-t; // Cache this common value
    double t2 = t*t; // Cache this common value
    VART::Point4D p0(controlPoints[0]);
    VART::Point4D p1(controlPoints[1]);
    VART::Point4D p2(controlPoints[2]);
    VART::Point4D p3(controlPoints[3]);

    p0 *= invT*invT*invT;
    p1 *= 3*t*invT*invT;
    p2 *= 3*t2*invT;
    p3 *= t2*t;
    *result = p0 + p1 + p2 + p3;
}
