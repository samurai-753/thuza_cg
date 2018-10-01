/// \file picknamelocator.h
/// \brief Header file for V-ART class "PickNameLocator".
/// \version $Revision: 1.1 $

#ifndef VART_PICKNAMELOCATOR_H
#define VART_PICKNAMELOCATOR_H

#include "vart/snlocator.h"

namespace VART {
    class SceneNode;
    class GraphicObj;
/// \class PickNameFinder picknamefinder.h
/// \brief A SNLocator that is used to find an object by its pick name.
///
/// The PickNameFinder is used by internal method Scene::GetObject to search the scene graph
/// when picking objects (see Scene::Pick)
    class PickNameLocator : public SNLocator
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            /// Creates an uninitialized PickNameFinder.
            PickNameLocator(unsigned int name);
            virtual ~PickNameLocator() {}
            virtual void OperateOn(const SceneNode* nodePtr);
            const GraphicObj* GetResult() const { return result; }
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            unsigned int pickName;
            const GraphicObj* result;
    }; // end class declaration
} // end namespace

#endif
