/// \file mousecontrol.h
/// \brief Header file for V-ART class "MouseControl".
/// \version $Revision: 1.3 $

#ifndef VART_MOUSECONTROL_H
#define VART_MOUSECONTROL_H

#include <bitset>

namespace VART {
    class ViewerGlutOGL;
/// \class MouseControl mousecontrol.h
/// \brief Keeps track of mouse events and state.
///
/// The mouse control class provides a common ground for mouse related classes. It provides
/// a few nested classes that are used to handle mouse events.
    class MouseControl {
        public:
        // PUBLIC TYPES
            enum ButtonID { LEFT, MIDDLE, RIGHT, WHEEL_UP, WHEEL_DOWN };
            enum ModifierID { SHIFT, CONTROL, ALT };
            enum StateID { DOWN, UP };
            /// Click handler
            class ClickHandler
            {
                friend class MouseControl;
                public:
                    //~ ClickHandler() { }
                    /// Called upon activation of MouseHandler::OnClick
                    virtual void OnClick() = 0;
                    virtual ~ClickHandler() {};
                protected:
                    /// \brief Pointer to mouse controller.
                    ///
                    /// Use this pointer to get access to mouse information such as position
                    /// and last event.
                    MouseControl* mouseCtrlPtr;

                    /// \brief Pointer to owner viewer.
                    ///
                    /// Use this pointer to get access to viewer that owns the drag handler.
                    ViewerGlutOGL* viewerPtr;
            };
            /// Drag handler
            ///
            /// Handles mouse motion while some button is pressed.
            class DragHandler
            {
                friend class MouseControl;
                public:
                    virtual ~DragHandler() {};
                    virtual void OnDrag() = 0;
                protected:
                    /// \brief Pointer to mouse controller.
                    ///
                    /// Use this pointer to get access to mouse information such as position
                    /// and last event.
                    MouseControl* mouseCtrlPtr;
                    /// \brief Pointer to owner viewer.
                    ///
                    /// Use this pointer to get access to viewer that owns the drag handler.
                    ViewerGlutOGL* viewerPtr;
            };
            /// Motion handler
            ///
            /// Handles mouse motion while no button is pressed.
            class MotionHandler
            {
                friend class MouseControl;
                public:
                    virtual ~MotionHandler() {};
                    virtual void OnMotion() = 0;
                protected:
                    /// \brief Pointer to mouse controller.
                    ///
                    /// Use this pointer to get access to mouse information such as position
                    /// and last event.
                    MouseControl* mouseCtrlPtr;
                    /// \brief Pointer to owner viewer.
                    ///
                    /// Use this pointer to get access to viewer that owns the drag handler.
                    ViewerGlutOGL* viewerPtr;
            };
        // PUBLIC METHODS
            /// Creates an unitialized mouse controller.
            MouseControl();
            /// Checks if last event is relative to given button.
            bool LastClickButtonIs(ButtonID btValue) const { return button == btValue; }
            /// Returns button relative to last event.
            ButtonID GetLastClickButton() const { return button; }
            /// Sets button relative to last event.
            void SetLastClickButton(ButtonID btValue) { button = btValue; }
            /// Checks if given modifier was active on last event.
            bool ModifierIsActive(ModifierID mdValue) const { return modifiers[mdValue]; }
            /// Sets the modifier state on last event.
            void SetModifierState(ModifierID mdValue, bool state);
            /// Checks whether state is DOWN.
            bool LastClickIsDown() const { return state == DOWN; }
            /// Returns the button state of last click.
            StateID GetLastClickState() const { return state; }
            /// Set the button state on last click.
            void SetLastClickState(StateID newState) { state = newState; }
            /// Returns the x positon of last click.
            int GetClickXPosition() const { return clickX; }
            /// Sets the x postion of last click.
            void SetClickXPosition(int newValue) { clickX = newValue; }
            /// Returns the y positon of last click.
            int GetClickYPosition() const { return clickY; }
            /// Sets the y postion of last click.
            void SetClickYPosition(int newValue) { clickY = newValue; }
            /// Sets the position of last click.
            void SetClickPosition(int newX, int newY) {
                clickX = newX; clickY = newY; }
            /// Returns the current x postion.
            int GetCurrentX() const { return currentX; }
            /// Sets the current x position.
            void SetCurrentX(int newValue) { currentX = newValue; }
            /// Returns the current y postion.
            int GetCurrentY() const { return currentY; }
            /// Sets the current y position.
            void SetCurrentY(int newValue) { currentY = newValue; }
            /// Set the current position.
            void SetCurrentPosititon(int newX, int newY) {
                currentX = newX; currentY = newY; }
            /// Returns the x positon of last button release.
            int GetReleasedXPosition() const { return releasedX; }
            /// Sets the x postion of last button release.
            void SetReleasedXPosition(int newValue) { releasedX = newValue; }
            /// Returns the y positon of last button release.
            int GetReleasedYPosition() const { return releasedY; }
            /// Sets the y postion of last button release.
            void SetReleasedYPosition(int newValue) { releasedY = newValue; }
            /// Sets the position of last button release.
            void SetReleasedPosition(int newX, int newY) {
                releasedX = newX; releasedY = newY; }
            /// Activates click handler, if any.
            void OnClick();
            /// Activates drag (motion with buttons pressed) handler, if any.
            void OnDrag() { if (dragHndPtr) dragHndPtr->OnDrag(); }
            /// Activates motion (with no buttons pressed) handler, if any.
            void OnMotion() { if (motionHndPtr) motionHndPtr->OnMotion(); }
            /// Sets the click handler.
            void SetClickHandler(ClickHandler* handlerPtr);
            /// Sets the drag handler.
            void SetDragHandler(DragHandler* handlerPtr);
            /// Sets the motion handler.
            void SetMotionHandler(MotionHandler* handlerPtr);
            /// Set the owner viewer.
            void SetOwner(ViewerGlutOGL* ownerPtr) { viewerPtr = ownerPtr; }
            /// Registers new button event.
            void NewEvent(ButtonID btn, StateID newState);
            /// Checks whether any of LEFT, MIDDLE, RIGHT buttons are down.
            bool AnyButtonIsDown() const { return buttonsState.any(); }
            /// Returns true if given button is down.
            bool ButtonIsDown(ButtonID btn) const { return buttonsState[btn]; }
            /// Returns Y drag.
            ///
            /// Y drag is the current Y coordinate minus Y coordinate of last button press.
            int GetYDrag() const { return currentY - clickY; }
            /// Returns X drag.
            ///
            /// X drag is the current X coordinate minus X coordinate of last button press.
            int GetXDrag() const { return currentX - clickX; }
        protected:
            /// which button was last pressed or released
            ButtonID button;
            /// state of modifiers upon last button event
            std::bitset<3> modifiers;
            /// state (down if set) of LEFT, MIDDLE, RIGHT, WHEEL_UP and WHEEL_DOWN buttons
            std::bitset<5> buttonsState;
            /// state of button event (DOWN/UP)
            StateID state;
            /// Mouse X position for last button down event
            int clickX;
            /// Mouse Y position for last button down event
            int clickY;
            /// Current mouse X position
            int currentX;
            /// Current mouse Y position
            int currentY;
            /// Mouse X position for last button up event
            int releasedX;
            /// Mouse Y position for last button up event
            int releasedY;
            /// Pointer to the viewer that owns the mouse control
            ViewerGlutOGL* viewerPtr;
        private:
            ClickHandler* clickHndPtr;
            DragHandler* dragHndPtr;
            MotionHandler* motionHndPtr;
    }; // end class declaration
} // end namespace

#endif
