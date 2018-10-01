/// \file noisydofmover.h
/// \brief Header file for V-ART class "NoisyDofMover".
/// \version $Revision: 1.3 $

#ifndef VART_NOISYDOFMOVER_H
#define VART_NOISYDOFMOVER_H

#include "vart/dofmover.h"

namespace VART {
/// \class NoisyDofMover noisydofmover.h
/// \brief Noisy Controller for DOF movement.
///
/// A DOF mover that adds noise to movement (See DofMover).
/// NoisyDofMover is not meant to be used by the application programmer. Use
/// the interface methods in Action and JointMover.
    class NoisyDofMover : public DofMover {
        friend class JointMover;
        friend class Action;
        public:
        // PUBLIC STATIC METHODS
            static void SetDefaultNoiseAmplitude(float value);
            static void SetDefaultNoiseWaveLenght(float value);
            static void SetDefaultOvershoot(float value);
            static void SetDefaultOffset(float value);
            static void SetDefaultPeakTime(float value);
        // PUBLIC METHODS
            //~ /// \brief Sets main positional error attributes (overshoot and offset)
            //~ void SetPositionalError(float newOvershoot, float newOffset);
            //~ /// \brief Set all positional error attributes
            //~ void SetPositionalError(float newOvershoot, float newOffset, float newPeakTime);

            /// \brief Sets the noise attributes
            void SetNoise(float newNoiseAmplitude, float newNoiseWaveLenght);

            /// \brief Sets the overshoot attributes
            void SetOvershoot(float amplitude, float peak);

            /// \brief Set the offset attribute
            void SetOffset(float newOffset);
        protected:
        // PROTECTED METHODS
            /// \brief Creates an unitialized noisy DOF mover
            ///
            /// After creating a noisy dof mover with the default constructor, call Initialize() to
            /// make sure all attributes will be initialized.
            NoisyDofMover();
            /// \brief Initializes a noisy DOF mover
            ///
            /// This method takes only values for attributes in the base class because all noise
            /// and error attributes are initialized to their default values. See also SetNoise()
            /// and SetPositionalError().
            virtual void Initialize(float iniTime, float finTime, float finPos);
            /// \brief Changes target DOF.
            virtual void Move();
            /// \brief Generates and returns corehent noise
            float Noise();
            /// \brief Generates and returns positional error
            ///
            /// Computes overshoot and offset for producing positional error.
            float PositionalError();
            /// \brief Generates and returns a pseudo-random number within internal noise amplitude
            float Random();
        // PROTECTED STATIC ATTRIBUTES
            static float defaultNoiseAmplitude;
            static float defaultNoiseWaveLenght;
            static float defaultOvershoot;
            static float defaultOffset;
            static float defaultPeakTime;
        // PROTECTED ATTRIBUTES
            /// \brief Noise amplitude (noise level)
            float noiseAmplitude; // in DOF position range

            /// \brief Noise wave length
            ///
            /// Time space between randoms in DOF's normalized time.
            float noiseWaveLenght;

            /// \brief Time for next noise peek
            float nextSubGoalTime;

            /// \brief Time of previous noise peek
            float previousSubGoalTime;

            /// \brief Amplitude of next noise peek
            float nextSubGoalPosition;

            /// \brief Amplitude of previous noise peek
            float previousSubGoalPosition;

            /// \brief how much to miss the DOF target position
            ///
            /// The offset is a controlled error that makes the position of a DOF go from zero
            /// to target+offset, aiding the creation of a sense of weight for natural movements.
            float offset;

            /// \brief how much to pass the DOF target position before correction
            ///
            /// The overshoot is a controlled error that makes the position of a DOF pass the
            /// desired target position and then fall back to the desired value. This may help
            /// creating an effect associated with inertia.
            float overshoot;

            /// \brief Overshoot time
            ///
            /// Peak time is the (normalized) time of maximum overshoot error.
            float peakTime;
            /// Movement range from intial position to overshoot position
            float overshootRange;
            /// Movement range from overshoot position to goal position
            float finalRange;
            float overshootPosition;
            bool hasOvershoot;
            bool hasNoise;
    }; // end class declaration
} // end namespace

#endif
