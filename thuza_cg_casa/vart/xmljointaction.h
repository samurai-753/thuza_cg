/// \file xmljointaction.h
/// \brief Header file for V-ART class "XmlJointAction".
/// \version $Revision: 1.1 $

#ifndef VART_XMLJOINTACTION_H
#define VART_XMLJOINTACTION_H

#include "vart/jointaction.h"
#include "vart/xmlbase.h"

namespace VART
{
    class SceneNode;
/// \class XmlJointAction xmljointaction.h
/// \brief Parser and loader for xml files that describe an JointAction.
///
/// FixMe: Put Detailed description here
    class XmlJointAction : public JointAction, public XmlBase 
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            XmlJointAction();

            /// \brief Loads JointAction from a XML file.
            ///
            /// Only descendents of given scene node are searched for joints.
            bool LoadFromFile(const std::string& fileName, const SceneNode& sNode);
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
            void ParseDofMovement(JointMover* jointMPtr,
                                  XERCES_CPP_NAMESPACE::DOMNode* dofMovItemPtr);
            void CreateJointMover(const VART::SceneNode& sNode,
                                  XERCES_CPP_NAMESPACE::DOMNode* xmlNodePtr);
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
    }; // end class declaration
} // end namespace

#endif
