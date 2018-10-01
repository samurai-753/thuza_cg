/// \file scenenode.cpp
/// \brief Implementation file for V-ART class "SceneNode".
/// \version $Revision: 1.9 $

#include "vart/scenenode.h"
#include "vart/joint.h"
#include "vart/meshobject.h"
#include "vart/transform.h"
#include "vart/sgpath.h"
#include "vart/snoperator.h"
#include "vart/snlocator.h"

#include <cassert>
using namespace std;

bool VART::SceneNode::recursivePrinting = true;

VART::SceneNode::SceneNode()
{
}

// FixMe: Implement a recursive copy that works on existing objects.
VART::SceneNode* VART::SceneNode::RecursiveCopy()
{
    cerr << "\aWarning: SceneNode::RecursiveCopy() is deprecated.\n";
    VART::SceneNode * thisCopy;
    std::list<VART::SceneNode*>::iterator iter;

    thisCopy = this->Copy();
    thisCopy->childList.clear();
    for( iter = childList.begin(); iter !=childList.end(); iter++ )
        thisCopy->AddChild( *(*iter)->RecursiveCopy() );
    return thisCopy;
}

VART::SceneNode::~SceneNode()
{
    //~ cout << "VART::SceneNode::~SceneNode(): " << GetDescription() << endl;
}

VART::SceneNode::SceneNode(VART::SceneNode& node)
{
    childList = node.childList;
    description = node.description;
}

VART::SceneNode& VART::SceneNode::operator=(const VART::SceneNode& node)
{
    childList = node.childList;
    description = node.description;
    return *this;
}

void VART::SceneNode::AddChild(VART::SceneNode& child)
{
    childList.push_back(&child);
}

bool VART::SceneNode::DetachChild(SceneNode* childPtr)
{
    assert(childPtr != NULL);
    list<VART::SceneNode*>::iterator iter = childList.begin();
    while (iter != childList.end())
    {
        if ((*iter) ==  childPtr)
        {
            childList.erase(iter);
            return true;
        }
        else
            ++iter;
    }
    return false;
}

bool VART::SceneNode::DrawOGL() const
{
    bool result = DrawInstanceOGL();
    list<VART::SceneNode*>::const_iterator iter = childList.begin();
    for (; iter != childList.end(); ++iter)
        result = (result && (*iter)->DrawOGL());
    return result;
}

void VART::SceneNode::AutoDeleteChildren() const
{
    list<VART::SceneNode*>::const_iterator iter;
    for (iter = childList.begin(); iter != childList.end(); ++iter)
    {
        (*iter)->AutoDeleteChildren();
        if ((*iter)->autoDelete)
            delete (*iter);
    }
}

VART::SceneNode* VART::SceneNode::FindChildByName(const std::string& name) const
{
    list<VART::SceneNode*>::const_iterator iter;
    VART::SceneNode* result;
    for (iter = childList.begin(); iter != childList.end(); ++iter)
    {
        if ((*iter)->GetDescription() == name)
            return *iter;
        else{
            result = (*iter)->FindChildByName(name);
            if (result) return result;
        }
    }
    return NULL;
}

list<VART::SceneNode*> VART::SceneNode::GetChilds()
// deprecated
{
    cerr << "\aWarning: SceneNode::GetChilds() is deprecated.\n";
    return childList;
}

bool VART::SceneNode::FindPathTo(SceneNode* targetPtr, SGPath* resultPtr) const
{
    assert(resultPtr != NULL);
    assert(targetPtr != NULL);
    resultPtr->Clear(); // erase previous elements, if any.
    return RecursiveFindPathTo(targetPtr, resultPtr);
}

bool VART::SceneNode::FindPathTo(const string& targetName, SGPath* resultPtr) const
{
    assert(resultPtr != NULL);
    resultPtr->Clear(); // erase previous elements, if any.
    return RecursiveFindPathTo(targetName, resultPtr);
}

bool VART::SceneNode::RecursiveFindPathTo(SceneNode* targetPtr, SGPath* resultPtr) const
{
    list<VART::SceneNode*>::const_iterator iter;

    if (targetPtr == this)
        return true;
    else
    {
        for (iter = childList.begin(); iter != childList.end(); ++iter)
        {
            if ((*iter)->RecursiveFindPathTo(targetPtr, resultPtr))
            {
                resultPtr->PushFront(*iter);
                return true;
            }
        }
    }
    return false;
}

