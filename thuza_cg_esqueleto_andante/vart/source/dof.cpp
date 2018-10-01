/// \file dof.cpp
/// \brief Implementation file for V-ART class "Dof".
/// \version $Revision: 1.6 $

#include "vart/dof.h"
#include "vart/joint.h"
#include "vart/modifier.h"
#include <algorithm>

using namespace std;
#ifdef VISUAL_JOINTS
float VART::Dof::axisSize = 0.5;
#endif
list<VART::Dof*> VART::Dof::instanceList;

VART::Dof::Dof()
{
    ownerJoint = NULL;
    evoluta = NULL;
    rangeModifier = NULL;

    // These initializations are needed in order to make a joint automatically compute
    // its transform when setting dofs. They could be removed if the application programmer
    // is forced to tell when a joint is ready to compute its transform.
    axis.SetXYZW(0,0,1,0);
    position.SetXYZW(0,0,0,1);
    lim.MakeIdentity();

    // FixMe: (by Bruno) Not sure if the folowing initializations are needed...
    minAngle = 0;
    maxAngle = 0;
    currentPosition = 0;
    restPosition = 0;
    instanceList.push_back(this);
}

VART::Dof::Dof(const VART::Dof& dof)
: MemoryObj(dof)
{
    description = dof.description;
    position = dof.position;
    axis = dof.axis;
    evoluta = dof.evoluta; //FixMe: Maybe a new evoluta should be created
    lim = dof.lim;
    minAngle = dof.minAngle;
    maxAngle = dof.maxAngle;
    currentMinAngle = dof.currentMinAngle;
    currentMaxAngle = dof.currentMaxAngle;
    confortMinAngle = dof.confortMinAngle;
    confortMaxAngle = dof.confortMaxAngle;
    rangeModifier = dof.rangeModifier;
    currentPosition = dof.currentPosition;
    restPosition = dof.restPosition;
    ownerJoint = dof.ownerJoint;
    instanceList.push_back(this);
}

VART::Dof::Dof(const VART::Point4D& vec, const VART::Point4D& pos, float min, float max)
{
    ownerJoint = NULL;
    evoluta = NULL;
    rangeModifier = NULL;
    axis = vec;
    position = pos;
    minAngle = min;
    currentMinAngle = min;
    maxAngle = max;
    currentMaxAngle = max;
    currentPosition = (0-min)/(max-min);
    axis.Normalize();
    ComputeLIM();
    instanceList.push_back(this);
}

VART::Dof::~Dof()
{
    // remove itself from list of instances
    list<VART::Dof*>::iterator iter = find(instanceList.begin(), instanceList.end(), this);
    instanceList.erase(iter);
}

VART::Dof& VART::Dof::operator=(const VART::Dof& dof)
{
    description = dof.description;
    position = dof.position;
    axis = dof.axis;
    evoluta = dof.evoluta; //FixMe: Maybe a new evoluta should be created
    lim = dof.lim;
    minAngle = dof.minAngle;
    maxAngle = dof.maxAngle;
    currentMinAngle = dof.currentMinAngle;
    currentMaxAngle = dof.currentMaxAngle;
    confortMinAngle = dof.confortMinAngle;
    confortMaxAngle = dof.confortMaxAngle;
    rangeModifier = dof.rangeModifier;
    currentPosition = dof.currentPosition;
    restPosition = dof.restPosition;
    ownerJoint = dof.ownerJoint;
    return *this;
}

void VART::Dof::Set(const VART::Point4D& vec, const VART::Point4D& pos, float min, float max)
{
    axis = vec;
    position = pos;
    minAngle = min;
    currentMinAngle = min;
    maxAngle = max;
    currentMaxAngle = max;
    // Note (by Bruno): Although not stated anywhere, it seems that when DOFs are
    // created, their current position is that of zero rotation
    currentPosition = (0-min)/(max-min);
    axis.Normalize();
    // After been set, a dof should be ready to draw
    ComputeLIM();
}

void VART::Dof::SetDescription(const string& desc)
{
    description = desc;
}

VART::Point4D VART::Dof::GetAxis() const
{
    return axis;
}

VART::Point4D VART::Dof::GetOrigin() const
{
    VART::Point4D result;
    lim.GetTranslation(&result);
    return result;
}

const VART::Transform& VART::Dof::GetLim() const
{
    return lim;
}

void VART::Dof::GetLim(VART::Transform* resultPtr)
{
    resultPtr->CopyMatrix(lim);
}

