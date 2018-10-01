/// \file picknamelocator.cpp
/// \brief Implementation file for V-ART class "PickNameLocator".
/// \version $Revision: 1.1 $

#include "vart/picknamelocator.h"
#include "vart/graphicobj.h"

VART::PickNameLocator::PickNameLocator(unsigned int name) : pickName(name), result(NULL)
{
}

void VART::PickNameLocator::OperateOn(const SceneNode* nodePtr)
{
    const VART::GraphicObj* castPtr = dynamic_cast<const VART::GraphicObj*>(nodePtr);
    if (castPtr && (castPtr->PickName() == pickName))
    {
        notFinished = false;
        result = castPtr;
    }
}

