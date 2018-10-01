/// \file mousecontrol.cpp
/// \brief Implementation file for V-ART class "MouseControl".
/// \version $Revision: 1.1 $

#include "vart/contrib/mousecontrol.h"

#include <iostream>
using namespace std;

VART::MouseControl::MouseControl()
{
    viewerPtr = NULL;
    clickHndPtr = NULL;
    dragHndPtr = NULL;
    motionHndPtr = NULL;
}

void VART::MouseControl::OnClick()
{
    //~ cout << "in VART::MouseControl::OnClick" << endl;
    if (clickHndPtr)
        clickHndPtr->OnClick();
}

void VART::MouseControl::SetModifierState(ModifierID mdValue, bool state)
{
    //~ cout << "in VART::MouseControl::SetModifierState" << endl;
    modifiers.set(mdValue,state);
    //~ cout << "out VART::MouseControl::SetModifierState" << endl;
}

void VART::MouseControl::SetDragHandler(DragHandler* handlerPtr)
{
    dragHndPtr = handlerPtr;
    handlerPtr->mouseCtrlPtr = this;
    handlerPtr->viewerPtr = viewerPtr;
}

void VART::MouseControl::SetClickHandler(ClickHandler* handlerPtr)
{
    clickHndPtr = handlerPtr;
    handlerPtr->mouseCtrlPtr = this;
    handlerPtr->viewerPtr = viewerPtr;
}

void VART::MouseControl::SetMotionHandler(MotionHandler* handlerPtr)
{
    motionHndPtr = handlerPtr;
    handlerPtr->mouseCtrlPtr = this;
    handlerPtr->viewerPtr = viewerPtr;
}

void VART::MouseControl::NewEvent(ButtonID btn, StateID newState)
{
    if (newState == DOWN)
        buttonsState.set(btn);
    else
        buttonsState.reset(btn);
    button = btn;
    state = newState;
}
