/// \file rangesineinterpolator.h
/// \brief Header file for V-ART class "SineInterpolator".
/// \version $Revision: 1.3 $

#ifndef VART_RANGESINEINTERPOLATOR_H
#define VART_RANGESINEINTERPOLATOR_H

#include "vart/sineinterpolator.h"

namespace VART {
/// \class RangeSineInterpolator rangesineinterpolator.h
/// \brief Smooth (sine function) interpolator with range
///
/// This is a smooth (following sine function) interpolator with period of 2 units,
/// with a range that defines the amplitute. The values of the range must be among 0 and 1.
    class RangeSineInterpolator : public SineInterpolator {
        public:
            RangeSineInterpolator();
            void SetMinimunLimit(float minLim);
            void SetMaximunLimit(float maxLimit);
            //~ virtual float GetValue(float n) const;
            virtual float GetValue(float linearIndex, float initialPos, float range) const;
        private:
            float minLimit;
            float maxLimit;
    }; // end class declaration
} // end namespace
#endif
