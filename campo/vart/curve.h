/// \file curve.h
/// \brief Header file for V-ART class "Curve".
/// \version $Revision: 1.2 $

#ifndef VART_CURVE_H
#define VART_CURVE_H

#include "vart/graphicobj.h"
#include "vart/point4d.h"
#include "vart/transform.h"
#include <vector>

namespace VART {
/// \class Curve curve.h
/// \brief A paramteric curve in 3D/4D space.
///
/// FixMe: Put detailed description here.
    class Curve : public GraphicObj {
        public:
            /// \brief Returns a point of the curve.
            /// \param t [in] curve parameter describing the desired point.
            /// \param result [out] the point at position t.
            virtual void GetPoint(double t, Point4D* result)=0;

            /// \brief Applies a transformation to all controlPoints
            void ApplyTransform(const Transform& t);

            // FixMe: Implement DrawInstanceOGL?
        protected:
            std::vector<Point4D> controlPoints;
    }; // end class declaration
} // end namespace
#endif