//~ VART::Matrix
//~ VART::Dof::GetBim() {
    //~ /// Build BIM.
    //~ axis.vpNormalize();
    //~ VART::Vector3D vx = VART::Vector3D(1, 0, 0);
    //~ VART::Vector3D vy = axis.vpCrossProduct(vx);
    //~ // Verify linearity
    //~ if (vy == VART::Vector3D(0, 0, 0)) {
        //~ vx = VART::Vector3D(0, 0, 1);
        //~ vy = axis.vpCrossProduct(vx);
    //~ }
    //~ vy.vpNormalize();
    //~ vx = vy.vpCrossProduct(axis);
    //~ vx.vpNormalize();
    //~ float           fm[4][4];

    //~ fm[0][0] = vx.vpGetX();
    //~ fm[1][0] = vx.vpGetY();
    //~ fm[2][0] = vx.vpGetZ();
    //~ fm[3][0] = 0;

    //~ fm[0][1] = vy.vpGetX();
    //~ fm[1][1] = vy.vpGetY();
    //~ fm[2][1] = vy.vpGetZ();
    //~ fm[3][1] = 0;

    //~ fm[0][2] = axis.vpGetX();
    //~ fm[1][2] = axis.vpGetY();
    //~ fm[2][2] = axis.vpGetZ();
    //~ fm[3][2] = 0;

    //~ fm[0][3] = 0;
    //~ fm[1][3] = 0;
    //~ fm[2][3] = 0;
    //~ fm[3][3] = 1;

    //~ VART::Matrix lBim = VART::Matrix(fm);
    //~ return lBim;
//~ }

float VART::Dof::GetMin() const
{
    return minAngle;
}

float VART::Dof::GetMax() const
{
    return maxAngle;
}

float VART::Dof::GetCurrentMin() const
{
    float minModif = minAngle;
    if (rangeModifier)
        minModif = rangeModifier->GetMin();
    if (minAngle < minModif)
        return minModif;
    return minAngle;
}

float VART::Dof::GetCurrentMax() const
{
    float maxModif = maxAngle;
    if (rangeModifier)
        maxModif = rangeModifier->GetMax();
    if (maxAngle > maxModif)
        return maxModif;
    return maxAngle;
}

float VART::Dof::GetCurrent() const
{
    return currentPosition;
}

float VART::Dof::GetRest() const
{
    return restPosition;
}

VART::Joint* VART::Dof::GetOwnerJoint() const
{
    return ownerJoint;
}

void VART::Dof::SetAxis(VART::Point4D vec)
{
    axis = vec;
}

void VART::Dof::SetEvoluta(VART::Bezier* evol)
{
    evoluta = evol;
}

// FixMe: There should not exist a "SetLim" because computing it is probably the
// main reason for the class to exist...
void VART::Dof::SetLim(const VART::Transform& t)
{
    lim = t;
}

void VART::Dof::SetMin(float min)
{
    minAngle = min;
}

void VART::Dof::SetMax(float max)
{
    maxAngle = max;
}

void VART::Dof::MoveTo(float pos)
{
    // Avoid values out of range
    if (pos > 1.0) pos = 1.0;
    if (pos < 0.0) pos = 0.0;

    // Find angles by interpolation
    double newAngle = currentMinAngle + pos * (currentMaxAngle - currentMinAngle);
    //double currentAngle = currentMinAngle + currentPosition * (currentMaxAngle - currentMinAngle);

    // Find center of rotation
    VART::Point4D center = position;
    if (evoluta)
    { // Compute "center", if necessary.
        VART::Point4D bezierPoint;
        evoluta->GetPoint(currentPosition, &bezierPoint);
        bezierPoint.SetW(0); // Turn it into translation vector
        center = position + bezierPoint;
    }

    // Update Local Instance Matrix
    currentPosition = pos;
    lim.MakeRotation(center, axis, newAngle);

    // Update external (joint) state
    ownerJoint->MakeLim();
}

void VART::Dof::MoveTo(float pos, unsigned int newPriority)
{
    if (newPriority > priority)
    {
        //~ if (description == "flexthoraxJoint")
            //~ cout << "pos: "<<pos<<" NEW priority: "<<newPriority<<" priority: "<<priority<<endl;
        priority = newPriority;
        MoveTo(pos);
    }
}

void VART::Dof::ComputeLIM()
{
    // Find angle by interpolation
    double angle = currentMinAngle + currentPosition * (currentMaxAngle - currentMinAngle);

    // Find center of rotation
    VART::Point4D center;
    if (evoluta)
    {
        VART::Point4D bezierPoint;
        evoluta->GetPoint(currentPosition, &bezierPoint);
        bezierPoint.SetW(0); // Turn it into translation vector
        center = position + bezierPoint;
    }
    else
        center = position;
    // Update Local Instance Matrix
    lim.MakeRotation(center, axis, angle);
}

