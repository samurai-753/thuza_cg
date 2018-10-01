/// \file graphicobj.cpp
/// \brief Implementation file for V-ART class "GraphicObj".
/// \version $Revision: 1.4 $

#include "vart/graphicobj.h"
#include "vart/transform.h"
#include <cassert>
#include <list>

using namespace std;

VART::GraphicObj::GraphicObj() {
    static unsigned int pickCounter = 0;
    show = true;
    howToShow = FILLED;
    pickName = ++pickCounter;
}

void VART::GraphicObj::Show() {
    show = true;
}

void VART::GraphicObj::Hide() {
    show = false;
}

void VART::GraphicObj::ToggleVisibility() {
    show = !show;
}

void VART::GraphicObj::ToggleRecVisibility() {
    list<VART::SceneNode*>::const_iterator iter;
    VART::GraphicObj* objPtr;
    VART::Transform* transPtr;

    for (iter = childList.begin(); iter != childList.end(); ++iter) {
        objPtr = dynamic_cast<VART::GraphicObj*>(*iter);
        if (objPtr) { // object is a graphic object
            objPtr->ToggleVisibility();
        }
        else { // object is not a graphic object
            transPtr = dynamic_cast<VART::Transform*>(*iter);
            if (transPtr) { // object is a transform
                transPtr->ToggleRecVisibility();
            }
            // If not a transform, then it must be a light. Ignore it.
        }
    }
}

void VART::GraphicObj::ToggleBBoxVisibility() {
    bBox.visible = !bBox.visible;
}

void VART::GraphicObj::ToggleRecBBoxVisibility() {
    recBBox.visible = !recBBox.visible;
}

bool VART::GraphicObj::IsVisible() {
    return show;
}

void VART::GraphicObj::ComputeRecursiveBoundingBox() {
    VART::BoundingBox box;
    list<VART::SceneNode*>::const_iterator iter;
    VART::GraphicObj* objPtr;
    VART::Transform* transPtr;

    recBBox.CopyGeometryFrom(bBox); // start with its own bounding box

    for (iter = childList.begin(); iter != childList.end(); ++iter) {
        objPtr = dynamic_cast<VART::GraphicObj*>(*iter);
        if (objPtr) { // object is a graphic object
            objPtr->ComputeRecursiveBoundingBox();
            recBBox.MergeWith(objPtr->GetRecursiveBoundingBox());
        }
        else { // object is not a graphic object
            transPtr = dynamic_cast<VART::Transform*>(*iter);
            if (transPtr) { // object is a transform
                if (transPtr->RecursiveBoundingBox(&box)) {
                    recBBox.MergeWith(box);
                }
            }
            // If not a transform, then it must be a light. Ignore it.
        }
    }
    recBBox.ProcessCenter();
}

void VART::GraphicObj::DrawForPicking() const {
    list<VART::SceneNode*>::const_iterator iter;

    glLoadName(pickName);
    DrawInstanceOGL();
    for (iter = childList.begin(); iter != childList.end(); ++iter)
        (*iter)->DrawForPicking();
}
