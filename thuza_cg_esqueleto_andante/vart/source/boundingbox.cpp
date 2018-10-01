/// \file boundingbox.cpp
/// \brief Implementation file for V-ART class "BoundingBox".
/// \version $Revision: 1.6 $

#include "vart/boundingbox.h"
#include "vart/transform.h"

using namespace std;

VART::BoundingBox::BoundingBox() {
    visible = false;
    color = VART::Color::RED();
}

VART::BoundingBox::BoundingBox(double minX, double minY, double minZ,
                             double maxX, double maxY, double maxZ) {
    visible = false;
    smallerX = minX;
    smallerY = minY;
    smallerZ = minZ;
    greaterX = maxX;
    greaterY = maxY;
    greaterZ = maxZ;
    color = VART::Color::RED();
    ProcessCenter();
}

VART::BoundingBox::BoundingBox(const VART::BoundingBox& box) {
    visible = box.visible;
    smallerX = box.smallerX;
    smallerY = box.smallerY;
    smallerZ = box.smallerZ;
    greaterX = box.greaterX;
    greaterY = box.greaterY;
    greaterZ = box.greaterZ;
    center   = box.center;
    color    = box.color;
}

VART::BoundingBox& VART::BoundingBox::operator=(const VART::BoundingBox& box) {
    visible = box.visible;
    smallerX = box.smallerX;
    smallerY = box.smallerY;
    smallerZ = box.smallerZ;
    greaterX = box.greaterX;
    greaterY = box.greaterY;
    greaterZ = box.greaterZ;
    center   = box.center;
    color    = box.color;
    return *this;
}

void VART::BoundingBox::CopyGeometryFrom(const VART::BoundingBox& box) {
    smallerX = box.smallerX;
    smallerY = box.smallerY;
    smallerZ = box.smallerZ;
    greaterX = box.greaterX;
    greaterY = box.greaterY;
    greaterZ = box.greaterZ;
    center   = box.center;
}


void VART::BoundingBox::ProcessCenter() {
    center.SetXYZW((greaterX + smallerX)/2,
                   (greaterY + smallerY)/2,
                   (greaterZ + smallerZ)/2, 1);
}

void VART::BoundingBox::ApplyTransform(const VART::Transform& trans) {
    VART::BoundingBox box = *this;

    VART::Point4D ptoBBox = trans * VART::Point4D(box.GetSmallerX(),box.GetSmallerY(),box.GetSmallerZ());
    this->SetBoundingBox(ptoBBox.GetX(),ptoBBox.GetY(),ptoBBox.GetZ(),ptoBBox.GetX(),ptoBBox.GetY(),ptoBBox.GetZ());
    this->ConditionalUpdate(trans * VART::Point4D(box.GetSmallerX(), box.GetSmallerY(), box.GetGreaterZ()));
    this->ConditionalUpdate(trans * VART::Point4D(box.GetSmallerX(), box.GetGreaterY(), box.GetGreaterZ()));
    this->ConditionalUpdate(trans * VART::Point4D(box.GetSmallerX(), box.GetGreaterY(), box.GetSmallerZ()));
    this->ConditionalUpdate(trans * VART::Point4D(box.GetGreaterX(), box.GetSmallerY(), box.GetSmallerZ()));
    this->ConditionalUpdate(trans * VART::Point4D(box.GetGreaterX(), box.GetSmallerY(), box.GetGreaterZ()));
    this->ConditionalUpdate(trans * VART::Point4D(box.GetGreaterX(), box.GetGreaterY(), box.GetGreaterZ()));
    this->ConditionalUpdate(trans * VART::Point4D(box.GetGreaterX(), box.GetGreaterY(), box.GetSmallerZ()));
    ProcessCenter();
}

double VART::BoundingBox::GetGreaterX() const {
    return greaterX;
}

double VART::BoundingBox::GetGreaterY() const {
    return greaterY;
}

double VART::BoundingBox::GetGreaterZ() const {
    return greaterZ;
}

void VART::BoundingBox::SetGreaterX(double v) {
    greaterX=v;
}

void VART::BoundingBox::SetGreaterY(double v) {
    greaterY=v;
}

void VART::BoundingBox::SetGreaterZ(double v) {
    greaterZ=v;
}

double VART::BoundingBox::GetSmallerX() const {
    return smallerX;
}

double VART::BoundingBox::GetSmallerY() const {
    return smallerY;
}

double VART::BoundingBox::GetSmallerZ() const {
    return smallerZ;
}

void VART::BoundingBox::SetSmallerX(double v) {
    smallerX=v;
}

void VART::BoundingBox::SetSmallerY(double v) {
    smallerY=v;
}

void VART::BoundingBox::SetSmallerZ(double v) {
    smallerZ=v;
}

void VART::BoundingBox::SetBoundingBox(double minX, double minY, double minZ,
                                   double maxX, double maxY, double maxZ) {
    smallerX = minX;
    smallerY = minY;
    smallerZ = minZ;
    greaterX = maxX;
    greaterY = maxY;
    greaterZ = maxZ;
    ProcessCenter();
}

