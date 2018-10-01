/// \file uniaxialjoint.cpp
/// \brief Implementation file for V-ART class "UniaxialJoint".
/// \version $Revision: 1.4 $

#include "vart/uniaxialjoint.h"
#include <cassert>

using namespace std;

VART::UniaxialJoint::UniaxialJoint()
{
}

VART::UniaxialJoint::UniaxialJoint(const VART::UniaxialJoint& joint)
{
    this->operator=(joint);
}

VART::UniaxialJoint& VART::UniaxialJoint::operator=(const VART::UniaxialJoint& joint)
{
    this->Joint::operator=(joint);
    return *this;
}

VART::SceneNode* VART::UniaxialJoint::Copy()
{
    VART::Joint* copy;

    copy = new VART::UniaxialJoint(*this);
    copy->CopyDofListFrom(*this);
    return copy;
}

VART::UniaxialJoint::~UniaxialJoint()
{
}

void VART::UniaxialJoint::SetFlexionTo(float value)
{
    VART::Dof* ptrFirstDof = dofList.front();
    ptrFirstDof->MoveTo(value);
}

// virtual
VART::Dof* VART::UniaxialJoint::AddDof(const Point4D& vec, const Point4D& pos, float min, float max)
{
    assert(GetNumDofs() == 0);
    Dof* result = Joint::AddDof(vec, pos, min, max);
    MakeLim();
    return result;
}

// virtual
void VART::UniaxialJoint::AddDof(VART::Dof* dof)
{
    assert(GetNumDofs() == 0);
    Joint::AddDof(dof);
    MakeLim();
}
