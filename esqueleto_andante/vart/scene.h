/// \file scene.h
/// \brief Header file for V-ART class "Scene".
/// \version $Revision: 1.12 $

#ifndef VART_SCENE_H
#define VART_SCENE_H

#ifdef WIN32
    #define WINGDI
    #include <windows.h>
#endif
#ifdef VART_OGL
    #include <GL/gl.h>
#endif

#include "vart/light.h"
#include "vart/camera.h"
#include "vart/color.h" // for background color
#include "vart/boundingbox.h"
#include <string> //STL include
#include <list>   //STL include
#include <iostream> // for XmlPrintOn

namespace VART {
    class GraphicObj;
/// \class Scene scene.h
/// \brief A set of objects, lights and cameras.
///
/// A Scene is a complete viewable object with objects, lights, cameras and
/// background color. Instances of this class should be declared as static objects inside
/// the "main" function (see MemoryObj and ~Scene()).
/// The cameras contained in a scene are "reference" cameras in the sense that they describe
/// especial points of view for that scene. Viewers should have their own camera which changes
/// as the user navigates de scene, not changing the scene's reference cameras.
    class Scene {
        public:
            Scene();
            /// \brief Destructor.
            ///
            /// The scene desctructor is of key importance in V-ART applications because it
            /// deallocates memory for every scene node marked as "auto delete" (see MemoryObj).
            /// The application programmer should create scenes as late as possible to ensure
            /// its destructor will be called before static members of classes are destroyed. This
            /// usually means that a scene should be declared inside the "main" function. It should
            /// also be declared as an static object.
            virtual ~Scene();

            std::list<const Light*> GetLights();

            /// \brief Returns a list of cameras.
            /// \deprecated See UseNextCamera and UsePreviousCamera
            std::list<Camera*> GetCameras();

            /// \brief Adds a light to the list of lights.
            ///
            /// This version only copies the light pointer to the internal list. The
            /// application programmer should keep the memory space untouched while
            /// the scene uses it.
            void AddLight(Light* newLight);

            /// \brief Adds a light to the list of lights.
            ///
            /// This version creates an internal copy of the light, so that the application
            /// programmer may free the light's memory space.
            void AddLight(const Light& newLight);

            /// \brief Add a camera to the list of cameras.
            ///
            /// The first added camera is made the current camera. Other cameras are
            /// added to end of the list of cameras. Currently, it is not a good idea
            /// to share a camera between different scenes, because viewers change the
            /// aspect ratio of their scenes.
            void AddCamera(Camera* newCamera);

            /// \brief Add a object to the scene.
            ///
            /// The object's address is added to the scene's list. The object will be
            /// deallocated by the scene destructor if marked as "auto delete". If
            /// not, the application programmer is responsible for freeing memory at
            /// the end of scene's existence.
            void AddObject(SceneNode* newObjectPtr);

            /// \brief Removes an object from scene graph.
            ///
            /// Removes references to given scene node from list of objects. Not recursive. No
            /// memory deallocation is done.
            void Unreference(const SceneNode* sceneNodePtr);

            /// \brief Finds a light by its name.
            const Light* GetLight(const std::string& lightName);

            /// \brief Finds an camera by its description.
            const Camera* GetCamera(const std::string& cameraName);

            /// \brief Sets the current camera by its description.
            void SetCamera(const std::string& cameraName);

            std::list<SceneNode*> GetObjects();

            /// \brief Searches an object by its description.
            ///
            /// Only top-level objects are verified (no recursion).
            SceneNode* GetObject(const std::string& objectName) const;

            /// \brief Recursively searches an object by its description.
            /// \deprecated See DescriptionLocator.
            SceneNode* GetObjectRec(const std::string& objectName) const;

            /// \brief Returns the background color.
            ///
            /// Returns a constant reference to the background color.
            const Color& GetBackgroundColor();

            /// \brief Sets the background color.
            ///
            /// Changes the background color attribute. Currently, the viewer uses the background
            /// for actual frame buffer filling.
            void SetBackgroundColor(Color color);

