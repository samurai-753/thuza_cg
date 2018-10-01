/// \file baseaction.h
/// \brief Header file for V-ART class "BaseAction".
/// \version $Revision: 1.1 $

#ifndef VART_BASEACTION_H
#define VART_BASEACTION_H

#include "vart/time.h"
#include <list>
#include <string>

namespace VART {
    class CallBack;
/// \class BaseAction baseaction.h
/// \brief Animations. Transformations acting through time. Abstract class.
///
/// Actions can be seen as animations. BaseAction is an abstract class providing the interface
/// for all kinds of actions. In order to use actions, the programmer creates action objects
/// from derived classes such as TranslationAction; activates them and finally calls
/// BaseAction::MoveAllActive() before each rendering iteration.
    class BaseAction {
        public:
        // PUBLIC STATIC METHODS
            /// \brief Moves all active actions.
            /// \return The number of active actions.
            static unsigned int MoveAllActive();
        // PUBLIC METHODS
            /// \brief Creates an action with sane defaults.
            ///
            /// Speed is initialized to 1 (normal speed); action is not cyclic; time to live is
            /// huge.
            BaseAction();

            virtual ~BaseAction() { }

            void SetSpeed(float newSpeed) { speedModifier = newSpeed; }
            void SetDuration(float seconds) { duration = seconds; }
            void SetCyclic(bool value) { cyclic = value; }
            bool IsCyclic() { return cyclic; }
            bool IsActive() const { return active; }

            /// \brief Mark for auto deactivation in given time.
            void Deactivate(float seconds);

            /// \brief Deactivate now.
            virtual void Deactivate();

            /// \brief Activates the action.
            ///
            /// Once an action becomes active, calls to MoveAllActive() will make it update
            /// its target.
            virtual void Activate();
        // PUBLIC ATTRIBUTES
            std::string description;

            /// \brief Termination callback.
            CallBack* callbackPtr;

        protected:
            /// \brief Action Initializer.
            ///
            /// When deriving from BaseAction, it might be usefull to perform some kind of
            /// initialization before each rendering cicle. In that case, derive from
            /// BaseAction::Initializer and add an object to the initializers list. 
            /// Initializers are singletons, when deriving this class, make sure only one
            /// initializer could be created for each derived class. See
            /// an example at JointAction.
            class Initializer
            {
                public:
                    virtual void Initialize() = 0;
                    virtual ~Initializer() {}
            }; // BaseAction::Initializer
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
            /// \brief Make the action update its targets.
            ///
            /// Move is to be called from MoveAllActive.
            virtual void Move() = 0;

            /// \brief Computes current position index.
            /// \param cycled [out] indicates whether index has started over
            ///
            /// Before updating its target at Move(), an action should compute its current
            /// positionIndex using this method. This method will deactivate the action if
            /// it is finished.
            virtual void ComputePositionIndex(bool* cycled);
        // PROTECTED STATIC ATTRIBUTES
            /// \brief List of active actions.
            static std::list<BaseAction*> activeInstances;

            /// \brief List of action initializers.
            static std::list<Initializer*> initializers;
        // PROTECTED ATTRIBUTES
            /// \brief Whether the action restarts upon termination.
            bool cyclic;

            /// \brief Whether the action is currently active (changing its target).
            bool active;

            /// \brief Speed modifier.
            ///
            /// The speed modifier is a speed multiplicator that changes how fast an action
            /// develops. Like a playback speed. A speed modifier of 2 indicates the action
            /// is twice as fast as originally created.
            float speedModifier;

            /// \brief Duration in seconds.
            ///
            /// The duration is how much time (in seconds) the action takes from start to
            /// finish. Actual playback is modified the speed modifier.
            float duration;

            /// \brief Time (in seconds) to auto deactivate.
            float timeToLive;

            /// \brief Current position index.
            ///
            /// The position index is a number between 0 and 1 that reflects the current
            /// progress of the action.
            float positionIndex;

            /// \brief Position index of last update.
            ///
            /// The position index is a number between 0 and 1 that reflects the current
            /// progress of the action.
            float lastPositionIndex;

            /// \brief Time of last update.
            Time lastUpdateTime;
        private:
    }; // end class declaration
} // end namespace

#endif
