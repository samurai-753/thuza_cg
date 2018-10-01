/// \file sphere.h
/// \brief Header file for V-ART class "Sphere".
/// \version $Revision: 1.3 $

#ifndef VART_SPHERE_H
#define VART_SPHERE_H

#include "vart/graphicobj.h"
#include "vart/material.h"

namespace VART {
/// \class Sphere sphere.h
/// \brief A sphere.
///
/// Spheres are created with center at the origin.
    class Sphere : public GraphicObj {
        public:
            Sphere();
            /// \brief Creates a new copy of an sphere.
            Sphere(const VART::Sphere& sphere);
            
            Sphere( float fRad );
            
            /// Copies all Sphere data to another sphere.
            VART::Sphere & operator=(const VART::Sphere & sphere);
            
            /// \brief Returns a copy of an Sphere. Every derived class must reimplements 
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();
            
            void SetRadius( float r );
            /// Material assignment.
            void SetMaterial(const Material& mat) { material = mat; }
            /// Returns of copy of the cylinder's material.
            Material GetMaterial() const { return material; }
            float GetRadius();
            /// \brief Draws the sphere using OpenGL engine.
            /// \return false if V-ART has not been compiled with OpenGL support.
            bool DrawInstanceOGL() const;
            virtual void ComputeBoundingBox();
        private:
            Material material;
            float radius;
    }; // end class declaration
} // end namespace
#endif
