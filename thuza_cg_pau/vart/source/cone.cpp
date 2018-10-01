/// \file cone.cpp
/// \brief Implementation file for V-ART class "Cone".
/// \version $Revision: 1.0 $

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/glu.h>
#endif
#include "vart/cone.h"

#include <iostream>
using namespace std;


VART::Cone::Cone() : VART::GraphicObj()
{
}


VART::Cone::Cone(const VART::Cone& cone)
{
    this->operator =(cone);
}

VART::Cone& VART::Cone::operator=(const VART::Cone& cone)
{
    this->GraphicObj::operator=(cone);
    height = cone.height;
    btRadius = cone.btRadius;
    sidesVisible = cone.sidesVisible;
    topVisible = cone.topVisible;
    bottomVisible = cone.bottomVisible;
    material = cone.material;
    return *this;
}



VART::Cone::Cone(float fHi, float fRad) : VART::GraphicObj()
{
    height = fHi;
    topRadius = 0.0;
    btRadius = fRad;
    sidesVisible = topVisible = bottomVisible = true;
    bBox.SetBoundingBox(-fRad, -fRad, 0, fRad, fRad, fHi);
    recBBox = bBox;
}


VART::SceneNode * VART::Cone::Copy()
{
    return new VART::Cone(*this);
}


VART::Cone::Cone( float fHi, float fRad, bool bS, bool bB ) : VART::GraphicObj()
{
    height = fHi;
    topRadius = 0.0;
    btRadius = fRad;
    sidesVisible = bS;
    bottomVisible = bB;
    bBox.SetBoundingBox(-fRad, -fRad, 0, fRad, fRad, fHi);
    recBBox = bBox;    
}

void VART::Cone::ComputeBoundingBox()
{
    float maxRadius = btRadius;
    bBox.SetBoundingBox(-maxRadius, -maxRadius, 0, maxRadius, maxRadius, height);
}

void VART::Cone::SetHeight(float h)
{
    float maxRadius = btRadius;
    height = h;
    bBox.SetBoundingBox(-maxRadius, -maxRadius, 0, maxRadius, maxRadius, h);
    ComputeRecursiveBoundingBox();
}

void VART::Cone::SetRadius(float r)
{
    btRadius = r;
    bBox.SetBoundingBox(-r, -r, 0, r, r, height);
    ComputeRecursiveBoundingBox();
}

void VART::Cone::SetPartsVisibility(PartsID parts)
{
    bottomVisible = static_cast<bool>(parts & BOTTOM);
    sidesVisible = static_cast<bool>(parts & SIDES);
}

void VART::Cone::TogglePartsVisibilty(PartsID parts)
{
    if (parts & BOTTOM) bottomVisible = !bottomVisible;
    if (parts & SIDES) sidesVisible = !sidesVisible;
}

VART::Cone::PartsID VART::Cone::GetPartsVisibility()
{
    PartsID result = NONE;
    if (bottomVisible) result = result & BOTTOM;
    if (sidesVisible) result = result & SIDES;
    return result;
}

void VART::Cone::ShowSide(bool yesno)
{
    sidesVisible = yesno;
    cerr << "Warning: VART::Cone::ShowSide is deprecated." << endl;
}


void VART::Cone::ShowBottom(bool yesno)
{
    bottomVisible = yesno;
    cerr << "Warning: VART::Cone::ShowBottom is deprecated." << endl;
}

float VART::Cone::GetHeight(void)
{
    return height;
}

float VART::Cone::GetRadius(void)
{
    return btRadius;
}

bool VART::Cone::ShowSide(void)
{
    cerr << "Warning: VART::Cone::ShowSide is deprecated." << endl;
    return sidesVisible;
}

bool VART::Cone::ShowBottom(void)
{
    cerr << "Warning: VART::Cone::ShowBottom is deprecated." << endl;
    return bottomVisible;
}

bool VART::Cone::DrawInstanceOGL() const
{
#ifdef VART_OGL
    GLUquadricObj* qObj = gluNewQuadric();
    bool result = true;

    if (show)
    {
        switch (howToShow)
        {
            case LINES:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case POINTS:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                break;
            default:
                glPolygonMode(GL_FRONT, GL_FILL);
                break;
        }
        if ( material.GetTexture().HasData() ) {
            gluQuadricTexture(qObj,GL_TRUE);
        }
        result = material.DrawOGL();
        // Render sides
        if (sidesVisible) {
            gluQuadricDrawStyle(qObj, GLU_FILL);
            gluQuadricNormals(qObj, GLU_SMOOTH);
            gluCylinder(qObj, btRadius, 0.0 , height, 15, 1);
        }
        // Render bottom
        if (bottomVisible) {
            gluQuadricOrientation(qObj,GLU_INSIDE);
            gluDisk(qObj, 0.0, btRadius, 15, 1);
        }
        gluDeleteQuadric(qObj);
    }
    if (bBox.visible) // Is the bounding box visible?
        bBox.DrawInstanceOGL();
    if (recBBox.visible)
        recBBox.DrawInstanceOGL();
    return result;
#else
    return false;
#endif
}


