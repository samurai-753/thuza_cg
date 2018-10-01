/// \file bezier.h
/// \brief Header file for V-ART class "Bezier".
/// \version $Revision: 1.2 $

#ifndef VART_BEZIER_H
#define VART_BEZIER_H

#include "vart/curve.h"
#include "vart/point4d.h"

namespace VART {
/// \class Bezier bezier.h
/// \brief A Bezier curve.
///
/// A Bezier curve defined by 4 control points.
    class Bezier : public Curve {
        public:
            Bezier();
            /// \brief Returns a copy a control point.
            Point4D GetControlPoint(int i);

            /// \brief Assigns a (copy of a) control point.
            void SetControlPoint(int i, const Point4D& point);

            /// \brief Returns a point of the curve.
            /// \param t [in] curve parameter describing the desired point.
            /// \param result [out] the point at position t.
            virtual void GetPoint(double t, Point4D* result);
    }; // end class declaration
} // end namespace
#endif
