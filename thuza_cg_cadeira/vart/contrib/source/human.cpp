/// \file human.cpp
/// \brief Implementation file for V-ART class "Human" (contrib).
/// \version $Revision: 1.5 $

#include "vart/contrib/human.h"
#include "vart/xmlscene.h"
#include "vart/xmljointaction.h"
#include "vart/collector.h"
#include "vart/action.h"
#include "vart/meshobject.h"
#include "vart/dot.h"
#include "vart/snlocator.h"

#include <iostream>
#include <cassert>

using namespace std;

// Class StepManager ===============================================================================

VART::Human::StepManager::StepManager(Human* human) : humanPtr(human), steppingRight(true)
{
}

//virtual
void VART::Human::StepManager::Activate()
// Change moving leg
{
    if (steppingRight)
    {
        // The right foot was moving, make it stick to ground
        humanPtr->StickRightFoot();
        humanPtr->moveLeftLegPtr->Activate();
    }
    else
    {
        // The left foot was moving, make it stick to ground
        humanPtr->StickLeftFoot();
        humanPtr->moveRightLegPtr->Activate();
    }
    steppingRight = !steppingRight;
}

// Class Human =====================================================================================

VART::Human::Human() : stickPositionPtr(NULL), hipJointPtr(NULL), pathToStickPosPtr(NULL),
                       moveLeftLegPtr(NULL), moveRightLegPtr(NULL), restPtr(NULL), breathePtr(NULL),
                       stepManager(this), forward(0,0,1,0), maxStepRotation(0.007), isWalking(false)
{
    //~ position.MakeIdentity();
    position.SetDescription("position");
    //~ adjustment.MakeIdentity();
    adjustment.SetDescription("adjustment");
    //~ orientation.MakeXRotation(-1.570796327);
    orientation.SetDescription("orientation");
    adjustment.AddChild(position);
    position.AddChild(orientation);
    // Note that general human hierarchy is:
    // adjustment -> position -> orientation -> hipJoint
}

VART::Human::Human(VART::Human& human) : stepManager(this), forward(human.forward),
                                         maxStepRotation(human.maxStepRotation),
                                         isWalking(human.isWalking)
{
    SceneNode* rootJointPtr;

    position.CopyMatrix(human.position);
    orientation.CopyMatrix(human.orientation);
    rootJointPtr = ( *human.childList.begin() )->RecursiveCopy();
    AddChild(*rootJointPtr);
    SetDescription(human.GetDescription());
}

VART::Human::~Human()
{
}

float VART::Human::AngleToPosition(const Point4D& pos, float* totalAngle) const
// Compute Angle to a position (pos) in World coordinates
{
    static const float delta = 3.5; // distances smaller than delta are to small to make sharp turns

    Point4D direction = pos - Position();
    direction.SetY(0);
    float stepAngle = maxStepRotation;
    if (direction.Length() < delta) // if too close to destination, make only a small adjustment
        stepAngle *= 0.01;
    float angle = direction.GenericAngleTo(forward);
    // Compute the Y coordinate of the cross product (we are working on world coordinates)
    // between forward and direction. If it is negative, the angle is also negative.
    if ((forward.GetZ()*direction.GetX() - forward.GetX()*direction.GetZ()) < 0)
    {   // Negative angle
        if (totalAngle)
            *totalAngle = -angle;
        if (angle > stepAngle)
            return -stepAngle;
        else
            return -angle;
    }
    else
    {   // Positive angle
        if (totalAngle)
            *totalAngle = angle;
        if (angle > stepAngle)
            return stepAngle;
        else
            return angle;
    }
}

void VART::Human::ChangeAdjustment(const Point4D& displacement)
{
    Transform translation;
    translation.MakeTranslation(displacement);
    adjustment.CopyMatrix(translation * adjustment);
}

VART::SceneNode* VART::Human::Copy()
{
    assert(false && "VART::Human::Copy() not implemented!");
    return NULL;
}

void VART::Human::ComputeBoundingBox()
{
    assert(false && "VART::Human::ComputeBoundingBox() not implemented!");
}

void VART::Human::StickRightFoot()
{
    // Left foot is backwards. Adjustment brings it forward, so...
    position.CopyMatrix(adjustment * position); // ...save current adjustment to human position
    pathToStickPosPtr = &pathToRFoot;
    stickPositionPtr = &rfFront; // now the right foot sticks to ground, but it is forward ...
                                 // ... adjustment will bring it backwards, so we compute how ...
    Point4D rightFootOffset;     // ... much forward it is
    ComputeLocalStickPosition(&rightFootOffset); // ... and add to position as well
    Transform trans;
    Point4D forward = position * orientation * (rightFootOffset - rfFront);
    forward.SetY(-adjustment.GetData()[13]);
    trans.MakeTranslation(forward);
    position.CopyMatrix(trans * position);
}

