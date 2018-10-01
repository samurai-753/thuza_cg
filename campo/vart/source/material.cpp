/// \file material.cpp
/// \brief Implementation file for V-ART class "Material".
/// \version $Revision: 1.5 $

#include "vart/material.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/gl.h>
#endif

VART::Material::Material()
    : shininess(0)
{
}

VART::Material::Material(const VART::Material& m)
{
    this->operator=(m);
}

VART::Material::Material(const VART::Color& c)
{
    SetPlasticColor(c);
}

VART::Material::Material(const VART::Texture& t)
    : texture(t)
    , shininess(0)
{
}

VART::Material::Material(const VART::Color& c, float spc, float amb, float ems, float shi)
{
    color = c;
    c.GetScaled(ems, &emissive);
    c.GetScaled(amb, &ambient);
    c.GetScaled(spc, &specular);
    shininess = shi;
}

VART::Material& VART::Material::operator=(const VART::Material& m)
{
    color = m.color;
    emissive = m.emissive;
    ambient = m.ambient;
    specular = m.specular;
    shininess = m.shininess;
    texture = m.texture;
    return *this;
}

void VART::Material::SetPlasticColor(const VART::Color& c)
{
    color = c;
    c.GetScaled(0.0001f, &emissive);
    c.GetScaled(0.3f, &ambient);
    c.GetScaled(0.1f, &specular);
    shininess = 0.01f;
}

const VART::Material& VART::Material::LIGHT_PLASTIC_GRAY()
{
    static const VART::Material lpGray(VART::Color(204,204,220));
    return lpGray;
}

const VART::Material& VART::Material::DARK_PLASTIC_GRAY()
{
    static const VART::Material dpGray(VART::Color(127,127,127));
    return dpGray;
}

const VART::Material& VART::Material::PLASTIC_WHITE()
{
    static const VART::Material pWhite(VART::Color::WHITE());
    return pWhite;
}

const VART::Material& VART::Material::PLASTIC_RED()
{
    static const VART::Material pRed(VART::Color::RED());
    return pRed;
}

const VART::Material& VART::Material::PLASTIC_GREEN()
{
    static const VART::Material pGreen(VART::Color::GREEN());
    return pGreen;
}

const VART::Material& VART::Material::PLASTIC_BLUE()
{
    static const VART::Material pBlue(VART::Color::BLUE());
    return pBlue;
}

const VART::Material& VART::Material::PLASTIC_BLACK()
{
    static const VART::Material pBlack(VART::Color::BLACK());
    return pBlack;
}

void VART::Material::SetTexture(const Texture& t)
{
    texture = t;
}

bool VART::Material::DrawOGL() const
{
#ifdef VART_OGL
    float fVec[4];

    texture.DrawOGL();
    color.Get(fVec);
    glColor4fv(fVec);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, fVec);
    ambient.Get(fVec);
    glMaterialfv(GL_FRONT, GL_AMBIENT, fVec);
    specular.Get(fVec);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fVec);
    emissive.Get(fVec);
    glMaterialfv(GL_FRONT, GL_EMISSION, fVec);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    return true;
#else
    return false;
#endif
}
