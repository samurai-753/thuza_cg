/// \file jointaction.cpp
/// \brief Implementation file for V-ART class "JointAction".
/// \version $Revision: 1.1 $

#include "vart/dofmover.h"
#include "vart/jointaction.h"
#include "vart/jointmover.h"
#include "vart/dof.h"
#include "vart/dmmodifier.h"

#include <iostream>
using namespace std;

// class JointAction::Initializer ==================================================================

VART::JointAction::Initializer::Initializer()
{
    // There can be only one!
    static unsigned char counter = 0;
    if (counter == 0)
    {
        // Add itself to the list of initializers
        BaseAction::initializers.push_back(this);
        counter = 1;
    }
    else
        cerr << "\aCreate just one instance of JointAction::Initializer!" << endl;
    //FixMe: - throw an exception instead of error message
}

void VART::JointAction::Initializer::Initialize()
{
    VART::Dof::ClearPriorities();
}

// class JointAction ===============================================================================

VART::JointAction::JointAction() : priority(1)
{
    // Create an initializer when the first joint action is constructed
    static Initializer jointActionInitializer;
}

// virtual (replaces BaseAction)
void VART::JointAction::Activate()
// Joint actions are inserted so that higher priority actions come first in the active instances list
{
    if (!active)
    {
        active = true;
        lastPositionIndex = 0;
        // Store current time so that we had data do compute on next Move()
        lastUpdateTime.Set();

        // Add action to list of active instances
        AddToActiveInstancesList();
    }
}

void VART::JointAction::AddToActiveInstancesList()
{
    list<BaseAction*>::iterator iter = activeInstances.begin();
    bool positionNotFound = true;
    while (positionNotFound && (iter != activeInstances.end()))
    {
        JointAction* jaPtr = dynamic_cast<JointAction*>(*iter);
        // if action is of type JointAction
        if (jaPtr)
        {
            if (jaPtr->priority > priority)
                ++iter;
            else
                positionNotFound = false;
        }
        else
            positionNotFound = false;
    }
    if (positionNotFound)
        // end of list reached
        activeInstances.push_back(this);
    else
        activeInstances.insert(iter, this);
    //~ iter = activeInstances.begin();
    //~ cout << "activeInstances: ";
    //~ for (; iter != activeInstances.end(); ++iter)
        //~ cout << (*iter)->description << " ";
    //~ cout << "\n";
}

void VART::JointAction::DeactivateDofMovers()
{
    list<VART::JointMover*>::iterator iter = jointMoverList.begin();
    for (; iter != jointMoverList.end(); ++iter)
        (*iter)->DeactivateDofMovers();
}

void VART::JointAction::GetFinalTimes(std::list<float>* resultPtr)
{
    resultPtr->clear();
    list<VART::JointMover*>::iterator iter = jointMoverList.begin();
    for (; iter != jointMoverList.end(); ++iter)
        (*iter)->GetFinalTimes(resultPtr);
}

void VART::JointAction::GetJoints(std::list<Joint*>* listPtr)
{
    listPtr->clear();
    list<VART::JointMover*>::iterator iter = jointMoverList.begin();
    for (; iter != jointMoverList.end(); ++iter)
        listPtr->push_back(const_cast<Joint*>((*iter)->GetAttachedJoint()));
}

VART::JointMover* VART::JointAction::AddJointMover(Joint* jointPtr,
                                                   float newDuration,
                                                   const Interpolator& interpolator)
{
    JointMover* jointMoverPtr = new JointMover(jointPtr, newDuration, interpolator);
    jointMoverList.push_back(jointMoverPtr);
    if (newDuration > duration)
        duration = newDuration;
    return jointMoverPtr;
}

// virtual
void VART::JointAction::Move()
{
    bool cycled;
    ComputePositionIndex(&cycled);
    if (cycled)
        DeactivateDofMovers(); // force them to restart from where they currently are

    // pass modified elapsed time to joint movers
    // joint movers see time as [0:action_duration] according to action activation and speed
    // FixMe: Perhaps the multiplication should be taken away. It was kept when porting the
    //        old Action class to this new JointAction.
    VART::JointMover::goalTime = positionIndex * duration;

    // pass priority to DOF movers
    VART::DofMover::priority = priority;
    // pass cycle flag to joint movers
    VART::DofMover::cycle = cyclic;
    
    // Tell joint movers to move their joints:
    list<VART::JointMover*>::iterator iter = jointMoverList.begin();
    for (; iter != jointMoverList.end(); ++iter)
        (*iter)->Move();
}

void VART::JointAction::ModifyDofMovers(DMModifier& modifier)
{
    // select joint movers that act on given joint
    list<JointMover*>::iterator jointIter = jointMoverList.begin();
    for(; jointIter != jointMoverList.end(); ++jointIter)
        (*jointIter)->ModifyDofMovers(modifier);
}

VART::DofMover* VART::JointAction::SearchDofMover(Joint* jointPtr, Joint::DofID dof)
{
    list<VART::JointMover*>::iterator iter = jointMoverList.begin();
    for (; iter != jointMoverList.end(); ++iter)
    {
        if (jointPtr == ((*iter)->GetAttachedJoint()))
            return (*iter)->GetDofMover(dof);
    }
    return NULL;
}

ostream& VART::operator<<(ostream& output, const VART::JointAction& action)
{
    list<JointMover*>::const_iterator iter = action.jointMoverList.begin();

    output << "<?xml version=\"1.0\"?>\n"
           << "<!DOCTYPE action SYSTEM \"vartAnimation.dtd\">\n"
           << "<action action_name=\"" << action.description << "\" speed=\""
           << action.speedModifier << "\" cycle=\"" << boolalpha << action.cyclic  << "\">\n";
    // for each joint mover
    while (iter != action.jointMoverList.end())
    {
        output << (**iter);
        ++iter;
    }
    output << "</action>\n";
    return output;
}

