/// \file scaleaction.cpp
/// \brief Implementation file for V-ART class "ScaleAction".
/// \version $Revision: 1.1 $

#include "vart/scaleaction.h"
#include "vart/transform.h"

#include <iostream>
using namespace std;

VART::ScaleAction::ScaleAction() : goalX(1), goalY(1), goalZ(1)
{
}

void VART::ScaleAction::SetScale(double sX, double sY, double sZ)
{
    goalX = sX;
    goalY = sY;
    goalZ = sZ;
}

//virtual 
void VART::ScaleAction::Move()
{
    bool cycled;
    ComputePositionIndex(&cycled);
    // use postionIndex to go from 1 to goal
    targetPtr->MakeScale((goalX - 1) * positionIndex + 1,
                         (goalY - 1) * positionIndex + 1,
                         (goalZ - 1) * positionIndex + 1);
}
