/// \file biaxialjoint.cpp
/// \brief Implementation file for V-ART class "BiaxialJoint".
/// \version $Revision: 1.2 $

#include "vart/biaxialjoint.h"
#include <cassert>

using namespace std;

VART::BiaxialJoint::BiaxialJoint()
{
}

VART::BiaxialJoint::BiaxialJoint(const VART::BiaxialJoint& joint)
{
    this->operator=(joint);
}

VART::BiaxialJoint& VART::BiaxialJoint::operator=(const VART::BiaxialJoint& joint)
{
    this->Joint::operator=(joint);
    return *this;
}

VART::SceneNode * VART::BiaxialJoint::Copy()
{
    VART::Joint * copy;

    copy = new VART::BiaxialJoint(*this);
    copy->CopyDofListFrom(*this);
    return copy;
}

VART::BiaxialJoint::~BiaxialJoint()
{
}

void VART::BiaxialJoint::SetFlexionTo(float value)
{
    VART::Dof* ptrFirstDof = dofList.front();
    ptrFirstDof->MoveTo(value);
}

void VART::BiaxialJoint::SetAductionTo(float value)
{
    cerr << "Warning: VART::BiaxialJoint::SetAductionTo is deprecated.\n";
    SetAdductionTo(value);
}

void VART::BiaxialJoint::SetAdductionTo(float value)
{
    assert(GetNumDofs() == 2);
    list<VART::Dof*>::iterator iter = dofList.begin();
    ++iter;
    VART::Dof* ptrSecondDof = *iter;
    ptrSecondDof->MoveTo(value);
}

void VART::BiaxialJoint::Adduct(float variance)
{
    assert(GetNumDofs() == 2);
    list<VART::Dof*>::iterator iter = dofList.begin();
    ++iter;
    VART::Dof* ptrSecondDof = *iter;
    ptrSecondDof->Move(variance);
}

void VART::BiaxialJoint::AddDof(VART::Dof* dof)
{
    assert(GetNumDofs() < 2);
    VART::Joint::AddDof(dof);
    // If the second DOF was added, the joint should be made ready for use.
    if (GetNumDofs() == 2) MakeLim();
}
