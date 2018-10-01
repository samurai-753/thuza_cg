/// \file xmlscene.cpp
/// \brief Implementation file for V-ART class "XmlScene".
/// \version $Revision: 1.9 $

#include "vart/xmlscene.h"
#include "vart/meshobject.h"
#include "vart/dof.h"
#include "vart/sphere.h"
#include "vart/cylinder.h"
#include "vart/biaxialjoint.h"
#include "vart/polyaxialjoint.h"
#include "vart/uniaxialjoint.h"
#include "vart/file.h"
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <sstream>

using XERCES_CPP_NAMESPACE::DOMNodeList;
using XERCES_CPP_NAMESPACE::DOMNode;
using XERCES_CPP_NAMESPACE::DOMNamedNodeMap;
using namespace std;

VART::XmlScene::XmlScene()
{
}

bool VART::XmlScene::LoadFromFile(const std::string& fileName)
{
    meshMap::iterator iterMesh;
    meshObjMap::iterator iterMeshObj;

    if (ParseFile(fileName))
    { //XML parser sucessfull
        //~ istringstream stream;
        //...load the elements of the scene,
        bool result = LoadScene( VART::File::GetPathFromString(fileName) );
        // Terminate Xerces
        Terminate();
        // ...delete unused mesh objects and clear the maps.
        for(iterMesh = mapMesh.begin(); iterMesh != mapMesh.end(); ++iterMesh)
        {
            for(iterMeshObj = mapMeshObj.begin(); iterMeshObj != mapMeshObj.end(); ++iterMeshObj)
                delete iterMeshObj->second;
            mapMeshObj.clear();
        }
        mapMesh.clear();
        return result;
    }

    else
    {   //the parser has failled!
        Terminate();
        return false;
    }
}

bool VART::XmlScene::LoadScene(const std::string& basePath)
// File contents come from xerces' structure at documentPtr. The "basePath" is used to fill in
// relative names in the XML file.
{
    VART::SceneNode* nodePtr;
    DOMNodeList* sceneList = documentPtr->getElementsByTagName(XercesString("scene"));
    unsigned int j;

    // The scene can have cameras and sceneNodes (lights and other objects).
    for (unsigned int i = 0; i < sceneList->getLength(); i++)
    {
        DOMNodeList* nodesL = sceneList->item(i)->getChildNodes();
        for(j = 0; j<nodesL->getLength(); j++)
        {
            if (TempCString(nodesL->item(j)->getNodeName()) == "camera")
            { // FixMe: a LoadCamera method must be implemented
            }
            else if (TempCString(nodesL->item(j)->getNodeName()) == "node")
            {
                // Recursively reads a scene node
                nodePtr = LoadSceneNode(nodesL->item(j), basePath);
                if (nodePtr == NULL)
                    return false;

                // Is it a light?
                VART::Light* lightPtr = dynamic_cast<VART::Light*>(nodePtr);
                if (lightPtr)
                    AddLight(lightPtr);
                else
                {
                    AddObject(nodePtr);
                }
            }
        }
    }
    return true;
}