bool VART::SceneNode::RecursiveFindPathTo(const string& targetName, SGPath* resultPtr) const
{
    list<VART::SceneNode*>::const_iterator iter;

    if (description == targetName)
        return true;
    else
    {
        for (iter = childList.begin(); iter != childList.end(); ++iter)
        {
            if ((*iter)->RecursiveFindPathTo(targetName, resultPtr))
            {
                resultPtr->PushFront(*iter);
                return true;
            }
        }
    }
    return false;
}

// virtual
void VART::SceneNode::TraverseDepthFirst(SNOperator* operatorPtr) const
{
    operatorPtr->OperateOn(this); // process this

    // process children
    list<SceneNode*>::const_iterator iter = childList.begin();
    for (; iter != childList.end(); ++iter)
    {
        (*iter)->TraverseDepthFirst(operatorPtr);
    }
}

// virtual
void VART::SceneNode::TraverseBreadthFirst(SNOperator* operatorPtr) const
{
    list<const SceneNode*> queue;
    const SceneNode* nodePtr;

    queue.push_back(this);
    while (!queue.empty())
    {
        nodePtr = queue.front();
        queue.pop_front();
        operatorPtr->OperateOn(nodePtr);
        for (list<SceneNode*>::const_iterator iter = nodePtr->childList.begin(); 
             iter != nodePtr->childList.end(); ++iter)
            queue.push_back(*iter);
    }
}

// virtual
void VART::SceneNode::LocateDepthFirst(SNLocator* locatorPtr) const
{
    locatorPtr->OperateOn(this); // process this
    if (locatorPtr->NotFinished())
    {
        // process children
        list<SceneNode*>::const_iterator iter = childList.begin();
        for (; locatorPtr->NotFinished() && (iter != childList.end()); ++iter)
        {
            (*iter)->LocateDepthFirst(locatorPtr);
        }
        if (locatorPtr->Finished()) // if target has been found...
        {
            --iter; // iter is beyond target
            locatorPtr->AddNodeToPath(*iter);
        }
    }
}

// virtual
void VART::SceneNode::LocateBreadthFirst(SNLocator* locatorPtr) const
{
    list<const SceneNode*> queue;
    const SceneNode* nodePtr;
    queue.push_back(this);
    while (locatorPtr->NotFinished() && (!queue.empty()))
    {
        nodePtr = queue.front();
        queue.pop_front();
        locatorPtr->OperateOn(nodePtr);
        list<SceneNode*>::const_iterator iter = nodePtr->childList.begin();
        for (; iter != nodePtr->childList.end(); ++iter)
            queue.push_back(*iter);
    }
}

int VART::SceneNode::GetNodeTypeList( TypeID type, std::list<SceneNode*>& nodeList )
// deprecated
{
    list<VART::SceneNode*>::const_iterator iter;
    int i=0;

    cerr << "\aWaring: SceneNode::GetNodeTypeList is deprecated. Please use SceneNode::TraverseDepthFirst.\n";
    switch ( type )
    {
        case JOINT:
            if ( dynamic_cast <VART::Joint*>(this) != NULL )
            {
                nodeList.push_back( this );
                i++;
            }
            break;
        case TRANSFORM:
            if ( dynamic_cast <VART::Transform*>(this) != NULL )
            {
                nodeList.push_back( this );
                i++;
            }
            break;
        case MESH_OBJECT:
            if ( dynamic_cast <VART::MeshObject*>(this) != NULL )
            {
                nodeList.push_back( this );
                i++;
            }
            break;
        default: ; // avoid compiler complaints about cases not handled
    }

    for (iter = childList.begin(); iter != childList.end(); ++iter)
            i = i + (*iter)->GetNodeTypeList( type, nodeList);

    return i;
}

void VART::SceneNode::XmlPrintOn(ostream& os, unsigned int indent) const
// virtual method
{
    list<SceneNode*>::const_iterator iter = childList.begin();
    string indentStr(indent,' ');

    os << indentStr << "Unimplemented XmlPrintOn for " << GetID() << "\n";
    if (recursivePrinting)
        while (iter != childList.end())
        {
            (*iter)->XmlPrintOn(os, indent + 2);
            ++iter;
        }
}
