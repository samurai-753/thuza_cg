/// \file linearinterpolator.h
/// \brief Header file for V-ART class "LinearInterpolator".
/// \version $Revision: 1.4 $

#ifndef VART_LINEARINTERPOLATOR_H
#define VART_LINEARINTERPOLATOR_H

#include "vart/interpolator.h"

namespace VART {
/// \class LinearInterpolator linearinterpolator.h
/// \brief Linear interpolation
///
/// This interpolation function is linear and has period of 2 units.
    class LinearInterpolator : public Interpolator {
        public:
            LinearInterpolator();
            //~ virtual float GetValue(float n) const;
            virtual float GetValue(float linearIndex, float initialPos, float range) const;
            virtual TypeID GetID() const { return LINEAR; }
    }; // end class declaration
} // end namespace
#endif
