/// \file rotationaction.cpp
/// \brief Implementation file for V-ART class "RotationAction".
/// \version $Revision: 1.1 $

#include "vart/rotationaction.h"
#include "vart/transform.h"

#include <iostream>
using namespace std;

VART::RotationAction::RotationAction()
{
}

void VART::RotationAction::SetXRotation(double radians)
{
    goal = radians;
    refPoint = Point4D::ORIGIN();
    refVector = Point4D::X();
}

void VART::RotationAction::SetYRotation(double radians)
{
    goal = radians;
    refPoint = Point4D::ORIGIN();
    refVector = Point4D::Y();
}

void VART::RotationAction::SetZRotation(double radians)
{
    goal = radians;
    refPoint = Point4D::ORIGIN();
    refVector = Point4D::Z();
}

// virtual
void VART::RotationAction::Move()
{
    bool cycled;
    ComputePositionIndex(&cycled);
    targetPtr->MakeRotation(refPoint, refVector, goal * positionIndex);
}

