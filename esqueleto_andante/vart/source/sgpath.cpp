/// \file sgpath.cpp
/// \brief Implementation file for V-ART class "SGPath".
/// \version $Revision: 1.3 $

#include "vart/sgpath.h"
#include "vart/transform.h"
#include "vart/joint.h"

#include <cassert>

using namespace std;

VART::SGPath::SGPath()
{
}

VART::SGPath& VART::SGPath::operator=(const SGPath& path)
{
    graphPath = path.graphPath;
    return *this;
}

void VART::SGPath::GetTransform(Transform* resultPtr) const
{
    Transform* mTrans;

    assert(resultPtr != NULL);
    resultPtr->MakeIdentity();

    // For each node in the path...
    list<VART::SceneNode*>::const_iterator iter = graphPath.begin();
    for(; iter != graphPath.end(); ++iter)
    {
        mTrans = dynamic_cast <VART::Transform*>(*iter);
        // ... if the node is some kind of transform ...
        if (mTrans != NULL)
        {
            // ... then accumulate it.
            resultPtr->CopyMatrix((*resultPtr)*(*mTrans));
        }
    }
}

VART::Joint* VART::SGPath::PointerToLastJoint()
{
    list<VART::SceneNode*>::reverse_iterator rIter = graphPath.rbegin();
    Joint* jointPtr;

    while (rIter != graphPath.rend())
    {
        jointPtr = dynamic_cast<Joint*>(*rIter);
        if (jointPtr != NULL)
            return jointPtr;
        ++rIter;
    }
    return NULL;
}

VART::SceneNode* VART::SGPath::FrontPtr() const
{
    assert(!graphPath.empty());
    return graphPath.front();
}

void VART::SGPath::Traverse(SNOperator* operatorPtr) const
{
    list<VART::SceneNode*>::const_iterator iter = graphPath.begin();
    for(; iter != graphPath.end(); ++iter)
        operatorPtr->OperateOn(*iter);
}

ostream& VART::operator<<(ostream& output, const VART::SGPath& path)
{
    list<VART::SceneNode*>::const_iterator iter = path.graphPath.begin();

    if (iter != path.graphPath.end())
    {
        cout << (*iter)->GetDescription();
        ++iter;
        while (iter != path.graphPath.end())
        {
            output << ", " << (*iter)->GetDescription();
            ++iter;
        }
    }
    return output;
}
