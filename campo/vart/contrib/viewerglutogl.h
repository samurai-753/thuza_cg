/// \file viewerglutogl.h
/// \brief Header file for V-ART class "ViewerGlutOGL".
/// \version $Revision: 1.3 $

#ifndef VART_VIEWERGLUTOGL_H
#define VART_VIEWERGLUTOGL_H

#include "vart/scene.h"
#include "vart/contrib/mousecontrol.h"
#include <list>

namespace VART {
    const int KEY_F1 = GLUT_KEY_F1 + 255;
    const int KEY_F2 = GLUT_KEY_F2 + 255;
    const int KEY_F3 = GLUT_KEY_F3 + 255;
    const int KEY_F4 = GLUT_KEY_F4 + 255;
    const int KEY_F5 = GLUT_KEY_F5 + 255;
    const int KEY_F6 = GLUT_KEY_F6 + 255;
    const int KEY_F7 = GLUT_KEY_F7 + 255;
    const int KEY_F8 = GLUT_KEY_F8 + 255;
    const int KEY_F9 = GLUT_KEY_F9 + 255;
    const int KEY_F10 = GLUT_KEY_F10 + 255;
    const int KEY_F11 = GLUT_KEY_F11 + 255;
    const int KEY_F12 = GLUT_KEY_F12 + 255;
    const int KEY_LEFT = GLUT_KEY_LEFT + 255;
    const int KEY_UP = GLUT_KEY_UP + 255;
    const int KEY_RIGHT = GLUT_KEY_RIGHT + 255;
    const int KEY_DOWN = GLUT_KEY_DOWN + 255;
    const int KEY_PAGE_UP = GLUT_KEY_PAGE_UP + 255;
    const int KEY_PAGE_DOWN = GLUT_KEY_PAGE_DOWN + 255;
    const int KEY_HOME = GLUT_KEY_HOME + 255;
    const int KEY_END = GLUT_KEY_END + 255;
    const int KEY_INSERT = GLUT_KEY_INSERT + 255;

/// \class ViewerGlutOGL viewerglutogl.h
/// \brief V-ART Viewer that uses GLUT/OpenGL.
///
/// A ViewerGlutOGL is a window which can display a V-ART scene (VART::Scene) using GLUT
/// and OpenGL. The window also has basic behavior such as mouse navigation and
/// keyboard response. At most 6 instances of this class can be created.
    class ViewerGlutOGL {
        public:
        // PUBLIC NESTED CLASSES
            // These classes are nested because other types of viewers may have different
            // needs for their handlers.

            /// Keyboard handler
            class KbHandler
            {
                friend class ViewerGlutOGL;
                public:
                    virtual ~KbHandler() {};
                    /// \brief Called when a key is pressed.
                    ///
                    /// \deprecated Please use OnKeyDown.
                    virtual void HandleKey(unsigned char key) { }
                    /// \brief Called when a key is pressed.
                    ///
                    /// Implement this method on your keyboard handler. It will be called
                    /// when a key is pressed. Includes what GLUT calls "special keys". If
                    /// you keep a key pressed, several such events (key down) will be 
                    /// dispached, because GLUT handles it as many key ups and downs.
                    virtual void OnKeyDown(int key); // FixMe: make pure virtual
                    /// \brief Called when a special key is pressed.
                    ///
                    /// \deprecated Please use OnKeyDown.
                    virtual void HandleSpecialKey(int key) { }
                    /// \brief Called when a key is released.
                    ///
                    /// This method may be implemented on your keyboard handler. It will 
                    /// be called when a key is released. Includes what GLUT calls 
                    /// "special keys". If you keep a key pressed, several such events 
                    /// (key down) will be dispached, because GLUT handles it as many key 
                    /// ups and downs.
                    virtual void OnKeyUp(int key) { }
                protected:
                    /// Points to the viewer on which the handler was attached.
                    ViewerGlutOGL* viewerPtr;
            };
            /// Idle time handler
            class IdleHandler
            {
                friend class ViewerGlutOGL;
                public:
                    virtual ~IdleHandler() {};
                    /// Called when the application is idle.
                    virtual void OnIdle()=0;
                protected:
                    /// Points to the viewer on which the handler was attached.
                    ViewerGlutOGL* viewerPtr;
            };
            /// \brief Draw handler
            ///
            /// A draw handler allows the application programmer to add custom behavior associated
            /// with the window drawing. To use a draw handler, declare a derived class that
            /// implements the OnDraw method, then link it to the viewer using SetDrawHandler.
            /// The OnDraw method will be called after the window is drawn at every rendering cycle.
            class DrawHandler
            {
                friend class ViewerGlutOGL;
                public:
                    virtual ~DrawHandler() {};
                    /// \brief Called when the window is redrawn.
                    ///
                    /// When a draw handler is associated to a viewer, the OnDraw method will
                    /// be called at every rendering cycle, after the scene has been drawn.
                    virtual void OnDraw()=0;
                protected:
                    /// Points to the viewer on which the handler was attached.
                    ViewerGlutOGL* viewerPtr;
            };
        // PUBLIC METHODS
            /// \brief Creates an empty, unusable viewer positioned at (0,0).
            ///
            /// Before using the viewer, a scene (with at least one object, one camera
            /// and one light) should be attached to it.
            ViewerGlutOGL();

