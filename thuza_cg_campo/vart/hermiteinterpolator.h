/// \file hermiteinterpolator.h
/// \brief Header file for V-ART class "HermiteInterpolator".
/// \version $Revision: $

#ifndef VART_HERMITEINTERPOLATOR_H
#define VART_HERMITEINTERPOLATOR_H

#include "vart/interpolator.h"
#include <vector>

namespace VART {
/// \class HermiteInterpolator hermiteinterpolator.h
/// \brief Hermite Spline interpolation
///
/// Interpolates positions using Hermite Curves.
/// Hermite interpolators have a set of controls points that are use to control the interpolation
/// values, i.e.: the curve passes thru control points. That given, hermite interpolators may not
/// be a good thing to share among dof movers.
    class HermiteInterpolator : public Interpolator {
        public:
        // PUBLIC METHODS
            HermiteInterpolator(float finalTime, float finalPosition);
            virtual float GetValue(float linearIndex, float initialPos, float range) const
                { return GetValue(linearIndex); }
            virtual float GetValue(float linearIndex) const;
            virtual TypeID GetID() const { return HERMITE; }

            /// \brief Sets initial position for interpolation
            void SetInitialPosition(float p) { positionVec[0] = p; }

            /// \brief Adds a control position for interpolation
            ///
            /// A control position is a value that should be given by interpolation at given time.
            void AddControlPosition(float pos, float time);
        protected:
        // PROTECTED METHODS
            /// \brief Warp index around valid vector indices
            int WarpIndex(int i) const;
        // STATIC PROTECTED METHODS
            /// \brief Hermite 1D interpolation
            static float Interpolate(float p1, float tg1, float p2, float tg2, float t);
        // PROTECTED ATTRIBUTES
            /// A vector of positions to interpolate
            std::vector<float> positionVec;

            /// \brief A vector of times for each position
            ///
            /// The times are passed as joint movement normalized times, i.e.: values where 0 means
            /// the beginning of joint movement and 1 means end of joint movement. But are stored as
            /// dof movement normalized time, i.e.: values where 0 means the beginning of dof movment
            /// and 1 menas the end of dof movement.
            std::vector<float> timeVec;
    }; // end class declaration
} // end namespace
#endif
