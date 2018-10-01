/// \file snoperator.h
/// \brief Header file for V-ART class "SnOperator".
/// \version $Revision: 1.2 $

// This abstract class defines an interface. There is no implementation.

#ifndef VART_SNOPERATOR_H
#define VART_SNOPERATOR_H

namespace VART {
    class SceneNode;
/// \class SNOperator snoperator.h
/// \brief Objects that process scene nodes.
///
/// A scene node operator is an object used to process scene nodes when traversing all (or part of)
/// the scene graph (see SceneNode::TraverseDepthFirst and SceneNode::TraverseBreadthFirst).
/// In order to use a scene node operator, derive the class, implementing the OperateOn method.
/// The new derived class can be used in one of the traversal methods.
    class SNOperator
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            //SnOperator();
            virtual void OperateOn(const SceneNode* nodePtr) = 0;
            virtual ~SNOperator() {}
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
    }; // end class declaration
} // end namespace

#endif
