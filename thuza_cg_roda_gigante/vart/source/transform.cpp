/// \file transform.cpp
/// \brief Implementation file for V-ART class "Transform".
/// \version $Revision: 1.12 $

// Internal matrix element organization reference
//             [ matrix[0] matrix[4] matrix[8]  matrix[12] ]
// Transform = [ matrix[1] matrix[5] matrix[9]  matrix[13] ]
//             [ matrix[2] matrix[6] matrix[10] matrix[14] ]
//             [ matrix[3] matrix[7] matrix[11] matrix[15] ]

#include "vart/transform.h"
#include "vart/boundingbox.h"
#include "vart/graphicobj.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/gl.h>
#endif

using namespace std;

VART::Transform::Transform()
{
}

VART::SceneNode * VART::Transform::Copy()
{
    return new VART::Transform(*this);
}

void VART::Transform::SetData(double* data)
{
    int i;

    for (i=0; i<16; i++)
    {
        matrix[i] = (*data);
        data++;
    }
}

VART::Transform::Transform(const VART::Transform &trans)
{
    this->Transform::operator=(trans);
}

void VART::Transform::MakeIdentity()
{
    for (int i=0; i<16; ++i)
        matrix[i] = 0.0;
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0;
}

void VART::Transform::MakeTranslation(const VART::Point4D& translationVector)
{
    MakeIdentity();
    matrix[12] = translationVector.GetX();
    matrix[13] = translationVector.GetY();
    matrix[14] = translationVector.GetZ();
}

void VART::Transform::MakeXRotation(double radians)
{
    MakeIdentity();
    matrix[5] =   cos(radians);
    matrix[9] =  -sin(radians);
    matrix[6] =   sin(radians);
    matrix[10] =  cos(radians);
}

void VART::Transform::MakeYRotation(double radians)
{
    MakeIdentity();
    matrix[0] =   cos(radians);
    matrix[8] =   sin(radians);
    matrix[2] =  -sin(radians);
    matrix[10] =  cos(radians);
}

void VART::Transform::MakeZRotation(double radians)
{
    MakeIdentity();
    matrix[0] =  cos(radians);
    matrix[4] = -sin(radians);
    matrix[1] =  sin(radians);
    matrix[5] =  cos(radians);
}

void VART::Transform::MakeScale(double sX, double sY, double sZ)
{
    MakeIdentity();
    matrix[0] =  sX;
    matrix[5] =  sY;
    matrix[10] = sZ;
}

void VART::Transform::MakeShear(double shX, double shY)
{
    MakeIdentity();
    matrix[8] = shX;
    matrix[9] = shY;
}

VART::Point4D VART::Transform::operator *(const VART::Point4D& point) const
{
    VART::Point4D result;

    return VART::Point4D(  matrix[0]*point.GetX()  + matrix[4]*point.GetY()
                     + matrix[8]*point.GetZ()  + matrix[12]*point.GetW(),
                       matrix[1]*point.GetX()  + matrix[5]*point.GetY()
                     + matrix[9]*point.GetZ()  + matrix[13]*point.GetW(),
                       matrix[2]*point.GetX()  + matrix[6]*point.GetY()
                     + matrix[10]*point.GetZ() + matrix[14]*point.GetW(),
                       matrix[3]*point.GetX()  + matrix[7]*point.GetY()
                     + matrix[11]*point.GetZ() + matrix[15]*point.GetW());
}

VART::Transform VART::Transform::operator*(const VART::Transform &t) const
{
    VART::Transform resultado;
    for (int i=0; i < 16; ++i)
        resultado.matrix[i] =
              matrix[i%4]    *t.matrix[i/4*4]  +matrix[(i%4)+4] *t.matrix[i/4*4+1]
            + matrix[(i%4)+8]*t.matrix[i/4*4+2]+matrix[(i%4)+12]*t.matrix[i/4*4+3];
    return resultado;
}

VART::Transform& VART::Transform::operator=(const VART::Transform& t)
{
    this->SceneNode::operator=(t);
    for (int i=0; i < 16; ++i)
        matrix[i] = t.matrix[i];
    return *this;
}

