/// \file polyaxialjoint.cpp
/// \brief Implementation file for V-ART class "PolyaxialJoint".
/// \version $Revision: 1.2 $

#include "vart/polyaxialjoint.h"
#include <cassert>

using namespace std;

VART::PolyaxialJoint::PolyaxialJoint()
{
}

VART::PolyaxialJoint::PolyaxialJoint(const VART::PolyaxialJoint& joint)
{
    this->operator=(joint);
}

VART::PolyaxialJoint& VART::PolyaxialJoint::operator=(const VART::PolyaxialJoint& joint)
{
    this->Joint::operator=(joint);
    return *this;
}

VART::SceneNode * VART::PolyaxialJoint::Copy()
{
    VART::Joint * copy;

    copy = new VART::PolyaxialJoint(*this);
    copy->CopyDofListFrom(*this);
    return copy;
}

void VART::PolyaxialJoint::SetFlexionTo(float value) {
    VART::Dof* ptrFirstDof = dofList.front();
    ptrFirstDof->MoveTo(value);
}

void VART::PolyaxialJoint::SetAductionTo(float value) {
    list<VART::Dof*>::iterator iter = dofList.begin();
    ++iter;
    VART::Dof* ptrSecondDof = *iter;
    ptrSecondDof->MoveTo(value);
}

void VART::PolyaxialJoint::SetTwistTo(float value) {
    list<VART::Dof*>::iterator iter = dofList.begin();
    ++iter;
    ++iter;
    VART::Dof* ptrThirdDof = *iter;
    ptrThirdDof->MoveTo(value);
}

void VART::PolyaxialJoint::Adduct(float variance)
{
    assert(GetNumDofs() == 3);
    list<VART::Dof*>::iterator iter = dofList.begin();
    ++iter;
    VART::Dof* ptrSecondDof = *iter;
    ptrSecondDof->Move(variance);
}

void VART::PolyaxialJoint::Twist(float variance)
{
    assert(GetNumDofs() == 3);
    list<VART::Dof*>::iterator iter = dofList.begin();
    ++iter;
    ++iter;
    VART::Dof* ptrSecondDof = *iter;
    ptrSecondDof->Move(variance);
}

void VART::PolyaxialJoint::AddDof(VART::Dof* dof) {
    assert (GetNumDofs() < 3);
    VART::Joint::AddDof(dof);
    // If the third DOF was added, the joint should be made ready for use.
    if (GetNumDofs() == 3)
        MakeLim();
}
