/// \file action.h
/// \brief Header file for V-ART class "Action".
/// \version $Revision: 1.10 $

#ifndef VART_ACTION_H
#define VART_ACTION_H

#include "vart/time.h"
#include "vart/scenenode.h"
#include <list>
#include <string>

namespace VART {
    class JointMover;
    class Joint;
    class Interpolator;
    class CallBack;
    class NoisyDofMover;
    class DMModifier;
/// \class Action action.h
/// \brief A coordinated movement of joints in an articulated body
/// \deprecated Please use JointAction.
///
/// Actions represent simple tasks such as walking, waving or scratching the head.
/// They may be thought as an integration of key poses, interpolation schemes and time
/// control.
/// When the time period for a non-cyclic action terminates, it is automatically deactivated.
/// They are implementated as a collection of joint movers (see JointMover).
    class Action {
        friend std::ostream& operator<<(std::ostream& output, const Action& action);
        public:
        // PUBLIC METHODS
            /// \brief Creates an unitialized action
            Action();

            ~Action();

            /// \brief Copy action data, except the active atribute (that remains unmodified).
            /// Used in Action::Copy() method.
            VART::Action& operator=(const VART::Action& action);

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
                                      float newDuration, const Interpolator& interpolator);

            /// \brief Used to add a copy of jointMover, aplied to jointPtr.
            JointMover* AddJointMover(Joint* jointPtr, JointMover* jointMover);

            /// \brief Sets the priority of an action.
            void SetPriority(unsigned int newPriority) { priority = newPriority; }

            /// \brief Sets the speed of an action.
            void SetSpeed(float newSpeed) { speed = newSpeed; }

            /// \brief Sets whether an action is cyclic.
            void SetCyclic(bool value) { cycle = value; }

            /// \brief Checks whether the action is cyclic.
            bool IsCyclic() { return cycle; }

            /// \brief Sets speed, priority and cycle attibutes.
            void Set(float newSpeed, unsigned int newPriority, bool cyclic);

            /// \brief Activate action.
            void Activate();

            /// \brief Deactivate action.
            void Deactivate();

            /// \brief Deactivate action after some time.
            void Deactivate(float seconds);

            /// \brief Returns true if the action is currently active.
            bool IsActive() const { return active; }

            /// \brief Returns a ordered list with all different final times for every dof mover.
            void GetFinalTimes(std::list<float>* resultPtr);

            /// \brief Returns a copy of an action, aplied to an targetNode sceneNode.
            ///
            /// Use it when copy actions and aply it to a copy of sceneNodes (using 
            /// SceneNode::RecursiveCopy() method).
            VART::Action* Copy(VART::SceneNode& targetNode);

            /// \brief Modifies dof movers.
            void ModifyDofMovers(DMModifier& mod);
        // STATIC PUBLIC METHODS
            /// \brief Moves all active actions.
            /// \return The number of active actions.
            static unsigned int MoveAllActive();
        // STATIC PUBLIC ATTRIBUTES
            /// \brief Fake animation time
            ///
            /// If greater then zero, the action does not measure real elapsed time from
            /// previous update. Instead it uses this value as elapsed time. Use it for
            /// creation of offline animations.
            static float frameFrequency;
        // PUBLIC ATTRIBUTES
            /// \brief Textual description
            std::string description;
            /// \brief A call-back to be activated once the action is finished/deactivated.
            CallBack* callbackPtr;
        protected:
        // PROTECTED METHODS
            /// \brief Animate joints.
            void Move();
            /// \brief Deactivates DOF movers in every joint mover.
            ///
            /// Deactivation of a DOF mover means it will have to recompute its motion at next move.
            /// See DofMover::Move().
            void DeactivateDofMovers();
        // PROTECTED ATTRIBUTES
            bool cycle;
            bool active;
            // FixMe: Speeds greater than 1 make a cyclic action stop between cycles!
            // FixMe: Speeds less than 1 make a cyclic action jump between cycles!
            float speed;
            float duration;
            float timeToLive; /// seconds to auto deactivation
            unsigned int priority;
            std::list<JointMover*> jointMoverList;
            Time initialTime;
        // STATIC PROTECTED ATTRIBUTES
            static std::list<Action*> activeInstances;
        private:
            // keep programmers from creating copies of actions
            Action(const Action& action) {}
            float timeDiff; // how many seconds have passed since activation
    }; // end class declaration
} // end namespace

#endif
