/// \file snlocator.cpp
/// \brief Implementation file for V-ART class "SNLocator".
/// \version $Revision: 1.1 $

#include "vart/snlocator.h"

#include <iostream>
using namespace std;

VART::SNLocator::SNLocator() : notFinished(true), nodePtr(NULL)
{
}

void VART::SNLocator::AddNodeToPath(SceneNode* nodePtr)
{
    path.PushFront(nodePtr);
}

void VART::SNLocator::Reset()
{
    path.Clear();
    notFinished = true;
    nodePtr = NULL;
}