const VART::Point4D& VART::BoundingBox::GetCenter() const {
    return center;
}

void VART::BoundingBox::ConditionalUpdate(double x, double y, double z) {
    if (x < smallerX)
        smallerX = x;
    else {
        if (x > greaterX) greaterX = x;
    }
    if (y < smallerY)
        smallerY = y;
    else {
        if (y > greaterY) greaterY = y;
    }
    if (z < smallerZ)
        smallerZ = z;
    else {
        if (z > greaterZ) greaterZ = z;
    }
}

void VART::BoundingBox::ConditionalUpdate(const VART::Point4D& point) {
    ConditionalUpdate(point.GetX(), point.GetY(), point.GetZ());
}

void VART::BoundingBox::MergeWith(const VART::BoundingBox& box) {
    ConditionalUpdate(box.smallerX, box.smallerY, box.smallerZ);
    ConditionalUpdate(box.greaterX, box.greaterY, box.greaterZ);
}

double VART::BoundingBox::GetGreaterEdge() const {
    double greater = greaterX - smallerX;
    double tmp = greaterY - smallerY;

    if (tmp > greater)
        greater = tmp;
    tmp = greaterZ - smallerZ;
    if (tmp > greater)
        greater = tmp;
    return greater;
}

double VART::BoundingBox::GetSmallerEdge() const {
    double smaller = greaterX - smallerX;
    double tmp = greaterY - smallerY;

    if (tmp < smaller)
        smaller = tmp;
    tmp = greaterZ - smallerZ;
    if (tmp < smaller)
        smaller = tmp;
    return smaller;
}

void VART::BoundingBox::ToggleVisibility() {
    visible = !visible;
}

// Should be called if the bounding box is visible
bool VART::BoundingBox::DrawInstanceOGL() const {
#ifdef VART_OGL
    static float fVec[4];

    glDisable(GL_LIGHTING); // FixMe: check if lighting is enabled
    color.Get(fVec);
    glColor4fv(fVec);
    glBegin (GL_LINE_LOOP);
        glVertex3d (smallerX, greaterY, smallerZ);
        glVertex3d (greaterX, greaterY, smallerZ);
        glVertex3d (greaterX, smallerY, smallerZ);
        glVertex3d (smallerX, smallerY, smallerZ);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3d (smallerX, smallerY, smallerZ);
        glVertex3d (smallerX, smallerY, greaterZ);
        glVertex3d (smallerX, greaterY, greaterZ);
        glVertex3d (smallerX, greaterY, smallerZ);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3d (greaterX, greaterY, greaterZ);
        glVertex3d (smallerX, greaterY, greaterZ);
        glVertex3d (smallerX, smallerY, greaterZ);
        glVertex3d (greaterX, smallerY, greaterZ);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex3d (greaterX, smallerY, smallerZ);
        glVertex3d (greaterX, greaterY, smallerZ);
        glVertex3d (greaterX, greaterY, greaterZ);
        glVertex3d (greaterX, smallerY, greaterZ);
    glEnd();
    glEnable(GL_LIGHTING);
    return true;
#else
    return false;
#endif
}

bool VART::BoundingBox::testAABBAABB(VART::BoundingBox &b)
{
    if (b.greaterX < smallerX)
        return false;
    if (b.smallerX > greaterX)
        return false;
    if (b.greaterZ < smallerZ)
        return false;
    if (b.smallerZ > greaterZ)
        return false;
    if (b.greaterY < smallerY)
        return false;
    if (b.smallerY > greaterY)
        return false;
    return true;
}

bool VART::BoundingBox::testPoint( VART::Point4D p )
{
    if (p.GetX() < smallerX)
        return false;
    if (p.GetX() > greaterX)
        return false;
    if (p.GetZ() < smallerZ)
        return false;
    if (p.GetZ() > greaterZ)
        return false;
    if (p.GetY() < smallerY)
        return false;
    if (p.GetY() > greaterY)
        return false;
    return true;

}

void VART::BoundingBox::CutBBox( const BoundingBox& box )
{
    if (box.greaterX < greaterX)
        greaterX = box.greaterX;
    if (box.smallerX > smallerX)
        smallerX = box.smallerX;
    if (box.greaterZ < greaterZ)
        greaterZ = box.greaterZ;
    if (box.smallerZ > smallerZ)
        smallerZ = box.smallerZ;
    if (box.greaterY < greaterY)
        greaterY = box.greaterY;
    if (box.smallerY > smallerY)
        smallerY = box.smallerY;
}

ostream& VART::operator<<(ostream& output, const VART::BoundingBox& box)
{
    output << "(Greaters: " << box.greaterX << "," << box.greaterY << ","
           << box.greaterZ << " Smallers: " << box.smallerX << ","
           << box.smallerY << "," << box.smallerZ << ")";
    return output;
}
