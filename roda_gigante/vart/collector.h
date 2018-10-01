/// \file collector.h
/// \brief Header file for V-ART class "Collector".
/// \version $Revision: 1.1 $

// This is a template class. There is no implementation file.


#ifndef VART_COLLECTOR_H
#define VART_COLLECTOR_H

#include "vart/snoperator.h"
#include "vart/scenenode.h"
#include <list>
#include <iterator>

namespace VART {
/// \class Collector collector.h
/// \brief A scene node operator that collects nodes of some kind.
///
/// A collector is a kind of scene node operator that collects pointers to nodes of a certain
/// kind when traversing a scene graph.
    template<class T>
    class Collector : public SNOperator, public std::list<const T*>
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            Collector() {};
            virtual ~Collector() {}
            virtual void OperateOn(const SceneNode* nodePtr);
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
    }; // end class declaration
} // end namespace

template <class T>
void VART::Collector<T>::OperateOn(const SceneNode* nodePtr)
{
    const T* castPtr = dynamic_cast<const T*>(nodePtr);
    if (castPtr)
        push_back(castPtr);
}

#endif
