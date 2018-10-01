/// \file scene.cpp
/// \brief Implementation file for V-ART class "Scene".
/// \version $Revision: 1.12 $

#include "vart/scene.h"
#include "vart/graphicobj.h"
#include "vart/transform.h"
#include "vart/picknamelocator.h"

#include <cassert>
#ifdef VART_OGL
#include <GL/gl.h>
#endif

using namespace std;

VART::Scene::Scene() : background(VART::Color::BLACK()), currentCamera(cameras.end())
{
    bBox.SetColor(VART::Color::WHITE());
}

VART::Scene::~Scene() {
    list<VART::SceneNode*>::const_iterator objItr;
    list<const VART::Light*>::const_iterator lightItr;

    // Recursively delete children
    for (objItr = objects.begin(); objItr != objects.end(); ++objItr)
    {
        (*objItr)->AutoDeleteChildren();
        if ((*objItr)->autoDelete)
            delete (*objItr);
    }
    // Delete lights
    for (lightItr = lights.begin(); lightItr != lights.end(); ++lightItr)
    {
        if ((*lightItr)->autoDelete)
            delete (*lightItr);
    }
}

list<const VART::Light*> VART::Scene::GetLights() {
    return lights;
}

list<VART::Camera*> VART::Scene::GetCameras() {
    return cameras;
}

list<VART::SceneNode*> VART::Scene::GetObjects() {
    return objects;
}

void VART::Scene::AddLight(VART::Light* newLight) {
    lights.push_back(newLight);
}

void VART::Scene::AddLight(const VART::Light& newLight) {
    VART::Light* lightPtr = new VART::Light(newLight);
    lightPtr->autoDelete = true;
    lights.push_back(lightPtr);
}

void VART::Scene::AddCamera(VART::Camera* newCamera) {
    cameras.push_back( newCamera );
    if (currentCamera == cameras.end())
        currentCamera = cameras.begin();
}

void VART::Scene::AddObject( VART::SceneNode* newObjectPtr ) {
    objects.push_back( newObjectPtr );
}

void VART::Scene::Unreference(const SceneNode* sceneNodePtr)
{
    list<SceneNode*>::iterator iter = objects.begin();
    bool unfinished = true;

    while (unfinished)
    {
        if (*iter == sceneNodePtr)
        {
            objects.erase(iter);
            unfinished = false;
        }
        else
        {
            ++iter;
            unfinished = (iter != objects.end());
        }
    }
}

const VART::Light* VART::Scene::GetLight(const string& lightName) {
    list<const VART::Light*>::const_iterator iter;
    for (iter = lights.begin(); iter != lights.end(); ++iter) {
        if( (*iter)->GetDescription() == lightName ) {
            return (*iter);
        }
    }
    return NULL;
}

const VART::Camera* VART::Scene::GetCamera(const string& cameraName) {
    list<VART::Camera*>::const_iterator iter;
    for (iter = cameras.begin(); iter != cameras.end(); ++iter) {
        if( (*iter)->GetDescription() == cameraName ) {
            return (*iter);
        }
    }
    return NULL;
}

void VART::Scene::SetCamera(const string& cameraName) {
    list<Camera*>::const_iterator iter;
    for (iter = cameras.begin(); iter != cameras.end(); ++iter) {
        if( (*iter)->GetDescription() == cameraName ) {
            currentCamera = iter;
        }
    }
}

VART::SceneNode* VART::Scene::GetObject(const string& objectName) const {
    list<VART::SceneNode*>::const_iterator iter;

    assert(!objects.empty());
    for (iter = objects.begin(); iter != objects.end(); ++iter) {
        //cout << objectName << " is " << (*iter)->GetDescription() << "?" << endl;
        if( (*iter)->GetDescription() == objectName ) {
            return (*iter);
        }
    }
    return NULL;
}

