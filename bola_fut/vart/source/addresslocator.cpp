/// \file addresslocator.cpp
/// \brief Implementation file for V-ART class "AddressLocator".
/// \version $Revision: 1.1 $

#include "vart/addresslocator.h"
#include "vart/scenenode.h"

#include <iostream>
using namespace std;

VART::AddressLocator::AddressLocator(const SceneNode* targetPtr) : targetAddress(targetPtr)
{
}

//virtual
void VART::AddressLocator::OperateOn(const SceneNode* nodePtr)
{
    if (nodePtr == targetAddress)
        notFinished = false;
}