VART::MeshObject* VART::XmlScene::LoadMeshFromFile(string filen, string type, string meshName)
{
    meshMap::iterator iterMesh;
    meshObjMap::iterator iterMeshObj;
    list<VART::MeshObject*> meshObjectList;
    list<VART::MeshObject*>::iterator iter;
    VART::MeshObject* finalMesh;
    VART::MeshObject* result = NULL;
    finalMesh = new VART::MeshObject;
    finalMesh->autoDelete = true;
    iterMesh = mapMesh.find(filen);
    int i=0;

    if(iterMesh != mapMesh.end())
    {//The file has been loaded
        iterMeshObj = mapMeshObj.find(meshName);
        if(iterMeshObj != mapMeshObj.end())
        {//The mesh has been loaded
            i++;
            finalMesh = iterMeshObj->second;
            mapMeshObj.erase(iterMeshObj);
            return finalMesh;
        }

        else
        {
            cerr<<"Error! Mesh "<<meshName<<" not found in file "<<filen<<endl;
            return NULL;
        }
    }

    else
    {
        //The file hasn't been loaded
        if(type == "obj")
        {
            meshObjectList.clear();
            VART::MeshObject::ReadFromOBJ(filen, &meshObjectList);
            for (iter = meshObjectList.begin(); iter != meshObjectList.end(); ++iter)
            {
                if ((*iter)->GetDescription() == meshName)
                {
                    result=*iter;
                }
                else
                {
                    finalMesh = *iter;
                    mapMeshObj[finalMesh->GetDescription()] = finalMesh;
                }
            }
            mapMesh[filen] = mapMeshObj;
            if (result == NULL)
                cerr << "Error: No mesh object named " << meshName << " was found!" << endl;
            //result->bBoxShow = true;
            return result;
        }
        else
        {
            //FixMe. Must load others types of file.
        }
    //result->bBoxShow = true;
    return result;
    }
}

