/// \file shearaction.cpp
/// \brief Implementation file for V-ART class "ShearAction".
/// \version $Revision: 1.1 $

#include "vart/shearaction.h"
#include "vart/transform.h"

//#include <iostream>
//using namespace std;

VART::ShearAction::ShearAction() : goalX(0), goalY(0)
{
}

void VART::ShearAction::SetShear(double shX, double shY)
{
    goalX = shX;
    goalY = shY;
}

// virtual
void VART::ShearAction::Move()
{
    bool cycled;
    ComputePositionIndex(&cycled);
    targetPtr->MakeShear(goalX * positionIndex, goalY * positionIndex);
}
