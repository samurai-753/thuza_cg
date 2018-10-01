/// \file scenenode.h
/// \brief Header file for V-ART class "SceneNode".
/// \version $Revision: 1.11 $

#ifndef VART_SCENENODE_H
#define VART_SCENENODE_H

#include "vart/memoryobj.h"
#include <list>
#include <string>
#include <iostream> // for XmlPrintOn

namespace VART {
    class SGPath;
    class SNOperator;
    class SNLocator;
/// \class SceneNode scenenode.h
/// \brief Base class for objects that compose a scene graph
///
/// A SceneNode is something directly related to the rendering engine such as shapes
/// (GraphicObj), transformations (Transform) and lights (Light). Several
/// nodes together create an environment (Scene) that is draw every rendering
/// cicle. SceneNodes have childs to allow creating a hierarchy of objects. This class
/// should be considered abstract.
    class SceneNode : public MemoryObj {
        public:
        // PUBLIC TYPES
            enum TypeID { NONE, GRAPHIC_OBJ, BOX, CONE, CURVE, BEZIER,
                          CYLINDER, IMAGE, IMAGE_2D, VOLUME, MESH_OBJECT,
                          SPHERE, JOINT, BIAXIAL_JOINT, CONDYLAR_JOINT,
                          ELLIPSOID_JOINT, SADDLE_JOINT, PLANE_JOINT,
                          POLYAXIAL_JOINT, UNIAXIAL_JOINT, HINGE_JOINT,
                          PIVOT_JOINT, TRANSFORM };
        // PUBLIC METHODS
            /// Creates an uninitialized scene node
            SceneNode();
            SceneNode(SceneNode& node);
            virtual ~SceneNode();
            SceneNode& operator=(const SceneNode& node);
            
            /// Returns a copy of an SceneNode. Every derived class must reimplement 
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual SceneNode* Copy() = 0;

            /// Returns a copy of an SceneNode, and of all of its children, recursively.
            /// \deprecated Returns a pointer to memory allocated by the framework.
            virtual SceneNode* RecursiveCopy();

            /// Returns a copy of the object's description
            const std::string& GetDescription() const { return description; }

            /// Changes the object's description
            void SetDescription(const std::string& desc) { description = desc; }

            /// Add a child at the end of child list
            void AddChild(SceneNode& child);

            /// \brief Removes a child from the child list
            /// \return False if given child pointer was not found.
            ///
            /// The child node is not destroyed, it is simply removed from the child list.
            /// Removes the first ocurrence of given scene node.
            bool DetachChild(SceneNode* childPtr);

            /// \brief Recursive drawing using OpenGL commands
            /// \return false if V-ART is was not compiled with OpenGL support
            virtual bool DrawOGL() const;

            /// \brief Draws and object, setting pick info
            ///
            /// This method should be called in selection mode in order to identify objects
            /// that are selected by the mouse (see Scene::Pick).
            virtual void DrawForPicking() const = 0;

            /// Returns type identification of the node.
            virtual TypeID GetID() const { return NONE; }

            /// Deletes (dealocate memory) recursively all children marked as 'autoDelete'.
            void AutoDeleteChildren() const;

            /// \brief Recusively searches its children for a given name
            /// \deprecated Please use a SNLocator.
            SceneNode* FindChildByName(const std::string& name) const;

            /// Returns the list of children.
            /// \deprecated Incorrect name. Exposes a private attribute. Returns a list by copy.
            ///             Please use TraverseDepthFirst or TraverseBreadthFirst.
            std::list<SceneNode*> GetChilds();

            /// \brief Search target among children.
            /// \param targetPtr [in] Node to search for.
            /// \param resultPtr [out] Where to store the computed path.
            /// \return True if a path is found. False otherwise.
            /// \deprecated Please use LocateDepthFirst.
            ///
            /// Performs a depth-first search and returns the first path to target node. If found,
            /// the computed path will not include the initial scene node (the one that was asked
            /// to find a path) but it will include the target scene node.
            bool FindPathTo(SceneNode* targetPtr, SGPath* resultPtr) const;

            /// \brief Search target among children.
            /// \param targetName [in] Node to search for.
            /// \param resultPtr [out] Where to store the computed path.
            /// \return True if a path is found. False otherwise.
            /// \deprecated Please use LocateDepthFirst.
            ///
            /// Performs a depth-first search and returns the first path to target node. If found,
            /// the computed path will not include the initial scene node (the one that was asked
            /// to find a path) but it will include the target scene node.
            bool FindPathTo(const std::string& targetName, SGPath* resultPtr) const;

            /// \brief Find all the nodes of with typeID 'type'.
            /// \deprecated Does not obey parameter convention.
            ///             Please use TraverseDepthFirst (or TraverseBreadthFirst) with a Collector.
            /// \return total Number of Nodes. 'nodeList' stores nodes.
            int GetNodeTypeList(TypeID type, std::list<SceneNode*>& nodeList );

            /// \brief Process all children in depth-first order.
            /// \param operatorPtr [in,out] A scene node operator.
            ///
            /// Applies a scene node operator to all children in depth-first order.
            virtual void TraverseDepthFirst(SNOperator* operatorPtr) const;

            /// \brief Process all children in breadth-first order.
            /// \param operatorPtr [in,out] A scene node operator.
            ///
            /// Applies a scene node operator to all children in breadth-first order.
            virtual void TraverseBreadthFirst(SNOperator* operatorPtr) const;

            /// \brief Seaches for a particular scene node (depth first)
            ///
            /// Recursively applies a locator, building a path (see SGPath) to it when it
            /// signals completion. The resulting path does not include the initial scene
            /// node.
            virtual void LocateDepthFirst(SNLocator* locatorPtr) const;

            /// \brief Seaches for a particular scene node (breadth first)
            ///
            /// Applies a locator to self and all of its children. Does not builds a path
            /// to target node, since the breadth search is not recursive.
            virtual void LocateBreadthFirst(SNLocator* locatorPtr) const;

            /// \brief Recursively outputs XML representation of the scene node.
            virtual void XmlPrintOn(std::ostream& os, unsigned int indent) const;

        // STATIC PUBLIC ATTRIBUTES
            static bool recursivePrinting;
        protected:
        // PROTECTED METHODS
            /// Non-recursive drawing - should be overriden by every derived class.
            virtual bool DrawInstanceOGL() const { return false; };

            /// Recursive auxiliar method for FindPathTo.
            bool RecursiveFindPathTo(SceneNode* targetPtr, SGPath* resultPtr) const;

            /// Recursive auxiliar method for FindPathTo.
            bool RecursiveFindPathTo(const std::string& targetName, SGPath* resultPtr) const;
        // PROTECTED ATTRIBUTES
            /// Child list
            std::list<SceneNode*> childList;
            /// Textual identification
            std::string description;
    }; // end class declaration
} // end namespace
#endif
