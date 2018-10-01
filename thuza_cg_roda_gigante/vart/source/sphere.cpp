/// \file sphere.cpp
/// \brief Implementation file for V-ART class "Sphere".
/// \version $Revision: 1.5 $

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/glu.h>
#endif
#include "vart/sphere.h"
#include <iostream>

using namespace std;

VART::Sphere::Sphere() : VART::GraphicObj() {
    radius = 1.0;
    ComputeBoundingBox();
}

VART::Sphere::Sphere(const VART::Sphere& sphere)
{
    this->operator=(sphere);
}

VART::Sphere& VART::Sphere::operator=(const VART::Sphere& sphere)
{
    this->GraphicObj::operator=(sphere);
    radius = sphere.radius;
    material = sphere.material;
    return *this;
}

VART::SceneNode * VART::Sphere::Copy()
{
    return new Sphere(*this);
}

VART::Sphere::Sphere( float fRad ) : VART::GraphicObj() {
    radius = fRad;
    ComputeBoundingBox();
}

void VART::Sphere::SetRadius( float r ) {
    radius = r;
    ComputeBoundingBox();
}

float VART::Sphere::GetRadius( void ) {
    return radius;
}

void VART::Sphere::ComputeBoundingBox() {
    bBox.SetSmallerX(-radius);
    bBox.SetSmallerY(-radius);
    bBox.SetSmallerZ(-radius);
    bBox.SetGreaterX(radius);
    bBox.SetGreaterY(radius);
    bBox.SetGreaterZ(radius);
    //oobBox=VART::OOBoundingBox(bBox);
}

bool VART::Sphere::DrawInstanceOGL() const {
#ifdef VART_OGL
    GLUquadricObj* qObj = gluNewQuadric();
    bool result = true;

    if (show)
    {
        if( material.GetTexture().HasData() )
        {
            gluQuadricTexture(qObj,GL_TRUE);
        }
        result = material.DrawOGL();
        gluQuadricDrawStyle(qObj, GLU_FILL);
        gluQuadricNormals(qObj, GLU_SMOOTH);
        gluSphere(qObj, radius, 15, 15);
        gluDeleteQuadric(qObj);
    }
    if (bBox.visible)
        bBox.DrawInstanceOGL();
    if (recBBox.visible)
        recBBox.DrawInstanceOGL();
    return result;
#else
    return false;
#endif
}
