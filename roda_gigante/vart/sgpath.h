/// \file sgpath.h
/// \brief Header file for V-ART class "SGPath".
/// \version $Revision: 1.2 $

#ifndef VART_SGPATH_H
#define VART_SGPATH_H

#include <list>
#include <iostream>

#include "vart/snoperator.h"


namespace VART {
    class Transform;
    class SceneNode;
    class Joint;
/// \class SGPath sgpath.h
/// \brief Scene Graph Path
///
/// A Scene Graph Path is a sequence of scene nodes (see SceneNode) in a scene graph.
/// It is commonly created by SceneNode::FindPathTo methods.
    class SGPath {
        friend std::ostream& operator<<(std::ostream& output, const SGPath& path);
        public:
            SGPath();
            SGPath& operator=(const SGPath& path);
            /// Erases all elements.
            void Clear() { graphPath.clear(); }
            /// Adds a node to the path beginning.
            void PushFront(SceneNode* nodePtr) { graphPath.push_front(nodePtr); }
            /// Combines and returns the multiplication of all transforms in a path.
            void GetTransform(Transform* resultPtr) const;
            /// Returns a pointer to the last joint in the path.
            Joint* PointerToLastJoint();
            /// Returns a pointer to the first node.
            SceneNode* FrontPtr() const;
            /// Process all nodes in path.
            void Traverse(SNOperator* operatorPtr) const;
        protected:
            std::list<SceneNode*> graphPath;
    }; // end class declaration
} // end namespace

#endif  // VART_SGPATH_H
