/// \file spotlight.h
/// \brief Header file for V-ART class "SpotLight".
/// \version $Revision: 1.2 $

#ifndef VART_SPOTLIGHT_H
#define VART_SPOTLIGHT_H

#include "vart/light.h"

namespace VART {
/// \brief Spot Light representation.
/// \bug Currently unmaintained.
/// A spot light is an ordinary light plus: attenuation, beamWidth, cutOffAngle,
/// direction and radius.
    class SpotLight : public Light {
        public:
            SpotLight(Point4D a, float bw, float coa, Point4D l, Point4D d, float r);
            void SetAttenuation( Point4D a );
            Point4D GetAttenuation( void );
            void SetBeamWidth( float bw );
            float GetBeamWidth( void );
            void SetCutOffAngle( float coa );
            float GetCutOffAngle( void );
            void SetDirection( Point4D d );
            Point4D GetDirection( void );
            void SetRadius( float r );
            float GetRadius( void );
        private:
            Point4D attenuation;
            float beamWidth;
            float cutOffAngle;
            Point4D direction;
            float radius;
    }; // end class declaration
} // end namespace
#endif
