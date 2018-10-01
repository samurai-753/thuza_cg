/// \file snlocator.h
/// \brief Header file for V-ART class "SNLocator".
/// \version $Revision: 1.1 $

// This abstract class defines an interface. There is no implementation.

#ifndef VART_SNLOCATOR_H
#define VART_SNLOCATOR_H

#include "vart/snoperator.h"
#include "vart/sgpath.h"

namespace VART {
    class SceneNode;
/// \class SNLocator snlocator.h
/// \brief Scene Node Locator -- used to find a particular object in the scene graph.
///
/// A SNLocator (Scene Node Locator) is a special kind of SNOperator that is able to inform whether
/// its task is finished. You should use a locator when you don't want the whole scene graph to
/// be searched, that is, you want the traversal to finish once some criteria is met, such as
/// when searching for a particular scene node.
///
/// It is also able to store scene graph path, so that the user may re-use the path to the target
/// scene node.
    class SNLocator : public SNOperator
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            SNLocator();
            virtual ~SNLocator() {}

            /// \brief Process given node.
            virtual void OperateOn(const SceneNode* nodePtr) = 0;

            /// \brief Indicates whether the traversal has not finished.
            ///
            /// The finishing criterion is usually finding a target node, so this method
            /// indicates whether the search was unsuccessfull.
            bool NotFinished() const { return notFinished; }

            /// \brief Indicates whether the traversal has finished.
            ///
            /// The finishing criterion is usually finding a target node, so this method
            /// indicates whether the search was successfull.
            bool Finished() const { return !notFinished; }

            /// \brief Returns the path to target node.
            ///
            /// The locator itself is not able to build a path to target node because it does not
            /// operates recursively. The resulting path must be built by calls to AddNodeToPath()
            /// on recursive methods such as SceneNode::LocateDepthFirst().
            ///
            /// This method only returns (by reference) the internal path.
            const SGPath& Path() const { return path; }

            /// \brief Adds a node to internal Path.
            ///
            /// The locator itself is not able to build a path to target node because it does not
            /// operates recursively. The resulting path must be built by calls to this method
            /// on recursive methods such as SceneNode::LocateDepthFirst().
            void AddNodeToPath(SceneNode* nodePtr);

            /// \brief Reverts to initial state.
            ///
            /// Resets the finished indicator and the path so that the locator may be used again.
            virtual void Reset();

            /// \brief Returns the located node.
            ///
            /// Returns NULL if no node was found.
            const SceneNode* LocatedNode() const { return nodePtr; }
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            /// Indicates whether the locator has found the target node.
            bool notFinished;
            SGPath path;
            const SceneNode* nodePtr;
    }; // end class declaration
} // end namespace

#endif