            /// \brief Creates an empty, unusable viewer positioned at (0,0) of given size.
            ///
            /// Before using the viewer, a scene (with at least one object, one camera
            /// and one light) should be attached to it.
            ViewerGlutOGL(int newWidth, int newHeight);

            ~ViewerGlutOGL();

            /// \brief Attaches a scene to the viewer.
            ///
            /// Attachment is done by storing a pointer to the scene. The viewer
            /// itself will not change the scene, but it should handle a changing scene.
            /// Lighting is enabled if there are lights attached to the scene.
            void SetScene(Scene& scene);

            /// \brief Changes the position of a viewer.
            /// \param x [in] New left coordinate.
            /// \param y [in] New top coordinate.
            void SetPosition(int x, int y);

            /// \brief Add a keyboard handler to the viewer.
            void SetKbHandler(KbHandler* ptrKbH);

            /// \brief Sets the idle time handler.
            void SetIdleHandler(IdleHandler* newIHPtr);

            /// \brief Sets the drawing handler.
            void SetDrawHandler(DrawHandler* newDHPtr);

            /// \brief Sets the camera used to view the scene.
            void SetCamera(Camera* camPtr);

            /// \brief Sets the mouse click handler.
            ///
            /// Define a "click handler" if you want your application to receive
            /// mouse button events (button down/up).
            void SetClickHandler(MouseControl::ClickHandler* newCHPtr);

            /// \brief Sets the mouse drag handler.
            void SetDragHandler(MouseControl::DragHandler* newDHPtr);

            /// \brief Sets the mouse motion handler.
            void SetMotionHandler(MouseControl::MotionHandler* newMHPtr);

            /// \brief Idle Management.
            ///
            /// Currently, handles mouse movment, updating the camera.
            void Idle();

            /// Sets the window size of the viewer.
            void SetSize(int newWidth, int newHeight);
            /// Returns current window width.
            int GetWidth() { return width; }
            /// Returns current window height.
            int GetHeight() { return height; }

            /// Returns the current camera or NULL if no camera exists.
            const Camera* GetCurrentCamera() const { return cameraPtr; }

            /// Hides the viewer.
            void Hide();

            /// Shows the viewer.
            void Show();

            /// Make the viewer become the current window.
            void TurnIntoCurrentWindow();

            /// Changes window title.
            void SetTitle(const std::string& newTitle);

            /// Iconifies (minimizes) the viewer window.
            void Iconify();

            /// Marks the viewer window for redisplay.
            void PostRedisplay();

            /// Tells the scene to use next camera in its list.
            void UseNextCamera() { ptScene->UseNextCamera(); }

        // PUBLIC STATIC METHODS

            /// \brief Enters main rendering loop.
            ///
            /// Draws (set them up) all lights in the scene and enters GLUT main loop.
            /// Never returns.
            static void MainLoop();

            /// \brief Initializes GLUT Library.
            ///
            /// Call this method before creation of the first viewer, passing the
            /// application arguments.
            static void Init(int* argcPtr, char* argv[]);

            /// \brief Marks all viewers for redisplay.
            static void RedisplayAll();

        // PUBLIC ATTRIBUTES
            /// Sets whether the aspect ratio of the viewer window should be passed
            /// along to its cameras.
            bool autoChangeCameraAspect;

            /// \brief Sets whether the viewer should redraw at every Idle call.
            ///
            /// This may usefull for simulations, where the scene is changing, to
            /// continuously redraw the scene.
            bool redrawOnIdle;

