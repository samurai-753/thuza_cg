/// \file offsetmodifier.cpp
/// \brief Implementation file for V-ART class "OffsetModifier".
/// \version $Revision: 1.1 $

#include "vart/offsetmodifier.h"
#include "vart/dof.h"
#include "vart/noisydofmover.h"

#include <iostream>
#include <fstream>
using namespace std;

VART::OffsetModifier::OffsetModifier() : weight(0.0f)
{
}

void VART::OffsetModifier::Modify(NoisyDofMover* moverPtr)
{
    string jointName = moverPtr->GetTarget()->GetOwnerJoint()->GetDescription();
    Joint2DofIDs::iterator dofIDMapIter = offsetMap.find(jointName);
    if (dofIDMapIter != offsetMap.end())
    {
        const Dof* dofPtr = moverPtr->GetTarget();
        Joint::DofID dof = dofPtr->GetOwnerJoint()->GetDofID(dofPtr);
        DofID2Offset::iterator offsetIter = dofIDMapIter->second.find(dof);
        if (offsetIter != dofIDMapIter->second.end())
            moverPtr->SetOffset(weight * offsetIter->second);
    }
}

void VART::OffsetModifier::LoadFromFile(const std::string& fileName)
{
    ifstream inputFile(fileName.c_str());
    string jointName;
    Joint::DofID dof;
    float offset;
    while (inputFile >> jointName >> dof >> offset)
        offsetMap[jointName][dof] = offset;
}
