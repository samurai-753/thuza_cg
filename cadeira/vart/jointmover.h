/// \file jointmover.h
/// \brief Header file for V-ART class "JointMover".
/// \version $Revision: 1.7 $

#ifndef VART_JOINTMOVER_H
#define VART_JOINTMOVER_H

#include "vart/joint.h"
#include <list>

namespace VART {
    class DofMover;
    class NoisyDofMover;
    class Interpolator;
    class DMModifier;

/// \class JointMover jointmover.h
/// \brief Controllers for joint movement.
///
/// Joint movers contain a set of DOF movers (see DofMover). They control how a joint
/// moves in a particular action (see Action).
    class JointMover {
        friend std::ostream& operator<<(std::ostream& output, const JointMover& mover);
        public:
        // PUBLIC METHODS
            JointMover();
            JointMover(Joint* newJointPtr, float newDuration, const Interpolator& interpolator);
            ~JointMover();

            /// \brief Moves the associated joint.
            void Move();

            /// \brief Sets the associated joint.
            void AttachToJoint(Joint* newJointPtr) { jointPtr = newJointPtr; }

            /// \brief Returns the associated joint.
            const VART::Joint* GetAttachedJoint() const { return jointPtr; }

            /// \brief Creates a DofMover.
            /// \param dof [in] DOF ID on which the new DOF mover will act
            /// \param iniTime [in] Initial movement time (in range [0..1])
            /// \param finTime [in] Final  movement time (in range [0..1])
            /// \param finPos [in] Final movement position  (in range [0..1])
            void AddDofMover(Joint::DofID dof, float iniTime, float finTime, float finPos);

            /// \brief Creates a noisy DofMover.
            /// \param dof [in] DOF ID on which the new DOF mover will act
            /// \param iniTime [in] Initial movement time (in range [0..1])
            /// \param finTime [in] Final  movement time (in range [0..1])
            /// \param finPos [in] Final movement position (in range [0..1])
            NoisyDofMover* AddNoisyDofMover(Joint::DofID dof, float iniTime, float finTime, float finPos);

            /// \brief Return the DofMover working on given Dof.
            /// \return NULL if no such DofMover.
            DofMover* GetDofMover(Joint::DofID dof);

            /// \brief Sets the total movement duration (in seconds).
            void SetDuration(float newDuration) { duration = newDuration; }

            /// \brief Return the total movement duration (in seconds).
            float GetDuration() { return duration; }

            /// \brief Changes the associated interpolator.
            void SetInterpolator(const Interpolator& interp) { interpolatorPtr = &interp; }

            /// \brief Returns a pointer to the associated interpolator.
            /// NULL is returned if there are no interpolator associated.
            const Interpolator* GetInterpolator() { return interpolatorPtr; }

            /// \brief Deactivates all DOF movers.
            void DeactivateDofMovers();

            /// \brief Changes starting/ending times of all dof movers.
            /// \param value [in] multiplying factor.
            ///
            /// Multiplies "value" for starting and ending time, of every dof mover. This could be
            /// usefull for sequencing dof movers inside an action.
            void MultiplyTimes(float value);

            /// \brief Returns a list with all different final times for every dof mover.
            void GetFinalTimes(std::list<float>* resultPtr);

            /// \brief Copy jointMover data to this jointMover, setting its jointPtr atribute to
            /// the targetJoint.
            void CopyFrom( VART::JointMover& jointMover, VART::Joint* targetJoint );

            /// \brief Modifies noisy dof movers.
            void ModifyDofMovers(DMModifier& modifier);

        // PUBLIC STATIC ATTRIBUTES
            //static bool cycle;
            static float goalTime;
            //static unsigned int priority;
        protected:
        // PROTECTED ATTRIBUTES
            /// \brief Associated joint
            Joint* jointPtr;
            float duration;
            float minimumDuration;
            const Interpolator* interpolatorPtr;
            std::list<DofMover*> dofMoverList;
    }; // end class declaration
} // end namespace

#endif
