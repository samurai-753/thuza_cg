/// \file joint.cpp
/// \brief Implementation file for V-ART class "Joint".
/// \version $Revision: 1.7 $

#include <algorithm>
#include <cassert>
#include "vart/joint.h"
#include "vart/dof.h"

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/gl.h>
#endif

using namespace std;

VART::Joint::Joint()
{
}

VART::Joint::Joint(const Joint& j)
{
    this->operator=(j);
}

VART::SceneNode * VART::Joint::Copy()
{
    VART::Joint * copy;
    
    copy = new VART::Joint(*this);
    copy->CopyDofListFrom(*this);
    return copy;
}

void VART::Joint::CopyDofListFrom( VART::Joint& joint )
{
    std::list<Dof*>::iterator iter;
    VART::Dof* dof;

    dofList.clear();
    for( iter = joint.dofList.begin(); iter != joint.dofList.end(); iter++ )
    {
        dof = new VART::Dof(**iter);
        dof->SetOwnerJoint( this );
        AddDof( dof );
    }
}

VART::Joint::~Joint()
{
    // Deallocate all Dofs marked as autoDelete
    list<VART::Dof*>::iterator iter;
    for (iter = dofList.begin(); iter != dofList.end(); ++iter)
    {
        if ((*iter)->autoDelete)
            delete *iter;
    }
}

const VART::Joint& VART::Joint::operator=(const Joint& j)
{
    this->Transform::operator=(j);
    dofList = j.dofList;
    return *this;
}

VART::Dof* VART::Joint::AddDof(const Point4D& vec, const Point4D& pos, float min, float max)
{
    Dof* dofPtr = new Dof(vec, pos, min, max);
    dofPtr->SetOwnerJoint(this);
    dofList.push_back(dofPtr);
    return dofPtr;
}

void VART::Joint::AddDof(VART::Dof* dof)
{
    dofList.push_back(dof);
    dof->SetOwnerJoint(this);
}

void VART::Joint::MakeLim()
// In visual mode, the lim is not really used
{
// LIM = ...DOF3 * DOF2 * DOF1
    list<VART::Dof*>::reverse_iterator iter = dofList.rbegin();

    // Copy DOF1's matrix to this object
    (*iter)->GetLim(this);
    ++iter;
    while (iter != dofList.rend())
    {
        // this = dof * this
        (*iter)->ApplyTransformTo(this);
        ++iter;
    }
}

const VART::Dof& VART::Joint::GetDof(DofID dof) const
{
    assert(static_cast<int>(dofList.size()) > dof);
    list<VART::Dof*>::const_iterator iter = dofList.begin();
    int position = 0;
    while (position < dof)
    {
        ++position;
        ++iter;
    }
    return **iter;
}

void VART::Joint::GetDofs(std::list<Dof*>* dofListPtr)
// Warning: this method exposes the dofs of a joint. It has been created for IKChain's constructor.
// An IK chain is a sequence of DOFs that need to need in order to make an articulated object reach
// a certain state, therefore, DOFs must not be constant. I'm not sure if there is a better way to
// deal with encapsulation of the joint class in this case (perhaps making it a friend of IKChain?).
{
    list<Dof*>::iterator iter = dofList.begin();
    while (iter != dofList.end())
    {
        dofListPtr->push_back(*iter);
        ++iter;
    }
}

VART::Joint::DofID VART::Joint::GetDofID(const Dof* dofPtr) const
{
    list<Dof*>::const_iterator iter = dofList.begin();
    unsigned int dofIndex = 0;
    
    while ((iter != dofList.end()) && (*iter != dofPtr))
    {
        ++iter;
        ++dofIndex;
    }
    assert(iter != dofList.end());
    return static_cast<DofID>(dofIndex);
}

void VART::Joint::SetAtRest()
{
    list<VART::Dof*>::iterator iter;
    for(iter = dofList.begin(); iter != dofList.end(); ++iter)
        (*iter)->Rest();
}

bool VART::Joint::MoveDof(DofID dof, float variance)
{
    unsigned int dofNum = static_cast<unsigned int>(dof);

    if (dofNum < dofList.size())
    {
        list<VART::Dof*>::iterator iter = dofList.begin();
        while (dofNum > 0)
        {
            ++iter;
            --dofNum;
        }
        (*iter)->Move(variance);
        return true;
    }
    else
        return false;
}

bool VART::Joint::HasDof(DofID dof)
{
    return static_cast<unsigned int>(dof) < dofList.size();
}

#ifdef VISUAL_JOINTS
bool VART::Joint::DrawOGL() const
{
#ifdef VART_OGL
    bool result = true;
    list<VART::SceneNode*>::const_iterator iter;
    list<VART::Dof*>::const_iterator dofIter;
    int i = 0;

    glPushMatrix();

    for (dofIter = dofList.begin(); dofIter != dofList.end(); ++dofIter)
    {
        glMultMatrixd((*dofIter)->GetLim().GetData());
        GetMaterial(i).DrawOGL();
        (*dofIter)->DrawInstanceOGL();
        ++i;
    }
    for (iter = childList.begin(); iter != childList.end(); ++iter)
        result &= (*iter)->DrawOGL();
    glPopMatrix();
    return result;
#else
    return false;
#endif // VART_OGL
}

const VART::Material& VART::Joint::GetMaterial(int num)
{
    static VART::Material red(VART::Color::RED());
    static VART::Material green(VART::Color::GREEN());
    static VART::Material blue(VART::Color::BLUE());
    switch (num)
    {
        case 0:
            return red;
            break;
        case 1:
            return green;
            break;
        default:
            return blue;
    }
}
#endif // VISUAL_JOINTS

void VART::Joint::XmlPrintOn(ostream& os, unsigned int indent) const
// virtual method
{
    list<Dof*>::const_iterator dofIter = dofList.begin();
    list<SceneNode*>::const_iterator iter = childList.begin();
    string indentStr(indent,' ');

    os << indentStr << "<joint description=\"" << description << "\" type=\"";
    switch (GetNumDofs())
    {
        case 1:
            os << "uniaxial";
            break;
        case 2:
            os << "biaxial";
            break;
        default:
            os << "poliaxial";
    }
    os << "\">\n";
    while (dofIter != dofList.end())
    {
        (*dofIter)->XmlPrintOn(os, indent+2);
        ++dofIter;
    }
    if (recursivePrinting)
        while (iter != childList.end())
        {
            (*iter)->XmlPrintOn(os, indent+2);
            ++iter;
        }
    os << indentStr << "</joint>\n";
    os << flush;
}

ostream& VART::operator<<(ostream& output, const VART::Joint::DofID& dofId)
{
    switch (dofId)
    {
        case VART::Joint::FLEXION:
            output << "FLEXION";
            break;
        case VART::Joint::ADDUCTION:
            output << "ADDUCTION";
            break;
        default:
            output << "TWIST";
            break;
    }
    return output;
}

istream& VART::operator>>(istream& input, VART::Joint::DofID& dofId)
{
    string buffer;
    input >> buffer;
    if (buffer == "FLEXION")
        dofId = VART::Joint::FLEXION;
    else
    {
        if (buffer == "ADDUCTION")
            dofId = VART::Joint::ADDUCTION;
        else
        {
            if (buffer == "TWIST")
                dofId = VART::Joint::TWIST;
            else
                input.setstate(ios_base::failbit);
        }
    }
    return input;
}
