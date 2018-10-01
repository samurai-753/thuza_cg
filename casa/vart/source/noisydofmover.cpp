/// \file noisydofmover.cpp
/// \brief Implementation file for V-ART class "NoisyDofMover".
/// \version $Revision: 1.4 $

#include "vart/noisydofmover.h"
#include "vart/sineinterpolator.h"
#include "vart/dof.h"
#include <cstdlib>

//#include <iostream>
using namespace std;

float VART::NoisyDofMover::defaultNoiseAmplitude = 0.0f;   // no noise
float VART::NoisyDofMover::defaultNoiseWaveLenght = 1.01f; // no subgoals
float VART::NoisyDofMover::defaultOvershoot = 0.0f;        // no overshoot
float VART::NoisyDofMover::defaultOffset = 0.0f;           // no offset
float VART::NoisyDofMover::defaultPeakTime = 0.9f;         // overshoot at 90% movement

VART::NoisyDofMover::NoisyDofMover()
// "active" is initialized at DofMover()
{
}

void VART::NoisyDofMover::Initialize(float iniTime, float finTime, float finPos)
// virtual method
{
    initialTime = iniTime;
    finalTime = finTime;
    targetPosition = finPos;
    noiseAmplitude = defaultNoiseAmplitude;
    noiseWaveLenght = defaultNoiseWaveLenght;
    overshoot = defaultOvershoot;
    offset = defaultOffset;
    peakTime = defaultPeakTime;
    hasOvershoot = (defaultOvershoot > 0.001);
    hasNoise = (defaultNoiseAmplitude > 0.001);
}

void VART::NoisyDofMover::SetNoise(float newNoiseAmplitude, float newNoiseWaveLenght)
{
    noiseAmplitude = newNoiseAmplitude;
    noiseWaveLenght = newNoiseWaveLenght;
    previousSubGoalTime = initialTime;
    previousSubGoalPosition = 0.0f;
    nextSubGoalTime = initialTime + noiseWaveLenght;
    nextSubGoalPosition = Random(); // probably not needed anymore, since activation computes noise.
    hasNoise = true;
}

//~ void VART::NoisyDofMover::SetPositionalError(float newOvershoot, float newOffset)
//~ {
    //~ overshoot = newOvershoot;
    //~ offset = newOffset;
    //~ hasOvershoot = true;
//~ }

//~ void VART::NoisyDofMover::SetPositionalError(float newOvershoot, float newOffset, float newPeakTime)
//~ {
    //~ overshoot = newOvershoot;
    //~ offset = newOffset;
    //~ peakTime = newPeakTime;
    //~ hasOvershoot = true;
//~ }

void VART::NoisyDofMover::SetOvershoot(float amplitude, float peak)
{
    overshoot = amplitude;
    peakTime = peak;
    hasOvershoot = (amplitude > 0.001);
}

void VART::NoisyDofMover::SetOffset(float newOffset)
{
    offset = newOffset;
}

void VART::NoisyDofMover::Move()
// virtual method
{
    if ((goalTime > initialTime) && (goalTime < finalTime))
    {
        float interpolationIndex;
        float goalPosition;

        if (active)
        {
            // Really move
            interpolationIndex = (goalTime - activationTime) / timeRange;
            //~ cout << interpolationIndex<<" "<<interpolationIndex-peakTime<<"\n";
            if (hasOvershoot)
            {
                if (interpolationIndex < peakTime)
                    // moving from initialPosition to overshoot position
                    goalPosition = interpolatorPtr->GetValue(interpolationIndex / peakTime,
                                                             initialPosition, overshootRange);
                else
                    // moving from overshoot position to targetPosition
                    goalPosition = interpolatorPtr->GetValue((interpolationIndex-peakTime)/(1-peakTime),
                                                             overshootPosition, finalRange);
            }
            else
                // no overshoot
                goalPosition = interpolatorPtr->GetValue(interpolationIndex,
                                                         initialPosition, positionRange);
            if (hasNoise)
            {
                float myNoise = Noise();
                //~ cout << goalTime<<" "<<myNoise<<"\n";
                goalPosition += myNoise;
            }
            //~ cout << goalTime<<" "<<goalPosition<<"\n";
            targetDofPtr->MoveTo(goalPosition, priority);
        }
        else
        { // Activate
            initialPosition = targetDofPtr->GetCurrent();
            nextSubGoalPosition = Random();
            active = true;
            positionRange = targetPosition + offset - initialPosition;
            timeRange = finalTime - goalTime;
            // Validade against minimum duration to avoid movements that are too fast.
            if (timeRange < minimumDuration)
                // When time range is too short, the movement will not reach its goal, but it will
                // be as fast as possible.
                timeRange = minimumDuration;
            activationTime = goalTime;
            overshootRange = positionRange * overshoot;
            finalRange = -overshootRange;
            overshootPosition = targetPosition + offset + overshootRange;
            overshootRange += positionRange;
            // Move to current position, so that lower priority movers do not take effect
            targetDofPtr->MoveTo(initialPosition, priority);
            //~ cout << "NoisyDofMover "<<this<<" activating from: "<<initialPosition<<" to: "<<targetPosition<<"\n";
            //~ cout << "overshootPosition: "<<overshootPosition<<endl;
        }
    }
    else
        // If outside its time range, the dof mover could be deactivating.
        active = false;
}

float VART::NoisyDofMover::Noise()
{
    static VART::SineInterpolator interpolator;
    float subPositionRange;
    float linearIndex;

    if (goalTime > nextSubGoalTime)
    {
        previousSubGoalTime = nextSubGoalTime;
        previousSubGoalPosition = nextSubGoalPosition;
        nextSubGoalTime += noiseWaveLenght;
        if (nextSubGoalTime > finalTime)
        {
            // into last part of path
            nextSubGoalTime = finalTime;
            if ((nextSubGoalTime - previousSubGoalTime) > 0.001)
                // protection against removing large amounts of noise in a short time
                nextSubGoalPosition = 0.0f;
        }
        else
        {
            // changing path parts
            nextSubGoalPosition = Random();
            //~ cout<<"noise: "<<nextSubGoalPosition<<" at "<<nextSubGoalTime<<"\n";
        }
    }
    else
    {
        if (goalTime < previousSubGoalTime)
        {
            // wraping around
            previousSubGoalTime = 0.0f;
            previousSubGoalPosition = 0.0f;
            nextSubGoalTime = noiseWaveLenght;
            nextSubGoalPosition = Random();
        }
        else
        {
            // same path part as before
        }
    }
    subPositionRange = nextSubGoalPosition - previousSubGoalPosition;
    linearIndex = (goalTime - previousSubGoalTime)/(nextSubGoalTime-previousSubGoalTime);
    return interpolator.GetValue(linearIndex, previousSubGoalPosition, subPositionRange);
}

float VART::NoisyDofMover::Random()
{
    return ((static_cast<float>(rand())/RAND_MAX) - 0.5f) * noiseAmplitude;
}

void VART::NoisyDofMover::SetDefaultNoiseAmplitude(float value)
{
    defaultNoiseAmplitude = value;
}

void VART::NoisyDofMover::SetDefaultNoiseWaveLenght(float value)
{
    defaultNoiseWaveLenght = value;
}

void VART::NoisyDofMover::SetDefaultOvershoot(float value)
{
    defaultOvershoot = value;
}

void VART::NoisyDofMover::SetDefaultOffset(float value)
{
    defaultOffset = value;
}

void VART::NoisyDofMover::SetDefaultPeakTime(float value)
{
    defaultPeakTime = value;
}