VART::SceneNode* VART::XmlScene::LoadSceneNode(DOMNode* sceneList, const std::string& basePath)
{
    istringstream stream;
    int numChildren;
    int currentChild = 1;
    VART::SceneNode* result;
    int rTemp, gTemp, bTemp, aTemp;
    string descrStr;
    DOMNodeList* listNode = sceneList->getChildNodes();
    numChildren = listNode->getLength();

    //The sceneNodes can be (joints, tranformations, geometry, lights, etc.)

    if (TempCString(listNode->item(1)->getNodeName()) == "bezier")
    { // FixMe: this part must be implemented
    }
    else if (TempCString(listNode->item(1)->getNodeName()) == "sphere")
    {
        VART::Sphere* spherePtr = new VART::Sphere;
        float radius;
        unsigned int i;
        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrAux;

        spherePtr->autoDelete=true;
        //The sphere is defined by a radius and a material.
        for(i = 0; i<childNodes->getLength(); i++)
        {
            if (TempCString(childNodes->item(i)->getNodeName()) == "radius")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString radiusStr(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                radiusStr.GetValue(&radius);
                spherePtr->SetRadius(radius);
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "material")
            {
                unsigned char r, g, b;
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString redStr(attrPt->getNamedItem(XercesString("r"))->getNodeValue());
                TempCString greenStr(attrPt->getNamedItem(XercesString("g"))->getNodeValue());
                TempCString blueStr(attrPt->getNamedItem(XercesString("b"))->getNodeValue());

                redStr.GetValue(&rTemp);
                r = static_cast<unsigned char>(rTemp);
                greenStr.GetValue(&gTemp);
                g = static_cast<unsigned char>(gTemp);
                blueStr.GetValue(&bTemp);
                b = static_cast<unsigned char>(bTemp);
                spherePtr->SetMaterial(Material(Color(r,g,b)));
            }
        }
        attrAux = listNode->item(1)->getAttributes();
        descrStr = TempCString(attrAux->getNamedItem(XercesString("description"))->getNodeValue());
        spherePtr->SetDescription(descrStr);
        result = spherePtr;
    }

    else if (TempCString(listNode->item(1)->getNodeName()) == "cylinder")
    {
        //The cylinder is defined by a radius, a height and a material.
        VART::Cylinder* cylinderPtr = new VART::Cylinder;
        float radius;
        float height;
        unsigned int i;
        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrAux;

        cylinderPtr->autoDelete=true;

        for(i = 0; i<childNodes->getLength(); i++)
        {
            if (TempCString(childNodes->item(i)->getNodeName()) == "radius")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString radiusStr(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                radiusStr.GetValue(&radius);
                cylinderPtr->SetRadius(radius);
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "height")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString heightStr(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                heightStr.GetValue(&height);
                cylinderPtr->SetHeight(height);
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "material")
            {
                unsigned char r, g, b;
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString redStr(attrPt->getNamedItem(XercesString("r"))->getNodeValue());
                TempCString greenStr(attrPt->getNamedItem(XercesString("g"))->getNodeValue());
                TempCString blueStr(attrPt->getNamedItem(XercesString("b"))->getNodeValue());

                redStr.GetValue(&rTemp);
                r = static_cast<unsigned char>(rTemp);
                greenStr.GetValue(&gTemp);
                g = static_cast<unsigned char>(gTemp);
                blueStr.GetValue(&bTemp);
                b = static_cast<unsigned char>(bTemp);
                cylinderPtr->SetMaterial(Material(Color(r,g,b)));
            }
        }
        attrAux = listNode->item(1)->getAttributes();
        descrStr = TempCString(attrAux->getNamedItem(XercesString("description"))->getNodeValue());
        cylinderPtr->SetDescription(descrStr);
        result =  cylinderPtr;
    }

    else if (TempCString(listNode->item(1)->getNodeName()) == "meshobject")
    {
        // The mesh object can have a material associated and is defined by a file (for example...
        // a Wavefront file).
        list<VART::MeshObject*> meshObjectList;
        list<VART::MeshObject*>::iterator iter;
        string file_name;
        string obj_name;
        string typeName;
        unsigned int i;
        unsigned int j; ///used to traversal in map of node attributes
        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrAux = listNode->item(1)->getAttributes();

        for (i = 0; i<childNodes->getLength(); i++)
        {
            if (TempCString(childNodes->item(i)->getNodeName()) == "material")
            { ///fixMe: this must be implemented
            }
        }
        for (j = 0; j<attrAux->getLength(); j++)
        {
            if (TempCString(attrAux->item(j)->getNodeName()) == "filename")
                file_name = TempCString(attrAux->getNamedItem(XercesString("filename"))->getNodeValue());

            else if (TempCString(attrAux->item(j)->getNodeName()) == "description")
            {
                obj_name = TempCString(attrAux->getNamedItem(XercesString("description"))->getNodeValue());
            }

            else if (TempCString(attrAux->item(j)->getNodeName()) == "type")
            {
                typeName = TempCString(attrAux->getNamedItem(XercesString("type"))->getNodeValue());
            }
        }
        result = LoadMeshFromFile(basePath+file_name, typeName, obj_name);
    }

    else if (TempCString(listNode->item(1)->getNodeName()) == "directionallight")
    {
        VART::Light* directionallightPtr = new VART::Light;
        //~ VART::Color* color;
        //~ VART::Point4D* location;
        unsigned int i;
        float intensity;
        float ambientIntensity;
        float xPos;
        float yPos;
        float zPos;
        //~ bool enabled;
        unsigned char r; // red
        unsigned char g; // green
        unsigned char b; // blue
        unsigned char a; // alpha
        istringstream stream;
        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrTemp = listNode->item(1)->getAttributes();

        //~ location = new VART::Point4D;
        //~ color = new VART::Color;
        directionallightPtr->autoDelete=true;

        for (i = 0; i<childNodes->getLength(); i++)
        {
            if (TempCString(childNodes->item(i)->getNodeName()) == "intensity")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString intensityStr(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                intensityStr.GetValue(&intensity);
                directionallightPtr->SetIntensity(intensity);
            }

            else if (TempCString(childNodes->item(i)->getNodeName()) == "ambientIntensity")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString aiStr(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                aiStr.GetValue(&ambientIntensity);
                directionallightPtr->SetAmbientIntensity(ambientIntensity);
            }

            else if (TempCString(childNodes->item(i)->getNodeName()) == "color")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString redStr(attrPt->getNamedItem(XercesString("red"))->getNodeValue());
                TempCString greenStr(attrPt->getNamedItem(XercesString("green"))->getNodeValue());
                TempCString blueStr(attrPt->getNamedItem(XercesString("blue"))->getNodeValue());
                TempCString alphaStr(attrPt->getNamedItem(XercesString("alpha"))->getNodeValue());

                redStr.GetValue(&rTemp);
                r = static_cast<unsigned char>(rTemp);
                greenStr.GetValue(&gTemp);
                g = static_cast<unsigned char>(gTemp);
                blueStr.GetValue(&bTemp);
                b = static_cast<unsigned char>(bTemp);
                alphaStr.GetValue(&aTemp);
                a = static_cast<unsigned char>(aTemp);

                directionallightPtr->SetColor(Color(r,g,b,a));
            }

            else if (TempCString(childNodes->item(i)->getNodeName()) == "enabled")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString boolValue(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                if (boolValue == "true")
                    directionallightPtr->Turn(true);
                else
                {
                    if (boolValue == "false")
                        directionallightPtr->Turn(false);
                    else
                    {
                        cerr << "XmlScene::LoadSceneNode: Error at 'directionallight' attribute: "
                             << "unknown value for 'enabled': '" << boolValue
                             << "'. Assuming 'true'.\n";
                        directionallightPtr->Turn(true);
                    }
                }
            }

            else if (TempCString(childNodes->item(i)->getNodeName()) == "position")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString xStr(attrPt->getNamedItem(XercesString("x"))->getNodeValue());
                TempCString yStr(attrPt->getNamedItem(XercesString("y"))->getNodeValue());
                TempCString zStr(attrPt->getNamedItem(XercesString("z"))->getNodeValue());

                xStr.GetValue(&xPos);
                yStr.GetValue(&yPos);
                zStr.GetValue(&zPos);
                directionallightPtr->SetLocation(Point4D(xPos, yPos, zPos, 1.0));
            }
        }
        descrStr = TempCString(attrTemp->getNamedItem(XercesString("description"))->getNodeValue());
        directionallightPtr->SetDescription(descrStr);
        result =  directionallightPtr;
    }

    else if (TempCString(listNode->item(1)->getNodeName()) == "spotlight")
    {
        ///FixMe: The spotlight must have an attenuation attribute
        VART::Light* spotlightPtr = new VART::Light;
        spotlightPtr->autoDelete=true;
        VART::Color* color;
        color = new VART::Color;
        VART::Point4D* location;
        location = new VART::Point4D;
        unsigned int i;
        float intensity;
        float ambientIntensity;
        float xPos;
        float yPos;
        float zPos;
        unsigned char r;
        unsigned char g;
        unsigned char b;
        istringstream stream;

        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrTemp = listNode->item(1)->getAttributes();

        for (i = 0; i<childNodes->getLength(); i++)
        {
            if (TempCString(childNodes->item(i)->getNodeName()) == "intensity")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString intensityStr(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                intensityStr.GetValue(&intensity);
                spotlightPtr->SetIntensity(intensity);
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "ambientIntensity")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString aiStr(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                aiStr.GetValue(&ambientIntensity);
                spotlightPtr->SetAmbientIntensity(ambientIntensity);
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "color")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString redStr(attrPt->getNamedItem(XercesString("red"))->getNodeValue());
                TempCString greenStr(attrPt->getNamedItem(XercesString("green"))->getNodeValue());
                TempCString blueStr(attrPt->getNamedItem(XercesString("blue"))->getNodeValue());

                redStr.GetValue(&rTemp);
                r = static_cast<unsigned char>(rTemp);
                greenStr.GetValue(&gTemp);
                g = static_cast<unsigned char>(gTemp);
                blueStr.GetValue(&bTemp);
                b = static_cast<unsigned char>(bTemp);
                spotlightPtr->SetColor(Color(r,g,b));
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "enabled")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString boolValue(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                if (boolValue == "true")
                    spotlightPtr->Turn(true);
                else
                {
                    if (boolValue == "false")
                        spotlightPtr->Turn(false);
                    else
                    {
                        cerr << "XmlScene::LoadSceneNode: Error at 'spotlight' attribute: "
                             << "unknown value for 'enabled': '" << boolValue
                             << "'. Assuming 'true'.\n";
                        spotlightPtr->Turn(true);
                    }
                }
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "position")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString xStr(attrPt->getNamedItem(XercesString("x"))->getNodeValue());
                TempCString yStr(attrPt->getNamedItem(XercesString("y"))->getNodeValue());
                TempCString zStr(attrPt->getNamedItem(XercesString("z"))->getNodeValue());

                xStr.GetValue(&xPos);
                yStr.GetValue(&yPos);
                zStr.GetValue(&zPos);
                spotlightPtr->SetLocation(Point4D(xPos, yPos, zPos, 1.0));
            }
        }
        descrStr = TempCString(attrTemp->getNamedItem(XercesString("description"))->getNodeValue());
        spotlightPtr->SetDescription(descrStr);
        result =  spotlightPtr;
    }

    else if (TempCString(listNode->item(1)->getNodeName()) == "pointlight")
    {///FixMe: The pointlight must have an attenuation attribute
        VART::Light* pointlightPtr = new VART::Light;
        pointlightPtr->autoDelete=true;
        VART::Color* color;
        color = new VART::Color;
        VART::Point4D* location;
        location = new VART::Point4D;
        unsigned int i;
        float intensity;
        float ambientIntensity;
        float xPos;
        float yPos;
        float zPos;
        ///Color attributes
        unsigned char r;
        unsigned char g;
        unsigned char b;
        istringstream stream;

        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrTemp = listNode->item(1)->getAttributes();

        for (i = 0; i<childNodes->getLength(); i++)
        {
            if (TempCString(childNodes->item(i)->getNodeName()) == "intensity")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString intensityStr(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                intensityStr.GetValue(&intensity);
                pointlightPtr->SetIntensity(intensity);
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "ambientIntensity")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString aiStr(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                aiStr.GetValue(&ambientIntensity);
                pointlightPtr->SetAmbientIntensity(ambientIntensity);
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "color")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString redStr(attrPt->getNamedItem(XercesString("red"))->getNodeValue());
                TempCString greenStr(attrPt->getNamedItem(XercesString("green"))->getNodeValue());
                TempCString blueStr(attrPt->getNamedItem(XercesString("blue"))->getNodeValue());

                redStr.GetValue(&rTemp);
                r = static_cast<unsigned char>(rTemp);
                greenStr.GetValue(&gTemp);
                g = static_cast<unsigned char>(gTemp);
                blueStr.GetValue(&bTemp);
                b = static_cast<unsigned char>(bTemp);
                pointlightPtr->SetColor(Color(r,g,b));
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "enabled")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString boolValue(attrPt->getNamedItem(XercesString("value"))->getNodeValue());

                if (boolValue == "true")
                    pointlightPtr->Turn(true);
                else
                {
                    if (boolValue == "false")
                        pointlightPtr->Turn(false);
                    else
                    {
                        cerr << "XmlScene::LoadSceneNode: Error at 'pointlight' attribute: "
                             << "unknown value for 'enabled': '" << boolValue
                             << "'. Assuming 'true'.\n";
                        pointlightPtr->Turn(true);
                    }
                }
            }
            else if (TempCString(childNodes->item(i)->getNodeName()) == "position")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString xStr(attrPt->getNamedItem(XercesString("x"))->getNodeValue());
                TempCString yStr(attrPt->getNamedItem(XercesString("y"))->getNodeValue());
                TempCString zStr(attrPt->getNamedItem(XercesString("z"))->getNodeValue());

                xStr.GetValue(&xPos);
                yStr.GetValue(&yPos);
                zStr.GetValue(&zPos);
                pointlightPtr->SetLocation(Point4D(xPos, yPos, zPos, 1.0));
            }
        }
        descrStr = TempCString(attrTemp->getNamedItem(XercesString("description"))->getNodeValue());
        pointlightPtr->SetDescription(descrStr);
        result =  pointlightPtr;
    }

    else if (TempCString(listNode->item(1)->getNodeName()) == "transform")
    {
        //The transform can be defined by a matrix that defines the translation and
        //rotations of the object, or a scale, or only a rotation, or only a translation.

        list<VART::Transform>::const_iterator iter;
        list<VART::Transform> listTrans;
        istringstream stream;
        VART::Transform* trans;
        trans = new VART::Transform();
        trans->autoDelete=true;
        unsigned int i;
        trans->MakeIdentity();
        float xPos;
        float yPos;
        float zPos;

        DOMNodeList* childNodes = listNode->item(1)->getChildNodes();
        DOMNamedNodeMap* attrTemp = listNode->item(1)->getAttributes();

        for (i = 0; i<childNodes->getLength(); i++)
        {

            if (TempCString(childNodes->item(i)->getNodeName()) == "matrix")
            {
                double data[16];
                float tempFloat;
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString m00Str(attrPt->getNamedItem(XercesString("m00"))->getNodeValue());
                TempCString m01Str(attrPt->getNamedItem(XercesString("m01"))->getNodeValue());
                TempCString m02Str(attrPt->getNamedItem(XercesString("m02"))->getNodeValue());
                TempCString m03Str(attrPt->getNamedItem(XercesString("m03"))->getNodeValue());
                TempCString m10Str(attrPt->getNamedItem(XercesString("m10"))->getNodeValue());
                TempCString m11Str(attrPt->getNamedItem(XercesString("m11"))->getNodeValue());
                TempCString m12Str(attrPt->getNamedItem(XercesString("m12"))->getNodeValue());
                TempCString m13Str(attrPt->getNamedItem(XercesString("m13"))->getNodeValue());
                TempCString m20Str(attrPt->getNamedItem(XercesString("m20"))->getNodeValue());
                TempCString m21Str(attrPt->getNamedItem(XercesString("m21"))->getNodeValue());
                TempCString m22Str(attrPt->getNamedItem(XercesString("m22"))->getNodeValue());
                TempCString m23Str(attrPt->getNamedItem(XercesString("m23"))->getNodeValue());
                TempCString m30Str(attrPt->getNamedItem(XercesString("m30"))->getNodeValue());
                TempCString m31Str(attrPt->getNamedItem(XercesString("m31"))->getNodeValue());
                TempCString m32Str(attrPt->getNamedItem(XercesString("m32"))->getNodeValue());
                TempCString m33Str(attrPt->getNamedItem(XercesString("m33"))->getNodeValue());

                m00Str.GetValue(&tempFloat);
                data[0] = tempFloat;
                m01Str.GetValue(&tempFloat);
                data[1] = tempFloat;
                m02Str.GetValue(&tempFloat);
                data[2] = tempFloat;
                m03Str.GetValue(&tempFloat);
                data[3] = tempFloat;
                m10Str.GetValue(&tempFloat);
                data[4] = tempFloat;
                m11Str.GetValue(&tempFloat);
                data[5] = tempFloat;
                m12Str.GetValue(&tempFloat);
                data[6] = tempFloat;
                m13Str.GetValue(&tempFloat);
                data[7] = tempFloat;
                m20Str.GetValue(&tempFloat);
                data[8] = tempFloat;
                m21Str.GetValue(&tempFloat);
                data[9] = tempFloat;
                m22Str.GetValue(&tempFloat);
                data[10] = tempFloat;
                m23Str.GetValue(&tempFloat);
                data[11] = tempFloat;
                m30Str.GetValue(&tempFloat);
                data[12] = tempFloat;
                m31Str.GetValue(&tempFloat);
                data[13] = tempFloat;
                m32Str.GetValue(&tempFloat);
                data[14] = tempFloat;
                m33Str.GetValue(&tempFloat);
                data[15] = tempFloat;

                trans->SetData(data);
                descrStr = TempCString(attrTemp->getNamedItem(XercesString("description"))->getNodeValue());
                trans->SetDescription(descrStr);
                result = trans;
            }

            else if (TempCString(childNodes->item(i)->getNodeName()) == "translation")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString xStr(attrPt->getNamedItem(XercesString("x"))->getNodeValue());
                TempCString yStr(attrPt->getNamedItem(XercesString("y"))->getNodeValue());
                TempCString zStr(attrPt->getNamedItem(XercesString("z"))->getNodeValue());

                xStr.GetValue(&xPos);
                yStr.GetValue(&yPos);
                zStr.GetValue(&zPos);
                trans-> MakeTranslation(Point4D(xPos, yPos, zPos, 0));

                descrStr = TempCString(attrTemp->getNamedItem(XercesString("description"))->getNodeValue());
                trans->SetDescription(descrStr);
                result =  trans;
            }

            else if (TempCString(childNodes->item(i)->getNodeName()) == "scale")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                TempCString xStr(attrPt->getNamedItem(XercesString("x"))->getNodeValue());
                TempCString yStr(attrPt->getNamedItem(XercesString("y"))->getNodeValue());
                TempCString zStr(attrPt->getNamedItem(XercesString("z"))->getNodeValue());

                xStr.GetValue(&xPos);
                yStr.GetValue(&yPos);
                zStr.GetValue(&zPos);
                trans-> MakeScale(xPos, yPos, zPos);

                descrStr = TempCString(attrTemp->getNamedItem(XercesString("description"))->getNodeValue());
                trans->SetDescription(descrStr);
                result =  trans;
            }

            else if (TempCString(childNodes->item(i)->getNodeName()) == "rotation")
            {
                DOMNamedNodeMap* attrPt = childNodes->item(i)->getAttributes();
                float radians;
                TempCString axisStr(attrPt->getNamedItem(XercesString("axis"))->getNodeValue());
                TempCString radiansStr(attrPt->getNamedItem(XercesString("radians"))->getNodeValue());

                radiansStr.GetValue(&radians);
                if (axisStr == "x")
                    trans->MakeXRotation(radians);
                else if (axisStr == "y")
                    trans->MakeYRotation(radians);
                else if (axisStr == "z")
                    trans->MakeZRotation(radians);
                descrStr = TempCString(attrTemp->getNamedItem(XercesString("description"))->getNodeValue());
                trans->SetDescription(descrStr);
                result =  trans;
            }
        }
    }

    else if (TempCString(listNode->item(1)->getNodeName()) == "joint")
    {
        list<VART::Dof*> listOfDofs;
        list<VART::Dof*>::const_iterator iter;
        DOMNamedNodeMap* attrNode = listNode->item(1)->getAttributes();
        TempCString name(attrNode->getNamedItem(XercesString("type"))->getNodeValue());

        /// Create a Joint and its dependencies
        if (name == "biaxial")
        {
            VART::BiaxialJoint* newJointB;
            DOMNamedNodeMap* attrAux;

            newJointB = new VART::BiaxialJoint();
            newJointB->autoDelete = true;
            loadDofs (listNode->item(1), &listOfDofs);
            for (iter = listOfDofs.begin(); iter!= listOfDofs.end(); ++iter)
                newJointB->AddDof(*iter);
            attrAux = listNode->item(1)->getAttributes();
            descrStr = TempCString(attrAux->getNamedItem(XercesString("description"))->getNodeValue());
            newJointB->SetDescription(descrStr);
            result = newJointB;
        }

        else if (name == "polyaxial")
        {
            VART::PolyaxialJoint* newJointB;
            DOMNamedNodeMap* attrAux;

            newJointB = new VART::PolyaxialJoint();
            newJointB->autoDelete = true;
            loadDofs (listNode->item(1), &listOfDofs);
            for (iter = listOfDofs.begin(); iter!=listOfDofs.end(); ++iter)
                newJointB->AddDof(*iter);
            attrAux = listNode->item(1)->getAttributes();
            descrStr = TempCString(attrAux->getNamedItem(XercesString("description"))->getNodeValue());
            newJointB->SetDescription(descrStr);
            result = newJointB;
        }

        else if (name == "uniaxial")
        {
            VART::UniaxialJoint* newJointB;
            DOMNamedNodeMap* attrAux;

            newJointB = new VART::UniaxialJoint();
            newJointB->autoDelete = true;
            attrAux = listNode->item(1)->getAttributes();
            descrStr = TempCString(attrAux->getNamedItem(XercesString("description"))->getNodeValue());
            newJointB->SetDescription(descrStr);
            loadDofs (listNode->item(1), &listOfDofs);
            for (iter = listOfDofs.begin(); iter!=listOfDofs.end(); ++iter)
            {
                newJointB->AddDof(*iter);
            }
            result = newJointB;
        }
    }

    // Next XML node
    currentChild += 2;
    while (currentChild < numChildren)
    {
        // FixMe: Need to treat lights in a proper way.
        SceneNode* snPtr = LoadSceneNode(listNode->item(currentChild), basePath);

        result->AddChild(*snPtr);
        currentChild += 2;
    }
    return result;
}

