/// \file transform.h
/// \brief Header file for V-ART class "Transform".
/// \version $Revision: 1.9 $

#ifndef VART_TRANSFORM_H
#define VART_TRANSFORM_H

#include "vart/point4d.h"
#include "vart/scenenode.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace VART {
    class BoundingBox;
/// \class Transform transform.h
/// \brief Geometric transformations.
///
/// A transformation matrix is represented by a 4x4 pre-multiplication matrix,
/// i.e., to apply T1, then T2, then T3, one must multiply T3*T2*T1. Translation
/// values are at the rightmost column.
/// In a scene graph, the innermost transformation (the one close to the graphic object, farther
/// away from root) is the one applied first.
    class Transform : public SceneNode {
        /// 'ostream' class extension to output a transformation on the console.
        friend std::ostream& operator<<(std::ostream& output, const Transform& t);
        public:
            /// \brief Creates an uninitialized transform.
            Transform();

            /// \brief Copy constructor.
            /// \param trans [in] Source transform
            Transform(const Transform& trans);

            /// \brief Returns a copy of an Transform. Every derived class must reimplements 
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();
            
            /// \brief Turns transform into identity.
            void MakeIdentity();

            /// \brief Applies transformation to a point.
            /// \param point [in] Point to apply transformation
            /// \return The transformed point.
            Point4D operator*(const Point4D& point) const;

            /// \brief Applies transformation to another transform.
            /// \param t [in] Transform to apply transformation
            /// \return The combined transform.
            Transform operator*(const Transform& t) const;

            /// \brief Copies data from anoter transform.
            Transform& operator=(const Transform& t);

            /// \brief Applies a transformation to itself.
            void Apply(const Transform& t);

            /// \brief Applies tranformation to a point.
            /// \param ptPoint [in,out] Point to be transformed
            void ApplyTo(Point4D* ptPoint) const;

            /// \brief Turns transform into a translation.
            ///
            /// MakeTranslation expects a vector but actualy ignores the W coordinate.
            /// \param translationVector [in] Vector representation of the translation
            void MakeTranslation(const Point4D& translationVector);

            /// \brief Turns transform into a translation.
            /// \param tx [in] X component of translation
            /// \param ty [in] Y component of translation
            /// \param tz [in] Z component of translation
            void MakeTranslation(double tx, double ty, double tz)
                { MakeTranslation(Point4D(tx,ty,tz,0)); }

            /// \brief Turns transform into a rotation around the X axis.
            /// \param radians [in] Angle of rotation
            void MakeXRotation(double radians);

            /// \brief Turns transform into a rotation around the Y axis.
            /// \param radians [in] Angle of rotation
            void MakeYRotation(double radians);

            /// \brief Turns transform into a rotation around the Z axis.
            /// \param radians [in] Angle of rotation
            void MakeZRotation(double radians);

            /// \brief Turns transform into a rotation (around some reference vetor).
            /// \param refVec [in] Reference vector
            /// \param radians [in] Angle of rotation
            void MakeRotation(const Point4D& refVec, const float radians);

            /// \brief Turns transform into a rotation (around some reference axis).
            /// \param refPoint [in] Reference point (defines an axis together with refVec)
            /// \param refVec [in] Reference vector (defines an axis together with refPoint)
            /// \param radians [in] Angle of rotation
            void MakeRotation(const Point4D& refPoint, const Point4D& refVec, const float radians);

            /// \brief Turns transform into a scale.
            void MakeScale(double sX, double sY, double sZ);

            /// \brief Turns transform into a shear.
            void MakeShear(double shX, double shY);

            /// \brief Set all data in the transform.
            void SetData(double* data);

            /// \brief Returns the address of transformation matrix.
            ///
            /// Use this method to get an OpenGl like transformation matrix, compatible
            /// with methods such as "glLoadMatrixd" and "glMultMatrixd".
            const double* GetData() const { return matrix; }

            /// \brief Returns the X vector of the transform.
            ///
            /// A rigid transformation matrix may be interpreted as a coordinate system.
            /// This method returns the 1st column of the matrix which may be interpreted
            /// as the X vector of such coordinate system.
            void GetVectorX(Point4D* result) const;

            /// \brief Returns the Y vector of the transform.
            ///
            /// A rigid transformation matrix may be interpreted as a coordinate system.
            /// This method returns the 2nd column of the matrix which may be interpreted
            /// as the Y vector of such coordinate system.
            void GetVectorY(Point4D* result) const;

            /// \brief Returns the Z vector of the transform.
            ///
            /// A rigid transformation matrix may be interpreted as a coordinate system.
            /// This method returns the 3rd column of the matrix which may be interpreted
            /// as the Z vector of such coordinate system.
            void GetVectorZ(Point4D* result) const;

            /// \brief Returns the translation part of the transform.
            void GetTranslation(Point4D* result) const;

            /// \brief Check for NaNs inside the matrix.
            ///
            /// This is for debugging purposes only.
            #ifndef NDEBUG
            bool HasNaN() const;
            #endif

            /// \brief Apply transform to rendering engine.
            /// \return false if V-ART was not compiled with OpenGL support.
            virtual bool DrawOGL() const;

            /// \brief Draws and object, setting pick info
            ///
            /// This method should be called in selection mode in order to identify objects
            /// that are selected by the mouse (see Scene::Pick).
            virtual void DrawForPicking() const;

            virtual TypeID GetID() const { return TRANSFORM; }

            /// \brief Returns the recursive bounding box.
            /// \param bBox [out] recursive bounding box.
            /// \return true if the is a return value exists.
            virtual bool RecursiveBoundingBox(BoundingBox* bBox);

            /// Toggles the recursive object's visibility.
            void ToggleRecVisibility();

            /// \brief Copies matrix data from another transform.
            ///
            /// Unlike operator=, CopyMatrix copies only the internal matrix.
            void CopyMatrix(const Transform& t);

        protected:
        // PROTECTED ATTRIBUTES
            double matrix[16];
        private:
        // PRIVATE METHODS
            bool Zero(const double& n) { return (fabs(n) < 0.0000001); }
    }; // end class declaration
} // end namespace
#endif
