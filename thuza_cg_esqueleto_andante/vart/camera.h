/// \file camera.h
/// \brief Header file for V-ART class "Camera".
/// \version $Revision: 1.8 $

#ifndef VART_CAMERA_H
#define VART_CAMERA_H

#include "vart/point4d.h"
#include <string>    //STL include

namespace VART {
/// \class Camera camera.h
/// \brief Virtual camera.
///
/// FixMe: A detailed class description is needed here.
    class Camera {
        friend std::ostream& operator<<(std::ostream& output, const Camera& cam);
        public:
            enum ProjectionType { ORTHOGRAPHIC, PERSPECTIVE };

            Camera();

            /// Creates a camera.
            /// \param position [in] Location of the new camera
            /// \param focus [in] Position at which the new camera is looking at
            /// \param upVec [in] Up vector specifing camera's orientation
            ///
            /// Creates a camera with given parameters. The "up vector" is modified in order
            /// to be orthogonal to the camera line of view and is normalized.
            Camera(const Point4D& position, const Point4D& focus, const Point4D& upVec);

            Camera(const Camera& cam);

            Camera& operator=(const Camera& cam);

            /// Gets the description associated with the camera.
            const std::string& GetDescription() const { return description; }

            /// Sets the description associated with the camera.
            void SetDescription(const std::string& descriptionValue);

            /// Gets the projection type (PERSPECTIVE/ORTHOGRAPHIC).
            ProjectionType GetProjectionType() const { return projectionType; }

            /// Sets the projection type (PERSPECTIVE/ORTHOGRAPHIC).
            void SetProjectionType(ProjectionType newValue) { projectionType = newValue; }

            /// Gets the camera's width/height ratio.
            float GetAspectRatio() const { return aspectRatio; }

            /// Sets the camera's width/height ratio.
            void SetAspectRatio(float newAR) { aspectRatio = newAR; }

            /// Returns the distance to the near plane.
            float GetNearPlaneDistance() const { return nearPlaneDistance; }

            /// Sets the distance to the near plane.
            void SetNearPlaneDistance(float newValue) { nearPlaneDistance = newValue; }

            /// Returns the distance to the far plane.
            float GetFarPlaneDistance() const { return farPlaneDistance; }

            /// Sets the distance to the far plane distance.
            void SetFarPlaneDistance(float newValue) { farPlaneDistance = newValue; }

            /// Returns the vertical field of view (in degrees).
            float GetFovY() const;

            /// Sets the vertical field of view (in degrees).
            void SetFovY(float f);

            /// Returns the camera location (its position).
            Point4D GetLocation() const;

            /// Sets the camera location.
            void SetLocation(const Point4D& locationValue);

            /// Returns the camera target (where it is looking at).
            Point4D GetTarget() const;

            /// Sets the camera target (where it is looking at).
            void SetTarget(const Point4D& targetValue);

            /// Returns the camera up vector.
            Point4D GetUp() const;

            /// Computes the vector pointing left.
            void LeftVector(Point4D* resultPtr) const;

            /// Computes the vector pointing ahead.
            void FrontVector(Point4D* resultPtr) const;

            /// Sets the camera up vector.
            void SetUp(const Point4D& upValue);

            /// Sets the horizontal limit coordinates of the orthographic view volume.
            void SetVisibleVolumeHLimits(double left, double right)
                                        { vvLeft = left; vvRight = right; }

            /// Sets the vertical limit coordinates of the orthographic view volume.
            void SetVisibleVolumeVLimits(double top, double bottom)
                                        { vvTop = top; vvBottom = bottom; }

            /// \brief Gets the coordinates of the orthographic view volume.
            /// \return left horizontal limit
            const double GetVisibleVolumeLeftLimit()  const   { return vvLeft; }

            /// \brief Gets the coordinates of the orthographic view volume.
            /// \return right horizontal limit
            const double GetVisibleVolumeRightLimit()  const   { return vvRight; }

            /// \brief Gets the coordinates of the orthographic view volume.
            /// \return top vertical limit
            const double GetVisibleVolumeTopLimit()  const   { return vvTop; }

            /// \brief Gets the coordinates of the orthographic view volume.
            /// \return bottom vertical limit
            const double GetVisibleVolumeBottomLimit()  const   { return vvBottom; }

            /// \brief Sets the visible volume by given height.
            ///
            /// The visible volume is changed so that the origin is centered. The camera's
            /// aspect ratio is used to compute the visible volume's width. Usefull only for orthographic
            /// cameras.
            void SetVisibleVolumeHeight(double newValue);

            /// \brief Scales the visible volume.
            ///
            /// The visible volume is changed by scaling horizontal and vertical limits by given
            /// factors. Usefull when changing the viewer size.
            void ScaleVisibleVolume(float horScale, float verScale);

            /// \brief Rotates the Camera around the axis defined by the target and up vector.
            void YawAroundTarget(float radians);
            /// \brief Rotates the Camera around itself (location and up vector).
            void Yaw(float radians);
            /// \brief Rolls the Camera around itself.
            void Roll(float radians);
            /// \brief Rotates the Camera around the axis defined by the target and left vector.
            void PitchAroundTarget(float radians);
            /// \brief Moves the camera forward (relative to the camera).
            void MoveForward(double distance);
            /// \brief Moves the camera sideways.
            void MoveSideways(double distance);
            /// \brief Moves the camera up and down.
            void MoveUp(double distance);

            /// \brief Positions a camera using OpenGL commands.
            /// \return false if V-ART was not compiled with OpenGL support.
            bool DrawOGL() const;

            /// \brief Loads camera transform into rendering engine.
            ///
            /// This method if much like DrawOGL, it only does not clear the camera transformation
            /// before setting loading the camera transform. It is usefull for object selection,
            /// if called after a call to gluPickMatrix.
            void SetMatrices() const;

        protected:
            /// Indicates whether a perspective ou orthographic projection should be used.
            ProjectionType projectionType;
            /// Vertical angle of frustum. Used if projectionType == PERSPECTIVE.
            float fovY;
            /// Viewing aspect ratio (width/height).
            float aspectRatio;
            /// Distance to near plane.
            float nearPlaneDistance;
            /// Distance to far plane.
            float farPlaneDistance;

            /// \brief Visible Volume left coordinate (in world coordinates).
            /// Used if projectionType is ORTHOGRAPHIC.
            double vvLeft;
            /// \brief Visible Volume right coordinate (in world coordinates).
            /// Used if projectionType is ORTHOGRAPHIC.
            double vvRight;
            /// \brief Visible Volume top coordinate (in world coordinates).
            /// Used if projectionType is ORTHOGRAPHIC.
            double vvTop;
            /// \brief Visible Volume bottom coordinate (in world coordinates).
            /// Used if projectionType is ORTHOGRAPHIC.
            double vvBottom;

            /// Position where the camera is at.
            Point4D location;
            /// Position where the camera is looking at.
            Point4D target;
            /// Up direction, relative to the camera.
            //FixMe: Should be kept orthogonal to (target-location).
            Point4D up;
        private:
            std::string description;
    }; // end class declaration
} // end namespace
#endif  // VART_CAMERA_H