void VART::Transform::CopyMatrix(const Transform& t)
{
    for (int i=0; i < 16; ++i)
        matrix[i] = t.matrix[i];
}

void VART::Transform::Apply(const Transform& t)
{
    CopyMatrix(t * (*this));
}

void VART::Transform::ApplyTo(VART::Point4D* ptPoint) const
{
    ptPoint->SetXYZW(
        matrix[0]*ptPoint->GetX()
            + matrix[4]*ptPoint->GetY()
            + matrix[8]*ptPoint->GetZ()
            + matrix[12]*ptPoint->GetW(),
        matrix[1]*ptPoint->GetX()
            + matrix[5]*ptPoint->GetY()
            + matrix[9]*ptPoint->GetZ()
            + matrix[13]*ptPoint->GetW(),
        matrix[2]*ptPoint->GetX()
            + matrix[6]*ptPoint->GetY()
            + matrix[10]*ptPoint->GetZ()
            + matrix[14]*ptPoint->GetW(),
        matrix[3]*ptPoint->GetX()
            + matrix[7]*ptPoint->GetY()
            + matrix[11]*ptPoint->GetZ()
            + matrix[15]*ptPoint->GetW()
        );
}

void VART::Transform::MakeRotation(const VART::Point4D& refVec, const float radians)
{
    VART::Point4D projEmY;
    VART::Point4D vetTemp;
    VART::Transform tTemp;
    double anguloY;
    double anguloZ;

    //Decompor refVec
    //Angulo em Y (para levar o vetor ao plano XY)
    if (Zero(refVec.GetZ()))
    {
        anguloY = 0.0;
        this->MakeIdentity();
        vetTemp = refVec;
    }
    else
    {
        // se o vetor nao esta no plano XY...
        projEmY.SetXYZW(refVec.GetX(), 0, refVec.GetZ(), 0);
        projEmY.Normalize();
        if (refVec.GetZ() < 0)
            anguloY = -(projEmY.AngleTo(VART::Point4D(1,0,0,0)));
        else
            anguloY = projEmY.AngleTo(VART::Point4D(1,0,0,0));
        this->MakeYRotation(anguloY);
        vetTemp = (*this) * refVec; //refVec no plano XY
    }
    //Angulo em Z (para levar o vetor ao X)
    if (vetTemp.GetY() < 0)
        anguloZ = vetTemp.AngleTo(VART::Point4D(1,0,0,0));
    else
        anguloZ = -(vetTemp.AngleTo(VART::Point4D(1,0,0,0)));
    tTemp.MakeZRotation(anguloZ);
    this->CopyMatrix(tTemp * (*this));
    //Rodar
    tTemp.MakeXRotation(radians);
    this->CopyMatrix(tTemp * (*this));
    //voltar
    tTemp.MakeZRotation(-anguloZ);
    this->CopyMatrix(tTemp * (*this));
    tTemp.MakeYRotation(-anguloY);
    this->CopyMatrix(tTemp * (*this));
}

void VART::Transform::MakeRotation(const VART::Point4D& refPoint, const VART::Point4D& refVec,
                               const float radians)
{
    VART::Transform tTemp;

    this->MakeTranslation(-refPoint);
    tTemp.MakeRotation(refVec, radians);
    this->CopyMatrix(tTemp * (*this));
    tTemp.MakeTranslation(refPoint);
    this->CopyMatrix(tTemp * (*this));
}

void VART::Transform::GetVectorX(VART::Point4D* result) const
{
    result->SetX(matrix[0]);
    result->SetY(matrix[1]);
    result->SetZ(matrix[2]);
    result->SetW(matrix[3]);
}

void VART::Transform::GetVectorY(VART::Point4D* result) const
{
    result->SetX(matrix[4]);
    result->SetY(matrix[5]);
    result->SetZ(matrix[6]);
    result->SetW(matrix[7]);
}

void VART::Transform::GetVectorZ(VART::Point4D* result) const
{
    result->SetX(matrix[8]);
    result->SetY(matrix[9]);
    result->SetZ(matrix[10]);
    result->SetW(matrix[11]);
}