            /// How much to walk at each step (mouse movement).
            float walkStep;

            /// Sets whether the built-in navigation is enabled. This is set to "true" by default.
            bool autoNavigationEnabled;

            /// Sets whether the built-in keyboard responses are enabled.
            /// This is set to "true" by default.
            bool autoRespondKeys;

        protected:
            /// \brief Sets clear color and clears OpenGL buffers.
            void ClearOGLBuffers();

        private:
        // PRIVATE STATIC METHODS (GLUT CALLBACKS)
            // Drawing callbacks
            static void DrawCB0();
            static void DrawCB1();
            static void DrawCB2();
            static void DrawCB3();
            static void DrawCB4();
            static void DrawCB5();
            // Mouse click callbacks
            static void MouseCB0(int, int, int, int);
            static void MouseCB1(int, int, int, int);
            static void MouseCB2(int, int, int, int);
            static void MouseCB3(int, int, int, int);
            static void MouseCB4(int, int, int, int);
            static void MouseCB5(int, int, int, int);
            // Mouse drag callbacks
            static void DragMouseCB0(int x, int y);
            static void DragMouseCB1(int x, int y);
            static void DragMouseCB2(int x, int y);
            static void DragMouseCB3(int x, int y);
            static void DragMouseCB4(int x, int y);
            static void DragMouseCB5(int x, int y);
            // Special keys callbacks for key down
            static void SpecialKeyCB0(int key, int x, int y);
            static void SpecialKeyCB1(int key, int x, int y);
            static void SpecialKeyCB2(int key, int x, int y);
            static void SpecialKeyCB3(int key, int x, int y);
            static void SpecialKeyCB4(int key, int x, int y);
            static void SpecialKeyCB5(int key, int x, int y);
            // Special keys callbacks for key up
            static void SpecialKeyUpCB0(int key, int x, int y);
            static void SpecialKeyUpCB1(int key, int x, int y);
            static void SpecialKeyUpCB2(int key, int x, int y);
            static void SpecialKeyUpCB3(int key, int x, int y);
            static void SpecialKeyUpCB4(int key, int x, int y);
            static void SpecialKeyUpCB5(int key, int x, int y);
            // Keyboard callbacks
            static void KeyboardCB0(unsigned char key, int x, int y);
            static void KeyboardCB1(unsigned char key, int x, int y);
            static void KeyboardCB2(unsigned char key, int x, int y);
            static void KeyboardCB3(unsigned char key, int x, int y);
            static void KeyboardCB4(unsigned char key, int x, int y);
            static void KeyboardCB5(unsigned char key, int x, int y);
            // Callbacks for key ups:
            static void OnKeyUpCB0(unsigned char key, int x, int y);
            static void OnKeyUpCB1(unsigned char key, int x, int y);
            static void OnKeyUpCB2(unsigned char key, int x, int y);
            static void OnKeyUpCB3(unsigned char key, int x, int y);
            static void OnKeyUpCB4(unsigned char key, int x, int y);
            static void OnKeyUpCB5(unsigned char key, int x, int y);
            // Resize callbacks
            static void ResizeCB0(int newWidth, int newHeight);
            static void ResizeCB1(int newWidth, int newHeight);
            static void ResizeCB2(int newWidth, int newHeight);
            static void ResizeCB3(int newWidth, int newHeight);
            static void ResizeCB4(int newWidth, int newHeight);
            static void ResizeCB5(int newWidth, int newHeight);
            // Idle callback
            static void IdleMngr();
        // PRIVATE STATIC ATRIBUTES
            static int glutIDVec[6]; // GLUT window IDs
            static ViewerGlutOGL* instancePtrVec[6]; // pointers to class instances
        // PRIVATE METHODS
            void RegisterCallbacks();
            void CommonConstructor();
            void HandleKey(unsigned char key);
        // PRIVATE ATRIBUTES
            Scene* ptScene; // objects, lights and cameras
            int glutID; // GLUT window id
            KbHandler* kbHandlerPtr;
            DrawHandler* drawHandlerPtr; // pointer to draw handler
            IdleHandler* idleHndPtr; // pointer to idle handler
            MouseControl mouseController;
            int height; // window height in pixels
            int width;  // window width in pixels
            Camera* cameraPtr;
    }; // end class declaration
} // end namespace

#endif
