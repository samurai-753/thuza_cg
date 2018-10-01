/// \file dofmover.h
/// \brief Header file for V-ART class "DofMover".
/// \version $Revision: 1.7 $

#ifndef VART_DOFMOVER_H
#define VART_DOFMOVER_H

#include <iostream>
#include <list>

namespace VART {
    class Dof;
    class Interpolator;
    class JointMover;

/// \class DofMover dofmover.h
/// \brief Controller for DOF movement.
///
/// DOF movers are entities that control movement of a DOF (see Dof).
/// They should "live" inside a joint mover (see JointMover) which takes
/// care of their memory management.
/// DofMover is not meant to be used by the application programmer. Use
/// the interface methods in Action and JointMover.
    class DofMover {
        friend class JointMover;
        friend class Action;
        friend class JointAction;
        friend std::ostream& operator<<(std::ostream& output, const DofMover& mover);
        public:
            /// \brief Returns a pointer to the target DOF.
            const Dof* GetTarget() { return targetDofPtr; }
        protected:
        // PROTECTED METHODS
            /// \brief Creates an unitialized DOF mover.
            DofMover();
            /// \brief Creates a copy of an DofMover. All data are copied, except
            /// the 'active' atribute, that is set to false.
            DofMover(const VART::DofMover& dofMover);
            virtual ~DofMover() {}
            virtual void Initialize(float iniTime, float finTime, float finPos);
            /// \brief Sets the target DOF.
            void SetDof(Dof* dofPtr) { targetDofPtr = dofPtr; }
            /// \brief Changes target DOF.
            virtual void Move();
            /// \brief Adds the final time to the list.
            ///
            /// Final time is added to the list, in order, if not already there.
            void GetFinalTime(std::list<float>* resultPtr);
        // PROTECTED ATTRIBUTES
            /// \brief Target position.
            float targetPosition;
            /// \brief Position of target DOF upon activation.
            float initialPosition;
            /// \brief Time of predicted activation (normalized).
            float initialTime;
            /// \brief Time of actual activation (normalized).
            float activationTime;
            /// \brief Normalized deactivation time.
            float finalTime;
            /// \brief How far from target at activation time.
            float positionRange;
            /// \brief How much time to reach target position.
            float timeRange;
            /// \brief Target DOF.
            Dof* targetDofPtr;
            /// \brief Tells whether a DOF mover is active.
            ///
            /// An active DOF mover has already checked its initial position and therefore knows
            /// the speed needed to get to target position. An inactive DOF mover must do these
            /// computations before moving its target DOF.
            bool active;
        //  STATIC PROTECTED ATTRIBUTES
            /// \brief Time of next snapshot, normalized to joint movement's duration.
            static float goalTime;
            /// \brief Indicates whether the current action is cyclic.
            static bool cycle;
            /// \brief Position interpolator.
            static const Interpolator* interpolatorPtr;
            /// \brief Priority of active action.
            static unsigned int priority;
            /// \brief Minimum duration when computing motion paths.
            static float minimumDuration;
    }; // end class declaration
} // end namespace

#endif