void VART::Human::StickLeftFoot()
{
    // Right foot is backwards. Adjustment brings it forward, so...
    position.CopyMatrix(adjustment * position); // ...save current adjustment to human position
    pathToStickPosPtr = &pathToLFoot;
    stickPositionPtr = &lfFront; // now the left foot sticks to ground, but it is forward ...
                                 // ... adjustment will bring it backwards, so we compute how ...
    Point4D leftFootOffset;     // ... much forward it is
    ComputeLocalStickPosition(&leftFootOffset); // ... and add to position as well ...
    Transform trans; // ... as vector, under influence of the human's orientation.
    Point4D forward = position * orientation * (leftFootOffset - lfFront);
    forward.SetY(-adjustment.GetData()[13]);
    trans.MakeTranslation(forward);
    position.CopyMatrix(trans * position);
}

//virtual
void VART::Human::TraverseDepthFirst(SNOperator* operatorPtr)
{
    position.TraverseDepthFirst(operatorPtr);
    SceneNode::TraverseDepthFirst(operatorPtr);
}

// virtual
void VART::Human::LocateDepthFirst(SNLocator* locatorPtr) const
{
    // FixMe: Not sure what to do here.
    orientation.LocateDepthFirst(locatorPtr);
    if (locatorPtr->NotFinished())
        SceneNode::LocateDepthFirst(locatorPtr);
}

//virtual
void VART::Human::DrawForPicking() const
{
    glLoadName(pickName); // store human pickName for hierarchy information
    glPushName(pickName); // push, saving previous name
    adjustment.DrawForPicking();
    glPopName();
}

bool VART::Human::LoadFromFile(const string& fileName)
{
    XmlScene scene;
    bool result = scene.LoadFromFile(fileName);
    if (result) // if no read errors
    {
        // Detach human's root joint from local scene and add it to self
        hipJointPtr = dynamic_cast<Joint*>(scene.GetObjects().front());
        orientation.AddChild(*hipJointPtr);
        scene.Unreference(hipJointPtr);

        // Find smaller vertex on front right foot
        MeshObject* rightFootFingersMeshPtr =
                       dynamic_cast<MeshObject*>(hipJointPtr->FindChildByName("R_footfingersMesh"));
        assert(rightFootFingersMeshPtr != NULL);
        hipJointPtr->FindPathTo(rightFootFingersMeshPtr, &pathToRFoot);
        Point4D::xWeight = 0.1;
        Point4D::yWeight = 3;
        Point4D::zWeight = 2;
        Point4D::operatorLess = &Point4D::WeightedLess;
        rightFootFingersMeshPtr->SmallerVertex(&rfFront); // Compute right foot front
        ResetPosition();

        // Find smaller vertex on front left foot
        MeshObject* leftFootFingersMeshPtr =
                       dynamic_cast<MeshObject*>(hipJointPtr->FindChildByName("L_footfingersMesh"));
        assert(leftFootFingersMeshPtr != NULL);
        hipJointPtr->FindPathTo(leftFootFingersMeshPtr, &pathToLFoot);
        Point4D::xWeight = -0.1;
        Point4D::yWeight = 3;
        Point4D::zWeight = 2;
        leftFootFingersMeshPtr->SmallerVertex(&lfFront); // Compute left foot front
        Point4D::operatorLess = &Point4D::LexicographicalLess;
        Dot* leftFootFrontDotPtr = new Dot(lfFront); // debug only
        leftFootFrontDotPtr->color = Color::RED();
        leftFootFingersMeshPtr->AddChild(*leftFootFrontDotPtr);
        stickPositionPtr = &lfFront;
        pathToStickPosPtr = &pathToLFoot;

        // Load Rest Action
        restPtr = LoadAction("rest.xml");
        if (restPtr)
        {
            restPtr->SetPriority(1);
            restPtr->Activate();
            //~ restPtr->SetSpeed(3);
        }
        // Load Breeth Action
        breathePtr = LoadAction("breathe.xml");
        if (breathePtr)
        {
            breathePtr->SetPriority(2);
            breathePtr->Activate();
        }
    }
    return result;
}

//virtual
bool VART::Human::DrawInstanceOGL() const
{
#ifdef VART_OGL
    return adjustment.DrawOGL();
#else
    return false;
#endif
}

void VART::Human::ActivateBreatheAction(bool status)
{
    if (breathePtr) // if breathePtr != NULL
    {
        if (status) // if status == true
            breathePtr->Activate();
        else
            breathePtr->Deactivate();
    }
}

void VART::Human::ActivateRestAction(bool status)
{
    if (restPtr) // if restPtr != NULL
    {
        if (status) // if status == true
            restPtr->Activate();
        else
            restPtr->Deactivate();
    }
}

void VART::Human::Move(const Point4D& offset)
{
    Transform translation;
    translation.MakeTranslation(offset);
    position.CopyMatrix(translation * position);
}

