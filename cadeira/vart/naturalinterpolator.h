/// \file naturalinterpolator.h
/// \brief Header file for V-ART class "NaturalInterpolator".
/// \version $Revision: 1.2 $
// The NaturalInterpolator has been replaced with NoisyDofMover

#ifndef VART_NATURALINTERPOLATOR_H
#define VART_NATURALINTERPOLATOR_H

#include "vart/sineinterpolator.h"
#include <list>

namespace VART {
/// \class NaturalInterpolator naturalinterpolator.h
/// \brief An interpolator for natural joint movement.
///
/// Natural Interpolators provide an ease-in ease-out interpolator
/// with noise, overshoot and offset.
    class NaturalInterpolator : public SineInterpolator {
        public:
        // PUBLIC METHODS
            /// Creates an unitialized natural interpolator
            NaturalInterpolator();
            NaturalInterpolator(float offsetLevel, float overshootLevel);
            virtual float GetValue(float linearIndex, float initialValue, float range) const;
            void SetOffset(float value) { offset = value; }
            float GetOffset() { return offset; }
            void SetOvershoot(float value) { overshoot = value; }
            float GetOvershoot() { return overshoot; }
            void SetNoise(float newNoiseAmplitude, float newNoiseWaveLenght);
        protected:
        // PROTECTED METHODS
        // PROTECTED ATTRIBUTES
            /// \brief how much to miss the target
            ///
            /// On a natural interpolator, values go from zero to 1+offset, creating positioning
            /// errors that may help creating natural movements.
            float offset;
            /// \brief how much to pass the target before correction
            ///
            /// On a natural interpolator, values may pass the desired target and then fall back
            /// to the target position. This may help creating an effect associated with inertia.
            float overshoot;
            /// \brief Overshoot time
            ///
            /// Peak time is the (normalized) time of maximum overshoot error.
            float peakTime;
            /// \brief Noise amplitude (noise level)
            float noiseAmplitude; // in DOF position range
            /// \brief Noise wave length
            ///
            /// Time space between randoms in DOF's normalized time.
            float noiseWaveLenght;
        // STATIC PROTECTED ATTRIBUTES
        private:
            std::list<float> noisePositionPeaks;
            std::list<float> noiseAmplitudePeaks;
            std::list<float>::iterator nextPeakItr;
            std::list<float>::iterator prevPeakItr;
    }; // end class declaration
} // end namespace
#endif