void VART::Dof::SetOwnerJoint(VART::Joint* ow)
{
    ownerJoint = ow;
}

void VART::Dof::SetRest(float rest)
{
    restPosition = rest;
}

void VART::Dof::Rest()
{
    MoveTo(restPosition);
}

void VART::Dof::SetRangeModifier(VART::Modifier* m)
{
    rangeModifier = m;
}

VART::Modifier* VART::Dof::GetRangeModifier()
{
    return rangeModifier;
}

void VART::Dof::ApplyTransformTo(VART::Transform* ptrTrans) const
{
    ptrTrans->CopyMatrix(lim * (*ptrTrans));
}

void VART::Dof::Reconfigure(const Point4D& state, const Point4D& target)
{
    // put target in the plane defined by current axis of rotation
    Point4D newTarget = axis.CrossProduct(target.CrossProduct(axis));
    Point4D realState = axis.CrossProduct( state.CrossProduct(axis));
    // find the absolute value of the desired correction
    double radians = acos(newTarget.DotProduct(realState));
    // To find the signal, we check the cross product between state and target. A positive rotation
    // is indicated by having axis (not -axis) as the result.
    // FixMe: != is a strong operator. Use something more flexible.
    if (realState.CrossProduct(newTarget) != axis)
        radians = -radians;
//    cout << "radians: " << radians;
    // find current angle of rotation
    double angle = currentMinAngle + currentPosition * (currentMaxAngle - currentMinAngle);
//    cout << " angle: " << angle;
    double newAngle = angle + radians;
    if (newAngle < currentMinAngle)
        newAngle = currentMinAngle;
    else {
        if (newAngle > currentMaxAngle)
            newAngle = currentMaxAngle;
    }
//    cout << " new angle: " << newAngle << endl;
    // find new configuration
    MoveTo(newAngle/(currentMaxAngle - currentMinAngle));
}

#ifdef VISUAL_JOINTS
bool VART::Dof::DrawInstanceOGL() const
{
#ifdef VART_OGL
    float width = axisSize * 0.04; // width of the axis line
    VART::Transform axisRotation; // a rotation around the axis
    VART::Point4D scaledAxis; // a vector from base to tip
    VART::Point4D baseVector; // rotating this vector on the base, we get base vertices
    VART::Point4D vertexVector[4]; // vertices

    scaledAxis = axis * axisSize;
    if ((1 - axis.DotProduct(VART::Point4D::Y())) < 0.01)
        // axis is too close to Y
        baseVector = VART::Point4D::X() * width;
    else
        // axis is not close to Y
        baseVector = VART::Point4D::Y() * width;
    axisRotation.MakeRotation(axis, 1.57079632679489661923); // set the rotation
    vertexVector[0] = position + baseVector;
    vertexVector[1] = vertexVector[0] + scaledAxis;

    glBegin(GL_QUADS);
    for (int i = 1; i < 5; ++i)
    {
        axisRotation.ApplyTo(&baseVector);
        vertexVector[2] = (position + scaledAxis) + baseVector;
        vertexVector[3] = position + baseVector;

        glVertex4dv(vertexVector[3].VetXYZW());
        glVertex4dv(vertexVector[2].VetXYZW());
        glVertex4dv(vertexVector[1].VetXYZW());
        glVertex4dv(vertexVector[0].VetXYZW());

        vertexVector[0] = vertexVector[3];
        vertexVector[1] = vertexVector[2];
    }
    glEnd();
    return true;
#else
    return false;
#endif // VART_OGL
}
#endif // VISUAL_JOINTS

void VART::Dof::ClearPriorities()
// static method
{
    list<VART::Dof*>::iterator iter;
    for (iter = instanceList.begin(); iter != instanceList.end(); ++iter)
        (*iter)->priority = 0;
}

void VART::Dof::XmlPrintOn(ostream& os, unsigned int indent) const
// virtual method
{
    string indentStr(indent,' ');

    os << indentStr << "<dof description=\"" << description << "\">\n"
       << indentStr << "  <position x=\"" << position.GetX() << "\" y=\"" << position.GetY()
                    << "\" z=\"" << position.GetZ() << "\"/>\n"
       << indentStr << "  <axis x=\"" << axis.GetX() << "\" y=\"" << axis.GetY()
                    << "\" z=\"" << axis.GetZ() << "\"/>\n"
       << indentStr << "  <range min=\"" << minAngle << "\" max=\"" << maxAngle
                    << "\" rest=\"" << restPosition << "\"/>\n"
       << indentStr << "</dof>\n";
    os << flush;
}
