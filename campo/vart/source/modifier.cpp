/// \file modifier.cpp
/// \brief Implementation file for V-ART class "Modifier".
/// \version $Revision: 1.1 $

#include "vart/modifier.h"
#include "vart/dof.h"

VART::Modifier::Modifier() {
    dofList = NULL;
    maxPonderatorList = NULL;
    minPonderatorList = NULL;
    numDofs = 0;
}

VART::Modifier::Modifier( VART::Dof **dofs, VART::Curve *mins, VART::Curve *maxs, int numD ) {
    dofList = dofs;
    maxPonderatorList = maxs;
    minPonderatorList = mins;
    numDofs = numD;
}

VART::Modifier::~Modifier() {
    if( dofList != NULL ) delete [] dofList;
    if( minPonderatorList != NULL ) delete [] minPonderatorList;
    if( maxPonderatorList != NULL ) delete [] maxPonderatorList;
}

VART::Curve* VART::Modifier::GetMinPonderatorList() {
    return minPonderatorList;
}

VART::Curve* VART::Modifier::GetMaxPonderatorList() {
    return maxPonderatorList;
}

float VART::Modifier::GetMin() {
    VART::Bezier* ptrMinPonderator;
    VART::Point4D ponderatorPoint;
    float aux;
    float min = -7; // FixMe: It seems this is just an odd initialization (-7 = 2 * -PI)
    for( int ind = 0; ind < numDofs; ind++ ) {
        ptrMinPonderator = dynamic_cast<VART::Bezier*>(&(minPonderatorList[ind]));
        ptrMinPonderator->GetPoint(dofList[ind]->GetCurrent(), &ponderatorPoint);
        aux = ponderatorPoint.GetY(); // why GetY?
        if( aux > min ) min = aux;
    }
    return min;
}

float VART::Modifier::GetMax() {
    VART::Bezier* ptrMaxPonderator;
    VART::Point4D ponderatorPoint;
    float aux;
    float max = 7;  // FixMe: It seems this is just an odd initialization (7 = 2 * PI)
    for( int ind = 0; ind < numDofs; ind++ ) {
        ptrMaxPonderator = dynamic_cast<VART::Bezier*>(&(maxPonderatorList[ind]));
        ptrMaxPonderator->GetPoint(dofList[ind]->GetCurrent(), &ponderatorPoint);
        aux = ponderatorPoint.GetY();
        if( aux < max ) max = aux;
    }
    return max;
}
