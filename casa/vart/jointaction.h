/// \file jointaction.h
/// \brief Header file for V-ART class "JointAction".
/// \version $Revision: 1.1 $

#ifndef VART_JOINTACTION_H
#define VART_JOINTACTION_H

#include "vart/baseaction.h"
#include "vart/joint.h"
#include <list>

namespace VART {
    class DofMover;
    class JointMover;
    class Joint;
    class Interpolator;
    class DMModifier;
/// \class JointAction jointaction.h
/// \brief A coordinated movement of joints in an articulated body.
///
/// Actions represent simple tasks such as walking, waving or scratching the head.
/// They may be thought as an integration of key poses, interpolation schemes and time
/// control.
/// When the time period for a non-cyclic action terminates, it is automatically deactivated.
/// They are implementated as a collection of joint movers (see JointMover).
    class JointAction : public BaseAction {
        friend std::ostream& operator<<(std::ostream& output, const JointAction& action);
        public:
            JointAction();
            virtual ~JointAction() { }

            /// \brief Adds a joint mover to the action.
            /// \return A pointer to a newly created joint mover.
            /// \param[in] jointPtr A pointer to the joint that will be controlled by the
            ///            new joint mover.
            /// \param[in] newDuration Duration (in seconds) of the joint movement.
            /// \param[in] interpolator An interpolator instance that will control motion.
            /// \attention Make sure that "interpolator" remains in its original memory 
            /// space during lifetime of the joint mover. The
            /// joint mover will keep just a pointer to the interpolator.
            ///
            /// Creates a new joint mover which is assigned to the action. The memory allocated
            /// by the joint mover will be deallocated by the action's destructor.
            JointMover* AddJointMover(Joint* jointPtr,
                                      float newDuration,
                                      const Interpolator& interpolator);

            /// \brief Used to add a copy of jointMover, aplied to jointPtr.
            JointMover* AddJointMover(Joint* jointPtr, JointMover* jointMover);

            /// \brief Sets the priority of an action.
            void SetPriority(unsigned int newPriority) { priority = newPriority; }

            /// \brief Returns a ordered list with all different final times for every dof mover.
            void GetFinalTimes(std::list<float>* resultPtr);

            /// \brief Modifies dof movers.
            void ModifyDofMovers(DMModifier& mod);

            /// \brief Returns a dof mover.
            ///
            /// Searches for a DofMover that works on given joint and dof.
            /// Returns NULL if not found.
            DofMover* SearchDofMover(Joint* jointPtr, Joint::DofID dof);

            /// \brief Returns a list of all target joints.
            void GetJoints(std::list<Joint*>* listPtr);

            /// \brief Activates a JointAction.
            virtual void Activate();
        protected:
        // PROTECTED NESTED CLASSES
            class Initializer : public BaseAction::Initializer
            {
                public:
                    Initializer();
                    virtual ~Initializer() {}
                    virtual void Initialize();
            };
        // PROTECTED METHODS
            virtual void Move();
            void DeactivateDofMovers();

            /// \brief Adds itself to list of active instances, in priority order.
            void AddToActiveInstancesList();
        // PROTECTED ATTRIBUTES
            std::list<JointMover*> jointMoverList;
            unsigned int priority;
        private:
    }; // end class declaration
} // end namespace

#endif
