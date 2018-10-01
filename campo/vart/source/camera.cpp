/// \file camera.cpp
/// \brief Implementation file for V-ART class "Camera".
/// \version $Revision: 1.8 $

#include "vart/camera.h"
#include "vart/transform.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/glu.h>
#endif

//?
#include <iostream>

using namespace std;

VART::Camera::Camera() {
    projectionType = PERSPECTIVE;
    fovY = 60;
    aspectRatio = 1.0f;
    vvLeft = -1.0;
    vvRight = 1.0;
    vvTop = 1.0;
    vvBottom = -1.0;
    nearPlaneDistance = 0.5f;
    farPlaneDistance = 100.0f;

    //~ viewHeight = 0;
    //~ viewWidth = 0;
}

VART::Camera::Camera(const VART::Point4D& position, const VART::Point4D& focus, const VART::Point4D& upVec)
{
    projectionType = PERSPECTIVE;
    fovY = 60;
    vvLeft = -1.0;
    vvRight = 1.0;
    vvTop = 1.0;
    vvBottom = -1.0;
    nearPlaneDistance = 0.5f;
    farPlaneDistance = 100.0f;
    aspectRatio = 1.0f;
    location = position;
    target = focus;
    up = upVec;
    // Make sure "up" is orthogonal to "front" and normalized
    VART::Point4D front = focus - position;
    up.Normalize();
    front.Normalize();
    VART::Point4D left = up.CrossProduct(front);
    up = front.CrossProduct(left);
}

VART::Camera::Camera(const VART::Camera& cam)
{
    description = cam.description;
    projectionType = cam.projectionType;
    vvLeft = cam.vvLeft;
    vvRight = cam.vvRight;
    vvTop = cam.vvTop;
    vvBottom = cam.vvBottom;
    nearPlaneDistance = cam.nearPlaneDistance;
    farPlaneDistance = cam.farPlaneDistance;
    fovY = cam.fovY;
    location = cam.location;
    target = cam.target;
    up = cam.up;
    aspectRatio = cam.aspectRatio;
    //~ viewHeight = cam.viewHeight;
    //~ viewWidth = cam.viewWidth;
    //~ winBottomLeft = cam.winBottomLeft;
    //~ winTopRight = cam.winTopRight;
}

VART::Camera& VART::Camera::operator=(const VART::Camera& cam)
{
    description = cam.description;
    projectionType = cam.projectionType;
    vvLeft = cam.vvLeft;
    vvRight = cam.vvRight;
    vvTop = cam.vvTop;
    vvBottom = cam.vvBottom;
    nearPlaneDistance = cam.nearPlaneDistance;
    farPlaneDistance = cam.farPlaneDistance;
    fovY = cam.fovY;
    location = cam.location;
    target = cam.target;
    up = cam.up;
    aspectRatio = cam.aspectRatio;
    return (*this);
}

void VART::Camera::SetDescription(const string& descriptionValue){
    description = descriptionValue;
}

//~ VART::Point2D VART::Camera::GetWinBottomLeft() const {
    //~ return winBottomLeft;
//~ }

//~ void VART::Camera::SetWinBottomLeft(VART::Point2D winBottomLeftValue){
    //~ winBottomLeft = winBottomLeftValue;
//~ }

// FixMe: This method should be const
//~ void VART::Camera::GetWinTopRight(float &x, float &y) {
    //~ x = winTopRight.vpGetX();
    //~ y = winTopRight.vpGetY();
//~ }

//~ void VART::Camera::SetWinTopRight(float x, float y) {
    //~ winTopRight.vpSetX(x);
    //~ winTopRight.vpSetY(y);
//~ }

//~ VART::Point2D VART::Camera::GetWinTopRight() const {
    //~ return winTopRight;
//~ }

//~ void VART::Camera::SetWinTopRight(VART::Point2D winTopRightValue){
    //~ winTopRight = winTopRightValue;
//~ }

// FixMe: This method should be const
//~ VART::Point2D VART::Camera::GetWinLeftRight() {
    //~ VART::Point2D point(winBottomLeft.vpGetX(), winTopRight.vpGetY());
    //~ return point;
//~ }

//~ void VART::Camera::SetWinLeftRight(VART::Point2D winLeftRightValue){
    //~ winBottomLeft.vpSetY(winLeftRightValue.vpGetX());
    //~ winTopRight.vpSetY(winLeftRightValue.vpGetY());
//~ }

// FixMe: This method should be const
//~ VART::Point2D VART::Camera::GetWinBottomTop() {
    //~ VART::Point2D point(winBottomLeft.vpGetX(), winTopRight.vpGetX());
    //~ return point;
//~ }

//~ void VART::Camera::SetWinBottomTop(VART::Point2D winBottomTopValue){
    //~ winBottomLeft.vpSetX(winBottomTopValue.vpGetX());
    //~ winTopRight.vpSetX(winBottomTopValue.vpGetY());
//~ }

VART::Point4D VART::Camera::GetLocation() const {
    return location;
}

void VART::Camera::SetLocation(const VART::Point4D& locationValue){
    location = locationValue;
}