void VART::XmlScene::loadDofs(DOMNode* node, list<VART::Dof*>* dofs)
{
    list<VART::Dof*>::iterator iter;
    unsigned int i;
    unsigned int j;
    istringstream stream;
    float xPos;
    float yPos;
    float zPos;
    float minValue, maxValue, resValue;
    VART::Point4D axis;
    VART::Point4D position;
    DOMNodeList* dof = node->getChildNodes();
    DOMNamedNodeMap* attrAux;
    string descrStr;

    for (i = 0; i< dof->getLength(); i++)
    {
        if (TempCString(dof->item(i)->getNodeName()) == "dof")
        {
            VART::Dof* d = new VART::Dof;
            DOMNodeList* dofAux = dof->item(i)->getChildNodes();

            d->autoDelete=true;

            for (j = 0; j <dofAux->getLength(); j++ )
            {
                if (TempCString(dofAux->item(j)->getNodeName()) == "position")
                {
                    DOMNamedNodeMap* attrPos = dofAux->item(j)->getAttributes();
                    TempCString xStr(attrPos->getNamedItem(XercesString("x"))->getNodeValue());
                    TempCString yStr(attrPos->getNamedItem(XercesString("y"))->getNodeValue());
                    TempCString zStr(attrPos->getNamedItem(XercesString("z"))->getNodeValue());

                    xStr.GetValue(&xPos);
                    yStr.GetValue(&yPos);
                    zStr.GetValue(&zPos);
                    position.SetXYZW(xPos,yPos,zPos,1);
                }

                else if (TempCString(dofAux->item(j)->getNodeName()) == "axis")
                {
                    DOMNamedNodeMap* attrAxis = dofAux->item(j)->getAttributes();
                    TempCString xStr(attrAxis->getNamedItem(XercesString("x"))->getNodeValue());
                    TempCString yStr(attrAxis->getNamedItem(XercesString("y"))->getNodeValue());
                    TempCString zStr(attrAxis->getNamedItem(XercesString("z"))->getNodeValue());

                    xStr.GetValue(&xPos);
                    yStr.GetValue(&yPos);
                    zStr.GetValue(&zPos);
                    axis.SetXYZW(xPos, yPos, zPos, 0);
                    axis.Normalize();
                }
                else if (TempCString(dofAux->item(j)->getNodeName()) == "range")
                {
                    DOMNamedNodeMap *attrRange = dofAux->item(j)->getAttributes();
                    TempCString minStr(attrRange->getNamedItem(XercesString("min"))->getNodeValue());
                    TempCString maxStr(attrRange->getNamedItem(XercesString("max"))->getNodeValue());
                    TempCString resStr(attrRange->getNamedItem(XercesString("rest"))->getNodeValue());

                    minStr.GetValue(&minValue);
                    maxStr.GetValue(&maxValue);
                    resStr.GetValue(&resValue);
                }
            }
            d->Set(axis, position, minValue, maxValue);
            attrAux = dof->item(i)->getAttributes();
            descrStr = TempCString(attrAux->getNamedItem(XercesString("description"))->getNodeValue());
            d->SetDescription(descrStr);
            d->SetRest(resValue);
            dofs->push_back(d);
        }
    }
}

VART::XmlScene::~XmlScene()
{
}
