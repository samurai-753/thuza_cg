/// \file jointmover.cpp
/// \brief Implementation file for V-ART class "JointMover".
/// \version $Revision: 1.7 $

#include "vart/jointmover.h"
#include "vart/dof.h"
#include "vart/dofmover.h"
#include "vart/noisydofmover.h"
#include "vart/time.h"
#include "vart/linearinterpolator.h"
#include "vart/dmmodifier.h"
#include <algorithm>
#include <cassert>

//#include <iostream>
using namespace std;

float VART::JointMover::goalTime;

VART::JointMover::JointMover()
{
    jointPtr = NULL;
    interpolatorPtr = NULL;
}

VART::JointMover::JointMover(VART::Joint* newJointPtr,
                           float newDuration, const VART::Interpolator& interpolator)
{
    jointPtr = newJointPtr;
    duration = newDuration;
    interpolatorPtr = &interpolator;
    minimumDuration = newDuration * 0.5;
}

VART::JointMover::~JointMover()
{
    list<VART::DofMover*>::iterator iter;
    // remove all DOF movers from memory
    for (iter = dofMoverList.begin(); iter != dofMoverList.end(); ++iter)
        delete *iter;
}

void VART::JointMover::Move()
// Note: A joint mover passes parameters to dof movers by setting some VART::DofMover class attributes.
// This implementation should make animations a bit faster.
{
    list<VART::DofMover*>::iterator iter;

    // pass normalized elapsed time to dof movers
    VART::DofMover::goalTime = goalTime/duration;
    // pass interpolator to dof movers
    VART::DofMover::interpolatorPtr = interpolatorPtr;
    VART::DofMover::minimumDuration = minimumDuration;
    // activate dof movers
    for (iter = dofMoverList.begin(); iter != dofMoverList.end(); ++iter)
        (*iter)->Move();
}

void VART::JointMover::AddDofMover(VART::Joint::DofID dof, float iniTime, float finTime, float finPos)
{
    float minimumDurationCandidate = (finTime - iniTime) * 0.5;
    VART::DofMover* moverPtr = new VART::DofMover;
    moverPtr->Initialize(iniTime, finTime, finPos);
    moverPtr->SetDof(const_cast<VART::Dof*>(&(jointPtr->GetDof(dof))));
    assert(moverPtr->targetDofPtr != NULL);
    dofMoverList.push_back(moverPtr);
    if (minimumDurationCandidate < minimumDuration)
        minimumDuration = minimumDurationCandidate;
    //~ cout << "joint mover "<<this<<" added dof mover "<<moverPtr<<endl;
}

VART::NoisyDofMover* VART::JointMover::AddNoisyDofMover(VART::Joint::DofID dof,
                                                        float iniTime, float finTime, float finPos)
{
    float minimumDurationCandidate = (finTime - iniTime) * 0.5;
    VART::NoisyDofMover* moverPtr = new VART::NoisyDofMover;

    moverPtr->Initialize(iniTime, finTime, finPos);
    moverPtr->SetDof(const_cast<VART::Dof*>(&(jointPtr->GetDof(dof))));
    assert(moverPtr->targetDofPtr != NULL);
    dofMoverList.push_back(moverPtr);
    if (minimumDurationCandidate < minimumDuration)
        minimumDuration = minimumDurationCandidate;
    return moverPtr;
}

void VART::JointMover::DeactivateDofMovers()
{
    list<VART::DofMover*>::iterator iter;
    for (iter = dofMoverList.begin(); iter != dofMoverList.end(); ++iter)
        (*iter)->active = false;
}

void VART::JointMover::MultiplyTimes(float value)
{
    list<VART::DofMover*>::iterator iter;
    //~ cout << "\nJointMover " << this << " at MultiplyTimes(" << value << ")\n";
    for (iter = dofMoverList.begin(); iter != dofMoverList.end(); ++iter)
    {
        //~ cout << "Dof mover " << this << " intialTime: " << (*iter)->initialTime << " to " << (*iter)->initialTime * value << "\n";
        (*iter)->initialTime *= value;
        //~ cout << "Dof mover " << this << " finalTime: " << (*iter)->finalTime << " to " << (*iter)->finalTime * value << "\n";
        (*iter)->finalTime   *= value;
    }
}

VART::DofMover* VART::JointMover::GetDofMover(Joint::DofID dofID)
{
    if (jointPtr->HasDof(dofID))
    {
        const Dof* dofPtr = &(jointPtr->GetDof(dofID));
        list<DofMover*>::iterator iter = dofMoverList.begin();
        for (; iter != dofMoverList.end(); ++iter)
        {
            if ((*iter)->GetTarget() == dofPtr)
                return *iter;
        }
    }
    return NULL;
}

void VART::JointMover::GetFinalTimes(list<float>* resultPtr)
{
    list<VART::DofMover*>::iterator iter;

    for (iter = dofMoverList.begin(); iter != dofMoverList.end(); ++iter)
        (*iter)->GetFinalTime(resultPtr);
}

void VART::JointMover::CopyFrom( VART::JointMover& jointMover, VART::Joint* targetJoint )
{
    VART::NoisyDofMover* noisyDofMover;
    VART::DofMover * dofMover;
    VART::Joint::DofID dofId;
    std::list<VART::DofMover*>::iterator iter;

    jointPtr = targetJoint;
    duration = jointMover.duration;
    minimumDuration = jointMover.minimumDuration;
    interpolatorPtr = jointMover.interpolatorPtr;
    for( iter = jointMover.dofMoverList.begin(); iter != jointMover.dofMoverList.end(); iter++ )
    {
        dofId = jointMover.jointPtr->GetDofID( (*iter)->GetTarget() );
        noisyDofMover = dynamic_cast<VART::NoisyDofMover*>(*iter);
        if( noisyDofMover )
            AddNoisyDofMover( dofId, noisyDofMover->initialTime, noisyDofMover->finalTime, noisyDofMover->targetPosition );
        else
        {
            dofMover = *iter;
            AddDofMover( dofId, dofMover->initialTime, dofMover->finalTime, dofMover->targetPosition );
        }
    }
}

void VART::JointMover::ModifyDofMovers(DMModifier& modifier)
{
    list<DofMover*>::iterator dofIter = dofMoverList.begin();
    for (; dofIter != dofMoverList.end(); ++dofIter)
    {
        NoisyDofMover* dofMoverPtr = dynamic_cast<NoisyDofMover*>(*dofIter);
        if (dofMoverPtr)
            modifier.Modify(dofMoverPtr);
    }
}

ostream& VART::operator<<(ostream& output, const VART::JointMover& mover)
{
    list<VART::DofMover*>::const_iterator iter = mover.dofMoverList.begin();

    output << "  <joint_movement joint_name=\""<< mover.jointPtr->GetDescription() << "\" duration=\""
           << mover.duration << "\">\n    <interpolation type=\"";
    switch (mover.interpolatorPtr->GetID())
    {
        case Interpolator::LINEAR:
            output << "linear\"/>\n";
            break;
        case Interpolator::EASE_IN_EASE_OUT:
            output << "ease-in_ease-out\"/>\n";
            break;
        default:
            output << "unknown\"/>\n";
            break;
    }
    // for each dof mover
    while (iter != mover.dofMoverList.end()) {
        output << "    <dof_movement dofID=\""
               << mover.jointPtr->GetDofID((*iter)->GetTarget()) << "\" "
               << **iter << "/>\n";
        ++iter;
    }
    output << "  </joint_movement>\n";
    return output;
}