// protected
VART::GraphicObj* VART::Scene::GetObject(unsigned int pickName)
// Finds and returns a pointer to object of given pick name
{
    // Create a PickNameLocator
    VART::PickNameLocator finder(pickName);
    list<VART::SceneNode*>::const_iterator iter = objects.begin();
    while ((iter != objects.end()) && (finder.NotFinished()))
    {
        (*iter)->LocateDepthFirst(&finder);
        ++iter;
    }
    // If not found, returns NULL.
    return const_cast<GraphicObj*>(finder.GetResult());
}

VART::SceneNode* VART::Scene::GetObjectRec(const string& objectName) const {
    VART::SceneNode* result;
    list<VART::SceneNode*>::const_iterator iter;
    for (iter = objects.begin(); iter != objects.end(); ++iter) {
        if( (*iter)->GetDescription() == objectName )
            return (*iter);
        else
        {
            result = (*iter)->FindChildByName(objectName);
            if (result)
                return result;
        }
    }
    return NULL;
}

const VART::Color& VART::Scene::GetBackgroundColor() {
    return background;
}

void VART::Scene::SetBackgroundColor(VART::Color color) {
    background = color;
}

// cameraPtr defauts to NULL
bool VART::Scene::DrawOGL(Camera* cameraPtr) const {
#ifdef VART_OGL
    // LookAT
    if (cameraPtr)
        cameraPtr->DrawOGL();
    else
    {
        assert(*currentCamera); // make sure there is a current camera
        (*currentCamera)->DrawOGL();
    }

    // FixMe: Lights need not be drawn every rendering cicle. They are should be drawn
    // by a different method.
    DrawLightsOGL();

    // Draw graphical objects
    list<VART::SceneNode*>::const_iterator iter;
    for (iter = objects.begin(); iter != objects.end(); ++iter)
    {
        (*iter)->DrawOGL();
    }
    if (bBox.visible)
        bBox.DrawInstanceOGL();
    return true;
#else
    return false;
#endif
}
//}

bool VART::Scene::DrawLightsOGL() const {
#ifdef VART_OGL
    unsigned int lightID = 0;
    list<const VART::Light*>::const_iterator iter;
    for (iter = lights.begin(); iter != lights.end(); ++iter)
    {
        (*iter)->DrawOGL(lightID);
        ++lightID;
    }
    return true;
#else
    return false;
#endif
}

VART::Camera* VART::Scene::GetCurrentCamera() const {
    if (currentCamera == cameras.end())
        return NULL;
    else
        return *currentCamera;
}

const VART::Camera* VART::Scene::UseNextCamera() {
    assert(!cameras.empty());
    ++currentCamera;
    if (currentCamera == cameras.end())
        currentCamera = cameras.begin();
    return *currentCamera;
}

const VART::Camera* VART::Scene::UsePreviousCamera() {
    assert(!cameras.empty());
    --currentCamera;
    if (currentCamera == cameras.end())
        --currentCamera;
    return *currentCamera;
}

void VART::Scene::SetCamerasAspectRatio(const std::string& cameraDescription, float newAspectRatio) {
    list<VART::Camera*>::iterator iter;
    for (iter = cameras.begin(); iter != cameras.end(); ++iter) {
        if( (*iter)->GetDescription() == cameraDescription ) {
            (*iter)->SetAspectRatio(newAspectRatio);
            break;
        }
    }
}

void VART::Scene::SetAllCamerasAspectRatio(float newAspectRatio) {
    list<VART::Camera*>::const_iterator iter;
    for (iter = cameras.begin(); iter != cameras.end(); ++iter) {
        (*iter)->SetAspectRatio(newAspectRatio);
    }
}

void VART::Scene::ChangeAllCamerasViewVolume(float horScale, float verScale) {
    list<VART::Camera*>::const_iterator iter = cameras.begin();
    for (; iter != cameras.end(); ++iter) {
        (*iter)->ScaleVisibleVolume(horScale, verScale);
    }
}

