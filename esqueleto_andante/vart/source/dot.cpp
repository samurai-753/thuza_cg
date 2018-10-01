/// \file dot.cpp
/// \brief Implementation file for V-ART class "Dot".
/// \version $Revision: 1.3 $

#include "vart/dot.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
    #include <GL/gl.h>
#endif
//#include <iostream>

//using namespace std;

VART::Dot::Dot()
{
    size = 3.0f;
    show = true;
}

VART::Dot::Dot(const VART::Dot& dot)
{
    this->operator=(dot);
}

VART::Dot& VART::Dot::operator=(const VART::Dot& dot)
{
    this->GraphicObj::operator=(dot);
    position = dot.position;
    color = dot.color;
    size = dot.size;
    return *this;
}

VART::SceneNode * VART::Dot::Copy()
{
    return new VART::Dot(*this);
}

VART::Dot::Dot(const VART::Point4D& location)
{
    size = 3.0f;
    show = true;
    position = location;
}

void VART::Dot::ComputeBoundingBox()
{
    bBox.SetBoundingBox(position.GetX(), position.GetY(), position.GetZ(),
                        position.GetX(), position.GetY(), position.GetZ());
}

bool VART::Dot::DrawInstanceOGL() const
{
#ifdef VART_OGL
    static float fColor[4];
    if (show)
    {
        color.Get(fColor);
        glPointSize(size); // FixMe: remove when size is turned into class attribute
        glDisable(GL_LIGHTING); // FixMe: check if lighting is enabled
        glBegin(GL_POINTS);
            glColor4fv(fColor);
            glVertex4dv(position.VetXYZW());
        glEnd();
        glEnable(GL_LIGHTING);
    }
    return true;
#else
    return false;
#endif

}
