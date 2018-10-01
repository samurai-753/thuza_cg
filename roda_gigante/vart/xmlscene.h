/// \file xmlscene.h
/// \brief Header file for V-ART class "XmlScene".
/// \version $Revision: 1.5 $

#ifndef VART_XML_H
#define VART_XML_H

#include "vart/scene.h"
#include "vart/xmlbase.h"
#include <string>
#include <map>

namespace VART {
    class MeshObject;
    class Dof;
    typedef std::map<std::string, MeshObject*> meshObjMap;
    typedef std::map<std::string, meshObjMap> meshMap;

    class XmlScene : public Scene, public XmlBase {
        public:
            XmlScene();
            ~XmlScene();
            /// Parses the xml file. If it doesn't have errors, load scene.
            bool LoadFromFile(const std::string& fileName);
            /// Load the scene based in xml archieve.
            bool LoadScene(const std::string& basePath);
            /// Load the nodes (transformations, geometry, etc.) of the scene.
            SceneNode* LoadSceneNode(XERCES_CPP_NAMESPACE::DOMNode* sceneList, const std::string& basePath);
            /// Load MeshObjects from file.
            MeshObject* LoadMeshFromFile(std::string filen, std::string type, std::string meshName);
            /// Load the dofs of the joint.
            void loadDofs( XERCES_CPP_NAMESPACE::DOMNode* node, std::list<Dof*>* dofs);

        private:
            meshObjMap mapMeshObj;
            meshMap mapMesh;
    }; // end class declaration
} // end namespace

#endif
