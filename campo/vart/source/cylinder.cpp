/// \file cylinder.cpp
/// \brief Implementation file for V-ART class "Cylinder".
/// \version $Revision: 1.4 $

#include "vart/cylinder.h"
#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/glu.h>
#endif

//?
#include <iostream>
using namespace std;


VART::Cylinder::Cylinder() : VART::GraphicObj()
{
}

VART::Cylinder::Cylinder(const VART::Cylinder& cylinder)
{
    this->operator =(cylinder);
}

VART::Cylinder& VART::Cylinder::operator=(const VART::Cylinder& cylinder)
{
    this->GraphicObj::operator=(cylinder);
    height = cylinder.height;
    topRadius = cylinder.topRadius;
    btRadius = cylinder.btRadius;
    sidesVisible = cylinder.sidesVisible;
    topVisible = cylinder.topVisible;
    bottomVisible = cylinder.bottomVisible;
    material = cylinder.material;
    return *this;
}

VART::SceneNode * VART::Cylinder::Copy()
{
    return new VART::Cylinder(*this);
}

VART::Cylinder::Cylinder(float fHi, float fRad) : VART::GraphicObj()
{
    height = fHi;
    topRadius = btRadius = fRad;
    sidesVisible = topVisible = bottomVisible = true;
    bBox.SetBoundingBox(-fRad, -fRad, 0, fRad, fRad, fHi);
    recBBox = bBox;
}

VART::Cylinder::Cylinder( float fHi, float fRad, bool bS, bool bT, bool bB )
           : VART::GraphicObj()
{
    height = fHi;
    topRadius = btRadius = fRad;
    sidesVisible = bS;
    topVisible = bT;
    bottomVisible = bB;
    bBox.SetBoundingBox(-fRad, -fRad, 0, fRad, fRad, fHi);
    recBBox = bBox;
}

void VART::Cylinder::ComputeBoundingBox()
{
    float maxRadius = (topRadius > btRadius)? topRadius : btRadius;
    bBox.SetBoundingBox(-maxRadius, -maxRadius, 0, maxRadius, maxRadius, height);
}

void VART::Cylinder::SetHeight(float h)
{
    float maxRadius = (topRadius > btRadius)? topRadius : btRadius;
    height = h;
    bBox.SetBoundingBox(-maxRadius, -maxRadius, 0, maxRadius, maxRadius, h);
    ComputeRecursiveBoundingBox();
}

void VART::Cylinder::SetRadius(float r)
{
    topRadius = btRadius = r;
    bBox.SetBoundingBox(-r, -r, 0, r, r, height);
    ComputeRecursiveBoundingBox();
}

void VART::Cylinder::SetPartsVisibility(PartsID parts)
{
    bottomVisible = static_cast<bool>(parts & BOTTOM);
    sidesVisible = static_cast<bool>(parts & SIDES);
    topVisible = static_cast<bool>(parts & TOP);
}

void VART::Cylinder::TogglePartsVisibilty(PartsID parts)
{
    if (parts & BOTTOM) bottomVisible = !bottomVisible;
    if (parts & SIDES) sidesVisible = !sidesVisible;
    if (parts & TOP) topVisible = !topVisible;
}

VART::Cylinder::PartsID VART::Cylinder::GetPartsVisibility()
{
    PartsID result = NONE;
    if (bottomVisible) result = result & BOTTOM;
    if (sidesVisible) result = result & SIDES;
    if (topVisible) result = result & TOP;
    return result;
}

void VART::Cylinder::ShowSide(bool yesno)
{
    sidesVisible = yesno;
    cerr << "Warning: VART::Cylinder::ShowSide is deprecated." << endl;
}

void VART::Cylinder::ShowTop(bool yesno)
{
    topVisible = yesno;
    cerr << "Warning: VART::Cylinder::ShowTop is deprecated." << endl;
}

void VART::Cylinder::ShowBottom(bool yesno)
{
    bottomVisible = yesno;
    cerr << "Warning: VART::Cylinder::ShowBottom is deprecated." << endl;
}

float VART::Cylinder::GetHeight(void)
{
    return height;
}

float VART::Cylinder::GetTopRadius(void)
{
    return topRadius;
}

float VART::Cylinder::GetBottomRadius(void)
{
    return btRadius;
}

bool VART::Cylinder::ShowSide(void)
{
    cerr << "Warning: VART::Cylinder::ShowSide is deprecated." << endl;
    return sidesVisible;
}

bool VART::Cylinder::ShowTop(void)
{
    cerr << "Warning: VART::Cylinder::ShowTop is deprecated." << endl;
    return topVisible;
}

bool VART::Cylinder::ShowBottom(void)
{
    cerr << "Warning: VART::Cylinder::ShowBottom is deprecated." << endl;
    return bottomVisible;
}

bool VART::Cylinder::DrawInstanceOGL() const
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
            gluCylinder(qObj, btRadius, topRadius, height, 15, 1);
        }
        // Render bottom
        if (bottomVisible) {
            gluQuadricOrientation(qObj,GLU_INSIDE);
            gluDisk(qObj, 0.0, btRadius, 15, 1);
        }
        // Render top
        if (topVisible) {
            glPushMatrix();
            gluQuadricOrientation(qObj,GLU_OUTSIDE);
            glTranslatef(0.0f, 0.0f, height);
            gluDisk(qObj, 0.0, topRadius, 15, 1);
            glPopMatrix();
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
