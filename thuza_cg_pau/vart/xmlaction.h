/// \file xmlaction.h
/// \brief Header file for V-ART class "XmlAction".
/// \version $Revision: 1.10 $

#ifndef VART_XMLACTION_H
#define VART_XMLACTION_H

#include "vart/action.h"
#include "vart/xmlbase.h"

namespace VART {
    class Scene;
    class SceneNode;

/// \class XmlAction xmlaction.h
/// \brief Parser and loader for xml files that describe an Action.
/// \bug Memory allocated for interpolators is never released.
/// \deprecated Please use XmlJointAction.
    // check if bug is still true after creation of XmlBase.

    class XmlAction : public Action, public XmlBase {
        friend std::ostream& operator<<(std::ostream& output, const XmlAction& action);
        public:
        // PUBLIC METHODS
            XmlAction();
            ~XmlAction();
            /// \brief Loads action from a XML file.
            ///
            /// The whole scene is searched for joints.
            bool LoadFromFile(const std::string& fileName, const Scene& scene);

            /// \brief Loads action from a XML file.
            ///
            /// Only descendents of given scene node are searched for joints.
            bool LoadFromFile(const std::string& fileName, const SceneNode& sNode);

        protected:
        // PROTECTED NESTED CLASSES
            class Source
            {
                public:
                    //~ Source();
                    Source(const Scene& scene);
                    Source(const SceneNode& sNode);
                    Joint* GetJointByName(const std::string& name) const;
                protected:
                    const Scene* scenePtr;
                    const SceneNode* sceneNodePtr;
            };
        // PROTECTED METHODS
            void LoadAnimation(const Source& jointSource);
            void ParseDofMovement(JointMover* jointMPtr, XERCES_CPP_NAMESPACE::DOMNode* dofMovItemPtr);
        private:
    }; // end class declaration
} // end namespace

#endif
