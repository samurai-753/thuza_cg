/// \file action.cpp
/// \brief Implementation file for V-ART class "Action".
/// \version $Revision: 1.8 $

#include "vart/action.h"
#include "vart/jointmover.h"
#include "vart/dofmover.h"
#include "vart/time.h"
#include "vart/dof.h"
#include "vart/callback.h"
#include "vart/dmmodifier.h"

//#include <iostream>
using namespace std;

list<VART::Action*> VART::Action::activeInstances;
float VART::Action::frameFrequency = 0.0f;

VART::Action::Action() : callbackPtr(NULL), active(false), duration(0.0f),
                         timeToLive(604800.0f) // a week, in seconds
{
}

VART::Action& VART::Action::operator=(const VART::Action& action)
{
    cycle = action.cycle;
    speed = action.speed;
    duration = action.duration;
    timeToLive = action.timeToLive;
    priority = action.priority;
    initialTime = action.initialTime;
    description = action.description;
    jointMoverList = jointMoverList;
    return *this;
}

VART::Action::~Action()
{
    list<VART::JointMover*>::iterator iter;
    // destroy all joint movers
    for (iter = jointMoverList.begin(); iter != jointMoverList.end(); ++iter)
        delete *iter;
}

void VART::Action::Set(float newSpeed, unsigned int newPriority, bool cyclic)
{
    speed = newSpeed;
    priority = newPriority;
    cycle = cyclic;
}

VART::JointMover* VART::Action::AddJointMover(VART::Joint* jointPtr,
                                      float newDuration, const VART::Interpolator& interpolator)
{
    VART::JointMover* jointMoverPtr = new VART::JointMover(jointPtr, newDuration, interpolator);
    jointMoverList.push_back(jointMoverPtr);
    if (newDuration > duration)
        duration = newDuration;
    return jointMoverPtr;
}

VART::JointMover* VART::Action::AddJointMover(Joint* jointPtr, JointMover* jointMover)
{
    VART::JointMover* jointMoverPtr;
    jointMoverPtr = AddJointMover( jointPtr, jointMover->GetDuration(), *jointMover->GetInterpolator() );
    jointMoverPtr->CopyFrom(*jointMover, jointPtr);
    return jointMoverPtr;
}

void VART::Action::Move()
// Note: An action passes parameters to joint movers by setting some JointMover class attributes.
// This should be a bit faster than passing some parameter to every joint mover.
// Joint movers pass parameters to dof movers using the same scheme.
{
    static VART::Time currentTime;
    list<VART::JointMover*>::iterator iter;

    // compute timeDiff
    currentTime.Set();
    if (frameFrequency < 0.001)
        // real time
        timeDiff = (currentTime - initialTime).AsFloat() * speed;
    else
        // fake time
        timeDiff += (frameFrequency * speed);

    // Check and enforce timeToLive
    if (timeDiff > timeToLive)
    {
        Deactivate();
        timeToLive = 604800.0f; // a week, in seconds
        return;
    }

    // deactivate if finished
    if (timeDiff > duration)
    {
        if (cycle)
        {
            timeDiff -= duration;
            initialTime += duration;
            DeactivateDofMovers();
            timeToLive -= duration;
        }
        else
        {
            Deactivate();
            return;
        }
    }

    // pass modified elapsed time to joint movers
    // joint movers see time as [0:action_duration] according to action activation and speed
    VART::JointMover::goalTime = timeDiff;
    // pass priority to DOF movers
    VART::DofMover::priority = priority;
    // pass cycle flag to joint movers
    VART::DofMover::cycle = cycle;

    // Tell joint movers to move their joints:
    for (iter = jointMoverList.begin(); iter != jointMoverList.end(); ++iter)
    {
        (*iter)->Move();
    }
}

