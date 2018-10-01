/// \file sineinterpolator.h
/// \brief Header file for V-ART class "SineInterpolator".
/// \version $Revision: 1.4 $

#ifndef VART_SINEINTERPOLATOR_H
#define VART_SINEINTERPOLATOR_H

#include "vart/interpolator.h"

namespace VART {
/// \class SineInterpolator sineinterpolator.h
/// \brief Smooth (sine function) interpolator
///
/// This is a smooth (following sine function) interpolator with period of 2 units.
/// Also known as ease-in ease-out interpolator.
    class SineInterpolator : public Interpolator {
        public:
            SineInterpolator();
            //~ virtual float GetValue(float n) const;
            virtual float GetValue(float linearIndex, float initialPos, float range) const;
            virtual TypeID GetID() const { return EASE_IN_EASE_OUT; }
    }; // end class declaration
} // end namespace
#endif