            /// \brief Sets projection and draws graphics objects using OpenGL commands.
            ///
            /// This method is intended to be executed at every rendering cicle. It
            /// does not draw lights (from the "lights" list), because they need not be
            /// drawn at every rendering cicle.
            /// \return false if V-ART was not compiled with OpenGL support.
            virtual bool DrawOGL(Camera* cameraPtr = NULL) const;

            /// \brief Set lights using OpenGL commands.
            ///
            /// Lights may be drawn apart from other scene components because they need
            /// not be drawn at every rendering cicle.
            /// \return false if V-ART was not compiled with OpenGL support.
            bool DrawLightsOGL() const;

            /// Returns the current camera in the scene or NULL if no camera exists.
            Camera* GetCurrentCamera() const;

            /// Sets the current camera to the next camera in list of cameras.
            /// \return Returns a pointer to the new current camera.
            const Camera* UseNextCamera();

            /// Sets the current camera to the previous camera in the list of cameras.
            /// \return Returns a pointer to the new current camera.
            const Camera* UsePreviousCamera();

            /// Set the aspect ratio of camera by its description.
            void SetCamerasAspectRatio(const std::string& cameraDescription, float newAspectRatio);

            /// Set the aspect ratio of all cameras
            void SetAllCamerasAspectRatio(float newAspectRatio);

            /// Changes the view volume of all cameras
            void ChangeAllCamerasViewVolume(float horScale, float verScale);

            /// \brief Computes the axis aligned bounding box of all objects.
            bool ComputeBoundingBox();

            /// \brief Returns the scene bounding box.
            /// Attention: uses the ComputeBoundingBox method to refresh value of bounding box.
            const BoundingBox& GetBoundingBox() const { return bBox; }

            /// \brief Sets the bounding box visibility.
            void SetBBoxVisibility(bool value)    { bBox.visible = value; };

            /// \brief Sets the bounding box.
            void SetBoundingBox(double minX, double minY, double minZ,
                                double maxX, double maxY, double maxZ);

            /// \brief Toggles the bounding box visibility.
            void ToggleBBoxVisibility() { bBox.visible = !bBox.visible; }

            /// Returns the number of light in the scene.
            size_t GetNumLights() const { return lights.size(); }

            /// \brief Fits the whole scene to drawing region.
            ///
            /// Changes current camera's position and clipping planes so that the whole scene
            /// fits the viewer window.
            void MakeCameraViewAll();

            /// Sets the scene description.
            void SetDescription(const std::string& newDescr) { description = newDescr; }

            /// Returns the scene description.
            const std::string& GetDescription() { return description; }

            /// \brief Get all scenenodes of type 'joint' in the scene.
            /// \return a list of scene nodes of type joint.
            /// \deprecated Please use a Joint Collector (see VART::Collector).
            std::list<SceneNode*> GetAllSceneJoints();

            /// \brief Get all scenenodes of type 'type' in the scene.
            /// \return a list of scene nodes of type 'type'
            /// \deprecated Please use a Collector (see VART::Collector).
            std::list<SceneNode*> GetAllSceneTypeObject(SceneNode::TypeID type);

            /// \brief Picks objects from viewport coordinates
            void Pick(int x, int y, std::list<GraphicObj*>* resultListPtr);

            /// \brief Outputs XML representation of the scene.
            void XmlPrintOn(std::ostream& os) const;

        protected:
            // PROTECTED METHODS
            // PROTECTED ATTRIBUTES
            /// Separate lights (not treated as common objects)
            std::list<const Light*> lights;
            // FixMe: Viewers should manipulate a local copy of the current camera, allowing
            // pointers to be const.
            std::list<Camera*> cameras;
            std::list<SceneNode*> objects;
            Color background;
        private:
            // PRIVATE METHODS
            /// \brief Finds and returns the object of given pickName
            ///
            /// This method is auxiliary to Pick.
            GraphicObj* GetObject(unsigned int pickName);

            // PRIVATE ATTRIBUTES
            // FixMe: Viewers should manipulate a local copy of the current camera, a different
            // method to switch between cameras should be developed.
            std::list<Camera*>::const_iterator currentCamera;
            BoundingBox bBox;
            std::string description;
    }; // end class declaration
} // end namespace
#endif  // VART_SCENE_H
