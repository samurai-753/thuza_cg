/// \file light.cpp
/// \brief Implementation file for V-ART class "Light".
/// \version $Revision: 1.8 $
#include "vart/light.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/gl.h>
#endif

using namespace std;

VART::Light::Light() {
    // FixMe: As with most V-ART default constructors, an unitilized object could be
    // created here.
    color.SetRGBA(255,255,255,255);
    intensity = 1;
    ambientIntensity = 1;
    on = true;

	transform = new VART::Transform();
	transform->MakeIdentity();
}

VART::Light::Light (float i, float ai, VART::Color c, bool o) {
    intensity=i;
    ambientIntensity=ai;
    color=c;
    on=o;

	transform = new VART::Transform();
	transform->MakeIdentity();
}

VART::Light::Light(const string& newDescription, float newIntensity,
                 float newAmbientIntensity, const VART::Color& newColor,
                 const VART::Point4D& newLocation)
{
    description = newDescription;
    intensity = newIntensity;
    ambientIntensity = newAmbientIntensity;
    color = newColor;
    location = newLocation;
    on = true;

	transform = new VART::Transform();
	transform->MakeIdentity();
}

VART::Light::Light(const VART::Light& light) {
    description = light.description;
    intensity = light.intensity;
    ambientIntensity = light.ambientIntensity;
    color = light.color;
    location = light.location;
    on = light.on;

	transform = new VART::Transform(*light.transform);
}

VART::SceneNode * VART::Light::Copy()
{
    return new VART::Light(*this);
}

VART::Light::~Light() {
	delete transform;
}

VART::Light& VART::Light::operator=(const VART::Light& light) {
    description = light.description;
    intensity = light.intensity;
    ambientIntensity = light.ambientIntensity;
    color = light.color;
    location = light.location;
    on = light.on;
    return (*this);
}

const VART::Light& VART::Light::SUN() {
    static const VART::Light sun("TheSun", 0.7f, 0.7f, VART::Color::WHITE(), VART::Point4D(0,-1,0,0));
    return sun;
}
const VART::Light& VART::Light::BRIGHT_AMBIENT() {
    static const VART::Light ba("BrightAmbient", 0.3f, 0.7f, VART::Color::WHITE(), VART::Point4D::ORIGIN());
    return ba;
}

void VART::Light::SetIntensity(float i) {
    intensity = i;
}

float VART::Light::GetIntensity() const {
    return(intensity);
}

void VART::Light::SetAmbientIntensity(float ai){
    ambientIntensity = ai;
}

float VART::Light::GetAmbientIntensity() const {
    return(ambientIntensity);
}

void VART::Light::SetColor(const VART::Color& c){
    color = c;
}

VART::Color VART::Light::GetColor() const {
    return(color);
}

void VART::Light::Turn(bool on_off){
    on=on_off;
}

bool VART::Light::IsOn() const {
    return on;
}

void VART::Light::SetLocation(const VART::Point4D& newLocation){
    location = newLocation;
}

bool VART::Light::DrawOGL(unsigned int oglLightID) const {
// The ID is needed because OpenGL calls require it.
#ifdef VART_OGL
    if (on) { // Is light enabled?
        float weightedColor[4];
		GLenum realID = getOpenGLID(oglLightID);
		
		//switch (oglLightID) {
  //          case 0:
  //              realID = GL_LIGHT0;
  //              break;
  //          case 1:
  //              realID = GL_LIGHT1;
  //              break;
  //          case 2:
  //              realID = GL_LIGHT2;
  //              break;
  //          case 3:
  //              realID = GL_LIGHT3;
  //              break;
  //          case 4:
  //              realID = GL_LIGHT4;
  //              break;
  //          case 5:
  //              realID = GL_LIGHT5;
  //              break;
  //          case 6:
  //              realID = GL_LIGHT6;
  //              break;
  //          default:
  //              realID = GL_LIGHT7;
  //              break;
  //      }

        color.GetScaled(ambientIntensity, weightedColor);
        glLightfv(realID, GL_AMBIENT, weightedColor);
        color.GetScaled(intensity, weightedColor);
        glLightfv(realID, GL_DIFFUSE, weightedColor);

		glEnable(realID);
    }
    // FixMe: if light is turned off, it seems that glDisable should be called.
    return true;
#else
    return false;
#endif
}

bool VART::Light::RecursiveBoundingBox(VART::BoundingBox* bBox) {
// virtual method
    return false;
}

unsigned int VART::Light::getOpenGLID(unsigned int lightID) const {
	switch (lightID) {
		case 0:
			return GL_LIGHT0;
		case 1:
			return GL_LIGHT1;
		case 2:
			return GL_LIGHT2;
		case 3:
			return GL_LIGHT3;
		case 4:
			return GL_LIGHT4;
		case 5:
			return GL_LIGHT5;
		case 6:
			return GL_LIGHT6;
		default:
			return GL_LIGHT7;
	}
}