void VART::Human::RotateOnY(float radians)
{
    // Modify the position transform
    Transform rotation;
    Point4D rotCenter;
    ComputeStickPosition(&rotCenter);
    rotation.MakeRotation(rotCenter, Point4D::Y(), radians);
    position.CopyMatrix(rotation * position);
    // Modify the forward vector
    rotation.ApplyTo(&forward);
}

bool VART::Human::ReachedDestiantion() const
{
    static const float delta = 1.4;
    Point4D location = adjustment * position * orientation * Point4D::ORIGIN();
    Point4D distanceVector = destination - location;
    //cout << "From " << location << " to " << destination << "\n";
    distanceVector.SetY(0);
    float distance = abs(distanceVector.Length());
    //~ cout << "distance: " << distance << "\n";
    return (distance < delta);
}

void VART::Human::ResetPosition()
{
    orientation.MakeXRotation(-1.5707963267948966192313216916398);
    Transform translation;
    translation.MakeTranslation(0, -rfFront.GetZ(), 0); // Floor is on World Y=0
    orientation.CopyMatrix(translation * orientation);
    position.MakeIdentity();
    adjustment.MakeIdentity();
}

VART::Point4D VART::Human::Position() const
{
    return adjustment * position * orientation * Point4D::ORIGIN();
}

void VART::Human::PutIntoScene(Scene* scenePtr)
{
    scenePtr->AddObject(this);
}

VART::XmlJointAction* VART::Human::LoadAction(const std::string& fileName)
{
    assert(hipJointPtr != NULL);
    XmlJointAction* actionPtr = new XmlJointAction;
    if (actionPtr->LoadFromFile(fileName, *hipJointPtr))
    {
        cerr << "Human::LoadAction (" << fileName << ") OK" << endl;
        actionList.push_back(actionPtr);
        if (fileName == "right_front.xml")
        {
            moveRightLegPtr = actionPtr;
            actionPtr->SetPriority(50);
        }
        if (fileName == "left_front.xml")
        {
            moveLeftLegPtr = actionPtr;
            actionPtr->SetPriority(50);
        }
        return actionPtr;
    }
    else
    {
        cerr << "Human::LoadAction (" << fileName << ") FAILED!" << endl;
        delete actionPtr;
        return NULL;
    }
}

void VART::Human::ComputeLocalStickPosition(Point4D* resultPtr)
// Returns the stick position, on Local Coordinates, ignoring human position.
{
    Transform transform;
    pathToStickPosPtr->GetTransform(&transform);
    *resultPtr = transform * (*stickPositionPtr);
}

void VART::Human::ComputeStickPosition(Point4D* resultPtr)
// Return the real stick position, on World Coordinates.
{
    ComputeLocalStickPosition(resultPtr);
    *resultPtr = adjustment * position * orientation * *resultPtr;
}

void VART::Human::AdjustPosition()
{
    Point4D stickPosition;
    ComputeLocalStickPosition(&stickPosition);
    // Compute the vector from stickPosition to *stickPositionPtr, i.e.: the displacement
    // vetor of the stick position due to current articulations state. Then put the vector
    // in World Coordinates.
    adjustment.MakeTranslation(position * orientation * (*stickPositionPtr - stickPosition));
}

void VART::Human::ComputeTransform(Transform* transPtr)
{
    transPtr->CopyMatrix(adjustment * position * orientation);
}

void VART::Human::Walk(bool status)
{
    if (status)
    { // start walking
        StickLeftFoot();
        cout << "Start walking" << endl;
        moveLeftLegPtr->callbackPtr = &stepManager;
        moveRightLegPtr->callbackPtr = &stepManager;
        moveRightLegPtr->Activate();
        cout << "stick position: " << *stickPositionPtr << endl;
        restPtr->Deactivate();
        isWalking = true;
    }
    else
    { // stop walking
        if (moveLeftLegPtr->IsActive())
            StickLeftFoot();
        else
            StickRightFoot();
        cout << "Stop walking" << endl;
        moveLeftLegPtr->callbackPtr = NULL;
        moveLeftLegPtr->Deactivate();
        moveRightLegPtr->callbackPtr = NULL;
        moveRightLegPtr->Deactivate();
        restPtr->Activate();
        isWalking = false;
    }
}

bool VART::Human::WalkTo(const Point4D& dest, const Point4D& orientation)
// FixMe: orientation is currently being ignored
{
    destination = dest;
    float angle = AngleToPosition(dest, NULL);
    RotateOnY(angle);
    return true; // FixMe, check if sucessfull
}

void VART::Human::ModifyActions(DMModifier& modifier)
{
    list<JointAction*>::iterator iter = actionList.begin();
    for(; iter != actionList.end(); ++iter)
    {
        (*iter)->ModifyDofMovers(modifier);
    }
}