bool VART::Scene::ComputeBoundingBox() {
    VART::BoundingBox box;
    bool initBBox = false;
    list<VART::SceneNode*>::const_iterator iter;
    VART::GraphicObj* objPtr;
    VART::Transform* transPtr;

    for (iter = objects.begin(); iter != objects.end(); ++iter) {
        objPtr = dynamic_cast<VART::GraphicObj*>(*iter);
        if (objPtr) { // object is a graphic object
            objPtr->ComputeRecursiveBoundingBox();
            if (initBBox)
                bBox.MergeWith(objPtr->GetRecursiveBoundingBox());
            else {
                bBox.CopyGeometryFrom(objPtr->GetRecursiveBoundingBox());
                initBBox = true;
            }
        }
        else { // object is not a graphic object
            transPtr = dynamic_cast<VART::Transform*>(*iter);
            if (transPtr) { // object is a transform
                if (transPtr->RecursiveBoundingBox(&box)) {
                    if (initBBox)
                        bBox.MergeWith(box);
                    else {
                        bBox.CopyGeometryFrom(box);
                        initBBox = true;
                    }
                }
            }
            // If not a transform, then it must be a light. Ignore it.
        }
    }
    bBox.ProcessCenter();
    return initBBox;
}

//~ void VART::Scene::ComputeBoundingBox() { //old version
    //~ list<VART::SceneNode*>::const_iterator iter = objects.begin();
    //~ const VART::GraphicObj* objPtr;
    //~ bool notInitialized = true;

    //~ // Initialize the bounding box
    //~ for (; (iter != objects.end()) && notInitialized; ++iter) {
        //~ objPtr = dynamic_cast<const VART::GraphicObj*>(*iter);
        //~ if (objPtr) // objPtr != NULL
        //~ {
            //~ bbox = objPtr->GetRecursiveBoundingBox();
            //~ notInitialized = false;
        //~ }
    //~ }

    //~ // Merge the bounding box with other graphical objects
    //~ for (; iter != objects.end(); ++iter) {
        //~ objPtr = dynamic_cast<const VART::GraphicObj*>(*iter);
        //~ if (objPtr) // objPtr != NULL
            //~ bbox.MergeWith(objPtr->GetRecursiveBoundingBox());
    //~ }
//~ }

void VART::Scene::SetBoundingBox(double minX, double minY, double minZ,
                                   double maxX, double maxY, double maxZ) {
    bBox.SetBoundingBox(minX,minY,minZ,maxX,maxY,maxZ);
    bBox.ProcessCenter();
}

void VART::Scene::MakeCameraViewAll()
{
    assert(*currentCamera); // make sure there is a current camera
    ComputeBoundingBox();
    VART::Point4D bBoxCenter = bBox.GetCenter();
    cout << bBox << endl;
    (*currentCamera)->SetTarget(bBoxCenter);
    if ((*currentCamera)->GetProjectionType() == VART::Camera::PERSPECTIVE) {
        double greaterEdgeBox = bBox.GetGreaterEdge();
        (*currentCamera)->SetLocation(bBoxCenter+VART::Point4D(0,greaterEdgeBox*1.2,greaterEdgeBox*1.2,0));
        (*currentCamera)->SetUp(VART::Point4D::Y());
        (*currentCamera)->SetFarPlaneDistance(greaterEdgeBox*2.4);
    }
    else if ((*currentCamera)->GetProjectionType() == VART::Camera::ORTHOGRAPHIC) {
        double edgeY = bBox.GetGreaterY();

        (*currentCamera)->SetLocation(bBoxCenter+VART::Point4D(0,edgeY,0,0));
        (*currentCamera)->SetUp(VART::Point4D::X());
        (*currentCamera)->SetVisibleVolumeHeight( bBox.GetGreaterX()-bBox.GetSmallerX() );
    }
    // FixMe: Not sure what to do with near plane distance
    //~ (*currentCamera)->SetNearPlaneDistance();
}

