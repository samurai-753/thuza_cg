/// \file light.h
/// \brief Header file for V-ART class "Light".
/// \version $Revision: 1.6 $

#ifndef VART_LIGHT_H
#define VART_LIGHT_H

#include "vart/point4d.h"
#include "vart/color.h"
#include "vart/scenenode.h"
#include "vart/transform.h"
#include <string>

namespace VART {
    class BoundingBox;
/// \class Light light.h
/// \brief Represents a light source.
///
/// This class represents simple light sources, i.e.: directional lights or point
/// lights with no attenuation. See the derived classes for more complex light sources.
/// Lights can have children, but they should not.
    class Light : public SceneNode {
        public:
            Light();
            Light(const Light& light);
            Light(float i, float ai, Color c, bool o);
            Light(const std::string& newDescription, float newIntensity,
                    float newAmbientIntensity, const Color& newColor,
                    const Point4D& newLocation);
            virtual ~Light();
            /// \brief Returns a copy of an Light. Every derived class must reimplements 
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual SceneNode * Copy();
            Light& operator=(const Light& light);
            void SetIntensity(float i);
            float GetIntensity() const;
            void SetAmbientIntensity(float ai);
            float GetAmbientIntensity() const;
            void SetColor(const Color& c);
            Color GetColor() const;
            /// Turns a light on or off.
            void Turn(bool on_off);
            bool IsOn() const;

            /// \brief Always returns false, therefore recursive bbox does not exist.
            virtual bool RecursiveBoundingBox(BoundingBox* bBox);

            /// \brief Sets the location of the light.

            /// newLocation should be a point for positional lights or a direction for
            /// directional lights.
            void SetLocation(const Point4D& newLocation);
            Point4D GetLocation() const { return location; }

			/// \brief Gets a pointer to the light transform.

            /// This transforms gets applied to the light each time it's
			/// drawn. It does nothing by default.
            Transform* GetTransform() const { return transform; }

            /// \brief Draws (sets up) a light using OpenGL commands.
            /// \return false if V-ART was not compiled with OpenGL support.
            /// \param oglLightID [in] OpenGL ID for light in the range 0..7.
            virtual bool DrawOGL(unsigned int oglLightID) const;

            /// \brief Draws and object, setting pick info
            ///
            /// This method should be called in selection mode in order to identify objects
            /// that are selected by the mouse (see Scene::Pick).
            virtual void DrawForPicking() const {};

        // CLASS CONSTANT INITIALIZERS
            /// White directional light towards negative Y. Small ambient intensity.
            static const Light& SUN();
            /// Strong, white ambient light.
            static const Light& BRIGHT_AMBIENT();
		protected:
			unsigned int getOpenGLID(unsigned int lightID) const;

        private:
            float intensity;
            float ambientIntensity;
            Color color;
            bool on;
		protected:
			Transform *transform;
            Point4D location; //unified representation of location/direction.
    }; // end class declaration
} // end namespace
#endif  // VART_LIGHT_H
