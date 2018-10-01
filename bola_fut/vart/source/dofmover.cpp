/// \file dofmover.cpp
/// \brief Implementation file for V-ART class "DofMover".
/// \version $Revision: 1.5 $

#include "vart/dofmover.h"
#include "vart/interpolator.h"
#include "vart/dof.h"

//#include <iostream>
using namespace std;

float VART::DofMover::goalTime;
float VART::DofMover::minimumDuration;
bool VART::DofMover::cycle;
const VART::Interpolator* VART::DofMover::interpolatorPtr = NULL;
unsigned int VART::DofMover::priority = 1;

VART::DofMover::DofMover() : active (false)
{
}

VART::DofMover::DofMover(const VART::DofMover& dofMover)
    : targetPosition( dofMover.targetPosition )
    , initialPosition( dofMover.initialPosition )
    , initialTime( dofMover.initialTime )
    , activationTime( dofMover.activationTime )
    , finalTime( dofMover.finalTime )
    , positionRange( dofMover.positionRange )
    , timeRange( dofMover.timeRange )
    , targetDofPtr( dofMover.targetDofPtr )
    , active( false )
{
}

// FixMe: Consider turning Initialize into the default constructor.
void VART::DofMover::Initialize(float iniTime, float finTime, float finPos)
// virtual method
{
    initialTime = iniTime;
    finalTime = finTime;
    targetPosition = finPos;
}

void VART::DofMover::Move()
{
    if ((goalTime > initialTime) && (goalTime < finalTime))
    {
        float interpolationIndex;
        float goalPosition;

        if (!active)
        { // Activate
            initialPosition = targetDofPtr->GetCurrent();
            active = true;
            positionRange = targetPosition - initialPosition;
            timeRange = finalTime - goalTime;
            // Validade against minimum duration to avoid movements that are too fast.
            if (timeRange < minimumDuration)
                timeRange = minimumDuration;
            activationTime = goalTime;
            // Move to current position, so that lower priority movers do not take effect
            targetDofPtr->MoveTo(initialPosition, priority);
        }
        else
        {
            // Really move
            interpolationIndex = (goalTime - activationTime)/timeRange;
            goalPosition = interpolatorPtr->GetValue(interpolationIndex,initialPosition,positionRange);
            targetDofPtr->MoveTo(goalPosition, priority);
        }
    }
    else
        // If outside its time range, the dof mover could be deactivating.
        active = false;
}

void VART::DofMover::GetFinalTime(list<float>* resultPtr)
{
    list<float>::iterator iter = resultPtr->begin();

    while ((iter != resultPtr->end()) && (*iter < finalTime))
        ++iter;
    if (!((iter != resultPtr->end()) && (*iter == finalTime)))
        resultPtr->insert(iter, finalTime);
}

ostream& VART::operator<<(ostream& output, const VART::DofMover& mover)
{
    output << "initialTime=\"" << mover.initialTime
           << "\" finalTime=\"" << mover.finalTime
           << "\" finalPosition=\"" << mover.targetPosition << "\"";
    return output;
}