std::list<VART::SceneNode*> VART::Scene::GetAllSceneJoints()
{
    cerr << "\aWarning: Scene::GetAllSceneJoints is deprecated. See documentation\n";
    list<VART::SceneNode*> nodeList;
    list<VART::SceneNode*>::const_iterator iter;

    for (iter = objects.begin(); iter != objects.end(); ++iter)
        (*iter)->GetNodeTypeList( VART::SceneNode::JOINT, nodeList);

    return nodeList;
}

std::list<VART::SceneNode*> VART::Scene::GetAllSceneTypeObject( VART::SceneNode::TypeID type)
{
    cerr << "\aWarning: Scene::GetAllSceneTypeObject is deprecated. See documentation\n";
    list<VART::SceneNode*> nodeList;
    list<VART::SceneNode*>::const_iterator iter;

    for (iter = objects.begin(); iter != objects.end(); ++iter)
        (*iter)->GetNodeTypeList( type, nodeList);

    return nodeList;
}

void VART::Scene::Pick(int x, int y, list<GraphicObj*>* resultListPtr)
{
#ifdef VART_OGL
    const static int SELECTION_BUFFER_SIZE = 512; // FixMe: find a better place to put this!

    resultListPtr->clear(); // remove old elements from list
    // get viewport origin and extent
    GLint viewport[4]; // viewport coordinates
    glGetIntegerv(GL_VIEWPORT, viewport);
    // set a memory region as OpenGL selection buffer
    GLuint selectBuf[SELECTION_BUFFER_SIZE];
    glSelectBuffer(SELECTION_BUFFER_SIZE, selectBuf);
    // enter selection mode
    glRenderMode(GL_SELECT);
    // initialize name stack
    glInitNames();
    // Save zero at stack, to indicate "no selection".
    glPushName(0);

    // create 5x5 pixel picking region near cursor location
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(static_cast<GLdouble>(x),
                  viewport[3]-static_cast<GLdouble>(y),
                  5.0, 5.0, viewport);
    (*currentCamera)->SetMatrices();

    // "draw" graphical objects
    list<VART::SceneNode*>::const_iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter)
        (*iter)->DrawForPicking();

    glFlush();

    // get hits
    // Each object counts as a hit. For instance, if we select some position where two legs of
    // a humanoid are shown, with the floor as well, 3 hits will be generated: 
    // [floor,[human,leftleg],[human,rightleg]].
    // FixMe: Implement a Pick that handles lists of lists such as [[floor],[human,leftleg]].
    // FixMe: Compute Z coordinate of a hit -- very usefull.
    GLint hits = glRenderMode(GL_RENDER); // number of primites picked
    GLuint* selectBufPtr = selectBuf; // points at first element in buffer
    GraphicObj* pickPtr;
    unsigned int numNames;
    unsigned int nameIndex;
    while (hits > 0) // for each hit record...
    {
        numNames = selectBufPtr[0];
        nameIndex = 3;
        while (numNames > 0) // for each name in hit record...
        {
            GLuint pickName = selectBufPtr[nameIndex];
            // Convert the pick name into a pointer
            pickPtr = GetObject(pickName);
            // Store the pointer in the list of results
            if (pickPtr) // if found...
            {
                resultListPtr->push_back(pickPtr);
            }
            // FixMe: Warn if not found??
            // Prepare for next iteration
            ++nameIndex;
            --numNames;
        }
        selectBufPtr += nameIndex;
        --hits;
    }
#else
    cerr << "Error: Scene::Pick is not implemented for your render engine!" << endl;
#endif
}

void VART::Scene::XmlPrintOn(ostream& os) const
{
    list<VART::SceneNode*>::const_iterator iter;

    os << "<?xml version=\"1.0\"?>\n"
       << "<!DOCTYPE scene SYSTEM \"vartScene.dtd\">\n"
       << "<scene description=\"" << description << "\">\n";
    for (iter = objects.begin(); iter != objects.end(); ++iter)
    {
        os << "  <node>\n";
        (*iter)->XmlPrintOn(os, 4);
        os << "  </node>\n";
    }
    os << "</scene>\n";
}