void VART::Action::Activate()
{
    if (!active)
    {
        initialTime.Set();

        // Add action to list of active instances (using priority order)
        list<Action*>::iterator iter = activeInstances.begin();
        bool notFinished = true;
        while (notFinished && (iter != activeInstances.end()))
        {
            // Actions with higher priority must come first in the list so that those
            // with lower priority will never have the chance to move DOFs. If they did,
            // transitions between DOF movers of the same action would get ugly.
            if ((*iter)->priority < priority)
            {
                activeInstances.insert(iter,this);
                notFinished = false;
            }
            ++iter;
        }
        if (notFinished)
            activeInstances.push_back(this);
        active = true;
        timeDiff = 0.0f;
        Move(); // ugly fix to prevent lower priority actions from changing target dofs
    }
}

void VART::Action::Deactivate()
{
    if (active)
    {
        list<VART::Action*>::iterator iter = activeInstances.begin();

        // Remove this instance from list and deactivate all dof movers so that they must be
        // recomputed if the action is activated again.
        active = false;
        while (iter != activeInstances.end())
        {
            //~ (*iter)->DeactivateDofMovers();
            if ((*iter) == this)
            {
                DeactivateDofMovers();
                //~ activeInstances.erase(iter++);
                activeInstances.erase(iter);
                iter = activeInstances.end();
            }
            else
                ++iter;
        }
        if (callbackPtr)
            callbackPtr->Activate();
    }
}

void VART::Action::Deactivate(float seconds)
{
    timeToLive = seconds;
}

void VART::Action::DeactivateDofMovers()
{
    list<VART::JointMover*>::iterator iter;
    for (iter = jointMoverList.begin(); iter != jointMoverList.end(); ++iter)
        (*iter)->DeactivateDofMovers();
}

unsigned int VART::Action::MoveAllActive()
// static method
{
    list<VART::Action*>::iterator iter = activeInstances.begin();
    list<VART::Action*>::iterator tempIter;

    // reset dof update priorities -- new draw cycle has begun
    VART::Dof::ClearPriorities();
    // move joints
    while (iter != activeInstances.end())
    {
        tempIter = iter;
        ++iter;
        // the action could remove itself from the list, so use a private iterator copy
        (*tempIter)->Move();
    }

    return activeInstances.size();
}

void VART::Action::GetFinalTimes(std::list<float>* resultPtr)
{
    list<VART::JointMover*>::iterator iter;

    resultPtr->clear();
    for (iter = jointMoverList.begin(); iter != jointMoverList.end(); ++iter)
        (*iter)->GetFinalTimes(resultPtr);
}

VART::Action* VART::Action::Copy(VART::SceneNode& targetNode)
{
    VART::Action* thisCopy;
    VART::Joint* joint;
    VART::JointMover* jointMover;
    std::list<JointMover*>::iterator jointMoverIter;

    thisCopy = new VART::Action();
    thisCopy->Action::operator=(*this);
    thisCopy->jointMoverList.clear();

    for( jointMoverIter = jointMoverList.begin(); jointMoverIter != jointMoverList.end(); jointMoverIter ++ )
    {
        joint = dynamic_cast<VART::Joint*>( targetNode.FindChildByName( (*jointMoverIter)->GetAttachedJoint()->GetDescription() ) );
        if( joint )
        {
            jointMover = thisCopy->AddJointMover( joint, *jointMoverIter );
        }
    }
    return thisCopy;
}

void VART::Action::ModifyDofMovers(DMModifier& modifier)
{
    // select joint movers that act on given joint
    list<JointMover*>::iterator jointIter = jointMoverList.begin();
    for(; jointIter != jointMoverList.end(); ++jointIter)
        (*jointIter)->ModifyDofMovers(modifier);
}

ostream& VART::operator<<(ostream& output, const VART::Action& action)
{
    list<JointMover*>::const_iterator iter = action.jointMoverList.begin();

    output << "Action '" << action.description << "'. Speed: " << action.speed << ". ";
    if (action.cycle)
        output << "Cyclic.\n";
    else
        output << "Not cyclic.\n";
    while (iter != action.jointMoverList.end())
    {
        output << (**iter);
        ++iter;
    }
    return output;
}
