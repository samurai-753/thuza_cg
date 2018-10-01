/// \file hermiteinterpolator.cpp
/// \brief Implementation file for V-ART class "HermiteInterpolator".
/// \version $Revision: $

#include "vart/hermiteinterpolator.h"
#include <iostream>

using namespace std;

VART::HermiteInterpolator::HermiteInterpolator(float finalTime, float finalPosition)
{
    positionVec.reserve(3);
    timeVec.reserve(3);
    timeVec.push_back(0.0f);
    positionVec.push_back(0.0f); // shall be replaced upon activation
    timeVec.push_back(finalTime);
    positionVec.push_back(finalPosition);
}

//float VART::HermiteInterpolator::GetValue(float linearIndex, float initialPos, float range) const
float VART::HermiteInterpolator::GetValue(float linearIndex) const
// virtual method
{
    // find next position
    int size = timeVec.size();
    int next = 1;
    bool notFinished = true;
    while ((next < size) && notFinished)
    {
        if (timeVec[next] >= linearIndex)
            notFinished = false;
        else
            ++next;
    }

    // find previous position
    int previous = next - 1;

    const float tension = 0.25;
    float prevPos = positionVec[previous];
    float nextPos = positionVec[next];
    float befPrevPos = positionVec[WarpIndex(previous - 1)];
    float aftNextPos = positionVec[WarpIndex(next + 1)];
    // find previous tangent
    float prevTg = tension * (nextPos - befPrevPos);
    // find next tangent
    float nextTg = tension * (prevPos - aftNextPos);
    // find linear index relative to control times
    float t = (linearIndex - timeVec[previous]) / (timeVec[next] - timeVec[previous]);
    // interpolate
    return Interpolate(prevPos, prevTg, nextPos, nextTg, t);
}

void VART::HermiteInterpolator::AddControlPosition(float pos, float time)
{
    int lastIndex = positionVec.size() - 1;
    // repeat last values at the end
    positionVec.push_back(positionVec[lastIndex]);
    timeVec.push_back(timeVec[lastIndex]);
    // add given values
    positionVec[lastIndex] = pos;
    timeVec[lastIndex] = time;
}

float VART::HermiteInterpolator::Interpolate(float p1, float tg1, float p2, float tg2, float t)
// protected static
{
    float t2 = t * t;
    float t3 = t * t2;
    float result = ((2 * t3) - (3 * t2) + 1) * p1
         + ((3 * t2) - (2 * t3)) * p2
         + (t3 - (2 * t2) + t) * tg1
         + (t3 - t2) * tg2;
    cout << p1 << " " << tg1 << " " << p2 << " " << tg2 << " " << t << " => " << result << "\n";
    return result;
}

int VART::HermiteInterpolator::WarpIndex(int i) const
// protected
{
    int size = positionVec.size();
    if (i < 0)
        return size + i;
    if (i >= size)
        return i - size;
    return i;
}