VART::Point4D VART::Camera::GetTarget() const {
    return target;
}

void VART::Camera::SetTarget(const VART::Point4D& targetValue){
    target = targetValue;
}

VART::Point4D VART::Camera::GetUp() const {
    return up;
}

void VART::Camera::SetUp(const VART::Point4D& upValue){
    up = upValue;
}

float VART::Camera::GetFovY() const {
    return fovY;
}

void VART::Camera::SetFovY(float f) {
    fovY = f;
}

void VART::Camera::LeftVector(Point4D* resultPtr) const {
    VART::Point4D front = target - location;
    front.Normalize();
    *resultPtr = up.CrossProduct(front);
}

void VART::Camera::FrontVector(Point4D* resultPtr) const {
    VART::Point4D front = target - location;
    front.Normalize();
    *resultPtr = front;
}

void VART::Camera::SetVisibleVolumeHeight(double newValue) {
    double halfHeight = newValue / 2;
    double halfWidth = halfHeight * aspectRatio;
    vvTop = halfHeight;
    vvBottom = -halfHeight;
    vvLeft = -halfWidth;
    vvRight = halfWidth;
}

void VART::Camera::ScaleVisibleVolume(float horScale, float verScale) {
    vvTop *= verScale;
    vvBottom *= verScale;
    vvLeft *= horScale;
    vvRight *= horScale;
}

void VART::Camera::YawAroundTarget(float radians) {
    VART::Transform trans;
    trans.MakeRotation(target, up, radians);
    trans.ApplyTo(&location);
}

void VART::Camera::Yaw(float radians) {
    VART::Transform trans;
    trans.MakeRotation(location, up, radians);
    trans.ApplyTo(&target);
}

void VART::Camera::Roll(float radians) {
    VART::Transform trans;
    VART::Point4D front = target - location;

    front.Normalize();
    trans.MakeRotation(location, front, radians);
    trans.ApplyTo(&up);
}

void VART::Camera::PitchAroundTarget(float radians) {
    VART::Transform trans;
    VART::Point4D left;
    VART::Point4D front = target - location;
    front.Normalize();
    left = up.CrossProduct(front);
    trans.MakeRotation(target, left, radians);
    trans.ApplyTo(&location);
    trans.ApplyTo(&up);
}

void VART::Camera::MoveForward(double distance) {
    VART::Transform trans;
    VART::Point4D front = target - location;
    front.Normalize();
    front *= distance;
    trans.MakeTranslation(front);
    trans.ApplyTo(&location);
    trans.ApplyTo(&target);
}

void VART::Camera::MoveSideways(double distance) {
    VART::Transform trans;
    VART::Point4D right;

    trans.MakeRotation(up,-1.5707963267948966192313216916398);
    right = trans * (target - location);

    right.Normalize();
    right *= distance;
    trans.MakeTranslation(right);
    trans.ApplyTo(&location);
    trans.ApplyTo(&target);
}

void VART::Camera::MoveUp(double distance) {
    VART::Transform trans;
    VART::Point4D translation(up);

    translation.Normalize();
    translation *= distance;
    trans.MakeTranslation(translation);
    trans.ApplyTo(&location);
    trans.ApplyTo(&target);
}

bool VART::Camera::DrawOGL() const {
#ifdef VART_OGL
    // It seems that setting the projection matrix at every rendering cycle is a
    // good idea because an application may have multiple cameras and multiple viewers.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (projectionType == PERSPECTIVE)
        gluPerspective(fovY, aspectRatio, nearPlaneDistance, farPlaneDistance);
    else
        glOrtho(vvLeft, vvRight, vvBottom, vvTop, nearPlaneDistance, farPlaneDistance);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(location.GetX(),location.GetY(),location.GetZ(),
              target.GetX(), target.GetY(), target.GetZ(),
              up.GetX(), up.GetY(), up.GetZ());
    return true;
#else
    return false;
#endif
}

void VART::Camera::SetMatrices() const
{
#ifdef VART_OGL
    // It seems that setting the projection matrix at every rendering cycle is a
    // good idea because an application may have multiple cameras and multiple viewers.
    glMatrixMode(GL_PROJECTION);
    if (projectionType == PERSPECTIVE)
        gluPerspective(fovY, aspectRatio, nearPlaneDistance, farPlaneDistance);
    else
        glOrtho(vvLeft, vvRight, vvBottom, vvTop, nearPlaneDistance, farPlaneDistance);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(location.GetX(),location.GetY(),location.GetZ(),
              target.GetX(), target.GetY(), target.GetZ(),
              up.GetX(), up.GetY(), up.GetZ());
#else
    cerr << "Error! Camera::SetMatrices() unimplemented for non OpenGL systems!" << endl;
#endif
}

ostream& VART::operator<<(ostream& output, const VART::Camera& cam)
{
    output << "Camera(" << cam.description << ": pos"<< cam.location << " target" << cam.target
           << " up" << cam.up << " near(" << cam.nearPlaneDistance << ") far(" << cam.farPlaneDistance
           << "))";
    return output;
}
