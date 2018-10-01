/// \file baseaction.cpp
/// \brief Implementation file for V-ART class "BaseAction".
/// \version $Revision: 1.1 $

#include "vart/baseaction.h"
#include "vart/time.h"
#include "vart/callback.h"

//#include <iostream>
using namespace std;

// Initialization of static attributes
list<VART::BaseAction*> VART::BaseAction::activeInstances;
list<VART::BaseAction::Initializer*> VART::BaseAction::initializers;


VART::BaseAction::BaseAction() : callbackPtr(NULL), cyclic(false), active(false),
                                 speedModifier(1.0), timeToLive(604800.0f)
{
}

// virtual
void VART::BaseAction::Deactivate()
{
    if (active)
    {
        // Update status
        active = false;

        // Remove this instance from list of active actions.
        list<VART::BaseAction*>::iterator iter = activeInstances.begin();
        while (iter != activeInstances.end())
        {
            if ((*iter) == this)
            {
                activeInstances.erase(iter);
                iter = activeInstances.end(); // force the way out of the while loop
            }
            else
                ++iter;
        }

        // Call its callback
        if (callbackPtr)
            callbackPtr->Activate();

        // Take "timeToLive" out of the way, in case the action is activated later and
        // no value is given to it by the programmer.
        timeToLive = 604800.0f;
    }
}

void VART::BaseAction::Deactivate(float seconds)
{
    timeToLive = seconds;
}

// virtual
void VART::BaseAction::Activate()
{
    if (!active)
    {
        active = true;
        lastPositionIndex = 0;
        // Store current time so that we had data do compute on next Move()
        lastUpdateTime.Set();

        // Add action to list of active instances
        activeInstances.push_back(this);
    }
}

// static
unsigned int VART::BaseAction::MoveAllActive()
{
    // Run all initializers
    list<VART::BaseAction::Initializer*>::iterator i = initializers.begin();
    while (i != initializers.end())
    {
        (*i)->Initialize();
        ++i;
    }

    // Move all actions
    list<VART::BaseAction*>::iterator iter = activeInstances.begin();
    list<VART::BaseAction*>::iterator tempIter;

    while (iter != activeInstances.end())
    {
        tempIter = iter;
        ++iter;
        // the action could remove itself from the list, so use a private iterator copy
        (*tempIter)->Move();
    }

    return activeInstances.size();
}

// virtual
void VART::BaseAction::ComputePositionIndex(bool* cycled)
{
    *cycled = false; // default value
    Time currentTime = Time::NOW();
    // Compute how much time has passed since last update
    float diffTime = (currentTime - lastUpdateTime).AsFloat();
    // Compute current position index
    positionIndex = ((diffTime * speedModifier) / duration) + lastPositionIndex;
    if (positionIndex > 1.0)
    {
        if (cyclic)
        {
            *cycled = true;
            positionIndex -= 1.0;
        }
        else
        {
            positionIndex = 1.0;
            Deactivate();
        }
    }
    // Enforce timeToLive
    timeToLive -= diffTime;
    if (timeToLive < 0)
        Deactivate();
    // Prepare for next iteration
    lastPositionIndex = positionIndex;
    lastUpdateTime = currentTime;
}