void VART::Transform::GetTranslation(VART::Point4D* result) const
{
    result->SetX(matrix[12]);
    result->SetY(matrix[13]);
    result->SetZ(matrix[14]);
    result->SetW(matrix[15]);
}

bool VART::Transform::DrawOGL() const
{
#ifdef VART_OGL
    bool result = true;

    glPushMatrix();
    glMultMatrixd(matrix);

    list<VART::SceneNode*>::const_iterator iter = childList.begin();
    for (; iter != childList.end(); ++iter)
        result &= (*iter)->DrawOGL();
    glPopMatrix();
    return result;
#else
    return false;
#endif
}

void VART::Transform::DrawForPicking() const {
#ifdef VART_OGL
    list<VART::SceneNode*>::const_iterator iter;

    glPushMatrix();
    glMultMatrixd(matrix);

    for (iter = childList.begin(); iter != childList.end(); ++iter)
        (*iter)->DrawForPicking();
    glPopMatrix();
#else
    cerr << "Error! Transform::DrawForPicking not implemented for non-OpenGL systems!" << endl;
#endif
}

bool VART::Transform::RecursiveBoundingBox(VART::BoundingBox* bBox) {
// virtual method

// Note: Bounding boxes from children should be transformed first and then merged.
// If merged before transforming, errors will occour because VART::BoundingBox::MergeWith
// expects aligned bounding boxes.

    VART::BoundingBox box; // must not be static because it is used in the recursion
    bool initBBox = false;
    list<VART::SceneNode*>::const_iterator iter;
    VART::GraphicObj* objPtr;
    VART::Transform* transPtr;

    for (iter = childList.begin(); iter != childList.end(); ++iter) {
        objPtr = dynamic_cast<VART::GraphicObj*>(*iter);
        if (objPtr) { // object is a graphic object
            objPtr->ComputeRecursiveBoundingBox();
            box = objPtr->GetRecursiveBoundingBox();
            box.ApplyTransform(*this);
            if (initBBox)
                bBox->MergeWith(box);
            else {
                bBox->CopyGeometryFrom(box);
                initBBox = true;
            }
        }
        else { // object is not a graphic object
            transPtr = dynamic_cast<VART::Transform*>(*iter);
            if (transPtr) { // object is a transform
                if (transPtr->RecursiveBoundingBox(&box)) { // object has graphic descendents
                    box.ApplyTransform(*this);
                    if (initBBox)
                        bBox->MergeWith(box);
                    else {
                        bBox->CopyGeometryFrom(box);
                        initBBox = true;
                    }
                }
            }
            // If not a transform, then it must be a light. Ignore it.
        }
    }
    if (initBBox)
        bBox->ProcessCenter();
    return initBBox;
}

void VART::Transform::ToggleRecVisibility() {
    list<VART::SceneNode*>::const_iterator iter;
    VART::GraphicObj* objPtr;
    VART::Transform* transPtr;

    for (iter = childList.begin(); iter != childList.end(); ++iter) {
        objPtr = dynamic_cast<VART::GraphicObj*>(*iter);
        if (objPtr) { // object is a graphic object
            objPtr->ToggleVisibility();
            objPtr->ToggleRecVisibility();
        }
        else { // object is not a graphic object
            transPtr = dynamic_cast<VART::Transform*>(*iter);
            if (transPtr) { // object is a transform
                transPtr->ToggleRecVisibility();  // object has graphic descendents
            }
            // If not a transform, then it must be a light. Ignore it.
        }
    }
}

#ifndef NDEBUG
bool VART::Transform::HasNaN() const
{
    for (int i=0; i < 16; ++i)
    {
        if (std::isnan(matrix[i]))
            return true;
    }
    return false;
}
#endif

ostream& VART::operator<<(ostream& output, const VART::Transform& t)
{
    int i, j;

    output.setf(ios::showpoint|ios::fixed);
    output.precision(6);
    for (i=0; i<4; ++i)
    {
        for (j=0; j<4; ++j)
            output << setw(12) << t.matrix[i+j*4] << " ";
        output << endl;
    }
    output.unsetf(ios::showpoint|ios::fixed);
    return output;
}
