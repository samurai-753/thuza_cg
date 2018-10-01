/// \file pointlight.h
/// \brief Header file for V-ART class "PointLight".
/// \version $Revision: 1.4 $

#ifndef VART_POINTLIGHT_H
#define VART_POINTLIGHT_H

#include "vart/light.h"

namespace VART {
/// \brief Point light source.
///
/// This is a Light with attenuation.
    class PointLight : public Light {
        private:
            //Point4D location;
			float constantAttenuation;
			float linearAttenuation;
			float quadraticAttenuation;

        public:
			PointLight();
            PointLight(Point4D loc, float constantAtt=1.0, float linearAtt=0.0, float quadraticAtt=0.0);

			/// \brief Sets the three attenuation factors, described on OpenGL Red Book, 
			/// Chapter 5: Lighting, section Creating Light Sources.
			/// \param constant [in] Constant attenuation factor.
			/// \param linear [in] Linear attenuation factor.
			/// \param quadratic [in] Quadratic attenuation factor.
            void SetAttenuation(float constant=1.0, float linear=0.0, float quadratic=0.0);

			float GetConstantAttenuation() const {return constantAttenuation;}
			float GetLinearAttenuation() const { return linearAttenuation; }
			float GetQuadraticAttenuation() const { return quadraticAttenuation; }

			/// \brief Draws (sets up) a light using OpenGL commands.
            /// \return false if V-ART was not compiled with OpenGL support.
            /// \param oglLightID [in] OpenGL ID for light in the range 0..7.
            virtual bool DrawOGL(unsigned int oglLightID) const;
			
			void SetLocation(const Point4D& newLocation) { location = newLocation; }
            Point4D GetLocation() const { return location; }

    }; // end class declaration
} // end namespace
#endif
