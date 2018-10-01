/// \file rotationaction.h
/// \brief Header file for V-ART class "RotationAction".
/// \version $Revision: 1.1 $

#ifndef VART_ROTATIONACTION_H
#define VART_ROTATIONACTION_H

#include "vart/transformaction.h"
#include "vart/point4d.h"

namespace VART {
/// \class RotationAction rotationaction.h
/// \brief ...
///
/// FixMe: Put Detailed description here
    class RotationAction : public TransformAction
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            RotationAction();
            virtual ~RotationAction() {}
            void SetXRotation(double radians);
            void SetYRotation(double radians);
            void SetZRotation(double radians);
            virtual void Move();
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            /// \brief A scalar that describes the final angle of the rotation.
            double goal;
            /// \brief The point portion of the axis of rotation.
            Point4D refPoint;
            /// \brief The vector portion of the axis of rotation.
            Point4D refVector;
    }; // end class declaration
} // end namespace

#endif
