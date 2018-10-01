/// \file translationaction.cpp
/// \brief Implementation file for V-ART class "TranslationAction".
/// \version $Revision: 1.1 $

#include "vart/translationaction.h"
#include "vart/transform.h"

#include <iostream>
using namespace std;

VART::TranslationAction::TranslationAction()
{
}

// virtual
void VART::TranslationAction::Move()
{
    bool cycled;
    ComputePositionIndex(&cycled);
    targetPtr->MakeTranslation(goal * positionIndex);
}

void VART::TranslationAction::SetTranslation(double tx, double ty, double tz)
{
    goal.SetXYZW(tx, ty, tz, 0);
}

void VART::TranslationAction::SetTranslation(const Point4D& vector)
{
    goal = vector;
}
