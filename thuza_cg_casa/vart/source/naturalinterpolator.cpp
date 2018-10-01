/// \file naturalinterpolator.cpp
/// \brief Implementation file for V-ART class "NaturalInterpolator".
/// \version $Revision: 1.2 $
// The NaturalInterpolator has been replaced with NoisyDofMover

#include "vart/naturalinterpolator.h"
#include <cmath>

// FORMULA:
// (sin((x*overshoot+1.5)*pi)+1)/2*offset
// where overshot is in [1..2]
//                if "< 1", causes a kind of offset
//                1 causes no overshoot
//                2 causes 100% overshoot (goes back to initial position)
//       offset   is in [0..2]
//                0 causes no movement
//                0.7 causes 70% movement
//                1 causes no offset
//                1.2 cause +20% error
//                2 causes +100% error

using namespace std;

VART::NaturalInterpolator::NaturalInterpolator()
{
}

VART::NaturalInterpolator::NaturalInterpolator(float offsetLevel, float overshootLevel)
{
    offset = offsetLevel;
    overshoot = overshootLevel;
    peakTime = 0.9f;
}

void VART::NaturalInterpolator::SetNoise(float newNoiseAmplitude, float newNoiseWaveLenght)
{
    float position = newNoiseWaveLenght;
    float randomFloat; // a random number in the range [-0.5 .. 0.5]

    assert(noisePositionPeaks.empty());
    noisePositionPeaks.push_back(0.0f);
    noiseAmplitudePeaks.push_back(0.0f); // no noise at starting position
    while (position < 1.0f)
    {
        // create a noise peak
        randomFloat = (static_cast<float>(rand())/RAND_MAX) - 0.5;
        noisePositionPeaks.push_back(position);
        noiseAmplitudePeaks.push_back(randomFloat * newNoiseAmplitude);
        position += newNoiseWaveLenght;
    }
    // no noise at end
    noisePositionPeaks.push_back(1.0f);
    noiseAmplitudePeaks.push_back(0.0f);
    prevPeakItr = no
}

float VART::NaturalInterpolator::GetNoise()
{
    if (noisePositionPeaks.empty())
        return 0.0f;
    else
    {
        SineInterpolator::GetValue();
    }
}

float VART::NaturalInterpolator::GetValue(float linearIndex, float initialValue, float range) const
// virtual method
{
    float result;
    float newLinearIndex;
    float newRange;

    if (linearIndex < peakTime)
    { // interpolating to peak
        newLinearIndex = linearIndex / peakTime;
        newRange = range + offset + overshoot;
        result = SineInterpolator::GetValue(newLinearIndex, initialValue, newRange);
        if (hasNoise)
            result += GetNoise(linearIndex);
    }
    else
    { // interpolating from peak to target
        result = SineInterpolator::GetValue((linearIndex-peakTime)/(1-peakTime),
                                            initialPos+range+offset+overshoot,
                                            -overshoot);
    }
    return result;
}
