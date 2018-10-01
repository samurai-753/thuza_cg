//! \file pointlight.cpp
//! \brief Implementation file for V-ART class "PointLight".
//! \version $Revision: 1.4 $

#include "vart/pointlight.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/gl.h>
#endif

VART::PointLight::PointLight(Point4D loc, float constantAtt, float linearAtt, float quadraticAtt){
    SetLocation(loc);
	SetAttenuation(constantAtt, linearAtt, quadraticAtt);
}

void VART::PointLight::SetAttenuation(float constant, float linear, float quadratic){
	constantAttenuation = constant;
	linearAttenuation = linear;
	quadraticAttenuation = quadratic;
}

bool VART::PointLight::DrawOGL(unsigned int oglLightID) const {
#ifdef VART_OGL
	GLenum realID = (GLenum) getOpenGLID(oglLightID);

	GLfloat pos[4];
	pos[0] = location.GetX();
	pos[1] = location.GetY();
	pos[2] = location.GetZ();
	pos[3] = location.GetW();

	glLightfv(realID, GL_POSITION, pos);
	glLightf(realID, GL_CONSTANT_ATTENUATION, constantAttenuation);
	glLightf(realID, GL_LINEAR_ATTENUATION, linearAttenuation);
	glLightf(realID, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);

	VART::Light::DrawOGL(oglLightID);

	return true;
#else
    return false;
#endif
}
