/// \file xmlaction.cpp
/// \brief Implementation file for V-ART class "XmlAction".
/// \version $Revision: 1.13 $

#include "vart/xmlaction.h"
#include "vart/scene.h"
#include "vart/joint.h"
#include "vart/linearinterpolator.h"
#include "vart/sineinterpolator.h"
#include "vart/rangesineinterpolator.h"
#include "vart/jointmover.h"
#include "vart/noisydofmover.h"

#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#include <sstream>

using namespace std;
using namespace XERCES_CPP_NAMESPACE;

VART::XmlAction::XmlAction()
{
}

bool VART::XmlAction::LoadFromFile(const string& fileName, const VART::Scene& scene)
{
    Source jointSource(scene);

    //~ cout << "xmlaction loading from " << fileName << endl;
    if(ParseFile(fileName))
    {   //XML parser sucessfull
        LoadAnimation(jointSource);
        Terminate();
        return true;
    }
    else
    {   //the parser has failled!
        Terminate();
        return false;
    }
}

bool VART::XmlAction::LoadFromFile(const string& fileName, const VART::SceneNode& sNode)
{
    Source jointSource(sNode);

    //~ cout << "xmlaction loading from " << fileName << endl;
    if(ParseFile(fileName))
    {   //XML parser sucessfull
        LoadAnimation(jointSource);
        Terminate();
        return true;
    }
    else
    {   //the parser has failled!
        Terminate();
        return false;
    }
}

void VART::XmlAction::LoadAnimation(const Source& jointSource)
// Called by LoadFromFile after parsing to actually build the action from file.
{
    static VART::LinearInterpolator linearInterpolator; // common interpolator for all joint movers
    static VART::SineInterpolator sineInterpolator;     // common interpolator for all joint movers
    istringstream stream; // helper for converting strings to numbers
    char* charAux; // helper for converting XMLStrings to C strings
    unsigned int i, j, k; // loop counters (aaargh!)
    float dur, speed; // duration and speed of the action
    bool cycle;       // cyclic status of the action
    string dofIdentifier, jName, itplName;
    XMLCh* xmlStr; // helper for converting C strings to XMLStrings
    DOMNodeList* actionList;
    DOMNamedNodeMap* attrAc; // attributes for the action
    VART::JointMover* jointMPtr;

    // Get list of actions (but there should be only one!)
    xmlStr = XMLString::transcode("action");
    actionList = documentPtr->getElementsByTagName(xmlStr);
    XMLString::release(&xmlStr);

    attrAc = documentPtr->getDocumentElement()->getAttributes();

    // Read speed value
    xmlStr = XMLString::transcode("speed");
    charAux = XMLString::transcode(attrAc->getNamedItem(xmlStr)->getNodeValue());
    stream.clear();
    stream.str(charAux);
    stream >> speed;
    XMLString::release(&xmlStr);
    XMLString::release(&charAux);

    // Read cyclic value
    xmlStr = XMLString::transcode("cycle");
    charAux = XMLString::transcode(attrAc->getNamedItem(xmlStr)->getNodeValue());
    cycle = (strcmp(charAux, "true") == 0);
    XMLString::release(&xmlStr);
    XMLString::release(&charAux);

    // Set action attributes
    Set(speed, 1, cycle); //Priority default is 1.

    // For each action (but there should be only one!):
    for (i = 0; i<actionList->getLength(); i++)
    {
        DOMNodeList* jointsL = actionList->item(i)->getChildNodes();

        // For each joint movement:
        for (j = 0; j<jointsL->getLength(); j++)
        {
            bool isJointMovement;

            charAux = XMLString::transcode(jointsL->item(j)->getNodeName());
            isJointMovement = (strcmp(charAux, "joint_movement") == 0);
            XMLString::release(&charAux);

            if (isJointMovement)
            {
                Joint* jointPtr;
                DOMNamedNodeMap* attrLt = jointsL->item(j)->getAttributes();

                // Read joint mover's duration value
                xmlStr = XMLString::transcode("duration");
                charAux = XMLString::transcode(attrLt->getNamedItem(xmlStr)->getNodeValue());
                stream.clear();
                stream.str(charAux);
                stream >> dur;
                XMLString::release(&xmlStr);
                XMLString::release(&charAux);

                // Read joint mover's target joint name
                xmlStr = XMLString::transcode("joint_name");
                charAux = XMLString::transcode(attrLt->getNamedItem(xmlStr)->getNodeValue());
                jName = charAux;
                XMLString::release(&xmlStr);
                XMLString::release(&charAux);

                jointPtr = jointSource.GetJointByName(jName);
                if (jointPtr)
                {
                    DOMNodeList* dofsL = jointsL->item(j)->getChildNodes();

                    // It is now almost safe to create a joint mover :)
                    // Add an ease-in, ease-out interpolator as default. Change it when the
                    // interpolator gets known.
                    jointMPtr = AddJointMover(jointPtr, dur, sineInterpolator);

    
                    // For each element of a joint movement (may be interpolator or dof movement):
                    for(k = 0; k<dofsL->getLength(); k++)
                    {
                        bool isInterpolation;

                        charAux = XMLString::transcode(dofsL->item(k)->getNodeName());
                        isInterpolation = (strcmp(charAux, "interpolation") == 0);
                        XMLString::release(&charAux);
                        if (isInterpolation)
                        {
                            DOMNamedNodeMap* attrInterpLt = dofsL->item(k)->getAttributes();

                            // Read interpolation type
                            xmlStr = XMLString::transcode("type");
                            charAux = XMLString::transcode(attrInterpLt->getNamedItem(xmlStr)->getNodeValue());
                            itplName = charAux;
                            XMLString::release(&xmlStr);
                            XMLString::release(&charAux);

                            // Change interpolator if it is different from default (ease-in/ease-out) value
                            if (itplName != "ease-in_ease-out")
                            {
                                if (itplName == "linear")
                                    jointMPtr->SetInterpolator(linearInterpolator);
                                else if (itplName == "range_sine")
                                {
                                    Interpolator* interpolatorPtr = new RangeSineInterpolator;
                                    jointMPtr->SetInterpolator(*interpolatorPtr);
                                }
                                else
                                    cerr << "Error: Unknown interpolator: '" << itplName
                                         << "'. Using: 'ease-in_ease-out'." << endl;
                            }
                        }
                        else // item is not "interpolation"
                        {
                            charAux = XMLString::transcode(dofsL->item(k)->getNodeName());
                            if(strcmp(charAux, "dof_movement") == 0)
                                ParseDofMovement(jointMPtr, dofsL->item(k));
                            XMLString::release(&charAux);
                        }
                    }
                }
                else // the target joint was not found
                {
                    cerr << "Error: Joint mover targeting at '" << jName
                         << "': No such scene node!" << endl;
                }
            }
        }
    }
}

void VART::XmlAction::ParseDofMovement(JointMover* jointMPtr, DOMNode* dofMovItemPtr)
// Called by LoadAnimation to create a dof mover from a XML entry.
// FixMe: Memory allocated for the interpolators is never released.
{
    DOMNodeList* dofMovElements;
    DOMNamedNodeMap* attrPt;
    XMLCh* xmlStr; // helper for converting C strings to XMLStrings
    float inTime;
    float fTime;
    float fPos;
    float noiseAmplitude;
    float noiseWaveLenght;
    float overshoot;
    float offset;
    float peakTime;
    char* charAux;
    char* cstr_nodeName; // a node name (as a C string)
    istringstream stream;
    VART::Joint::DofID dofID;
    unsigned int count;
    bool foundPositionalError = false; // any positional error elements found inside the dof mover?
    bool foundNoise = false;           // any noise elements found inside the dof mover?

    attrPt = dofMovItemPtr->getAttributes();

    xmlStr = XMLString::transcode("initialTime");
    charAux = XMLString::transcode(attrPt->getNamedItem(xmlStr)->getNodeValue());
    stream.str(charAux);
    stream >> inTime;
    XMLString::release(&xmlStr);
    XMLString::release(&charAux);

    xmlStr = XMLString::transcode("finalTime");
    charAux = XMLString::transcode(attrPt->getNamedItem(xmlStr)->getNodeValue());
    stream.clear();
    stream.str(charAux);
    stream >> fTime;
    XMLString::release(&xmlStr);
    XMLString::release(&charAux);

    xmlStr = XMLString::transcode("finalPosition");
    charAux = XMLString::transcode(attrPt->getNamedItem(xmlStr)->getNodeValue());
    stream.clear();
    stream.str(charAux);
    stream >> fPos;
    XMLString::release(&xmlStr);
    XMLString::release(&charAux);

    // Read value for dofID
    xmlStr = XMLString::transcode("dofID");
    charAux = XMLString::transcode(attrPt->getNamedItem(xmlStr)->getNodeValue());
    if ((strcmp(charAux, "FLEX") == 0) || (strcmp(charAux, "FLEXION") == 0))
        dofID = VART::Joint::FLEXION;
    else
    {
        if ((strcmp(charAux, "ADDUCT") == 0) || (strcmp(charAux, "ADDUCTION") == 0))
            dofID = VART::Joint::ADDUCTION;
        else
            if (strcmp(charAux, "TWIST") == 0)
                dofID = VART::Joint::TWIST;
            else
            {
                cerr << "XmlAction::ParseDofMovement: Unknown DOF ID" << endl;
                exit (1);
            }
    }
    XMLString::release(&xmlStr);
    XMLString::release(&charAux);

    dofMovElements = dofMovItemPtr->getChildNodes();

    // for each child node of the dof mover (may be "noise" or "error"):
    for (count = 0; count < dofMovElements->getLength(); ++count)
    {
        cstr_nodeName = XMLString::transcode(dofMovElements->item(count)->getNodeName());
        if (strcmp(cstr_nodeName, "noise") == 0)
        {
            attrPt = dofMovElements->item(count)->getAttributes();

            xmlStr = XMLString::transcode("amplitude");
            charAux = XMLString::transcode(attrPt->getNamedItem(xmlStr)->getNodeValue());
            stream.clear();
            stream.str(charAux);
            stream >> noiseAmplitude;
            XMLString::release(&xmlStr);
            XMLString::release(&charAux);

            xmlStr = XMLString::transcode("length");
            charAux = XMLString::transcode(attrPt->getNamedItem(xmlStr)->getNodeValue());
            stream.clear();
            stream.str(charAux);
            stream >> noiseWaveLenght;
            XMLString::release(&xmlStr);
            XMLString::release(&charAux);

            foundNoise = true;
        }
        else if (strcmp(cstr_nodeName, "error") == 0)
        {
            attrPt = dofMovElements->item(count)->getAttributes();

            xmlStr = XMLString::transcode("overshoot");
            charAux = XMLString::transcode(attrPt->getNamedItem(xmlStr)->getNodeValue());
            stream.clear();
            stream.str(charAux);
            stream >> overshoot;
            XMLString::release(&xmlStr);
            XMLString::release(&charAux);
            NoisyDofMover::SetDefaultOvershoot(overshoot);

            xmlStr = XMLString::transcode("offset");
            charAux = XMLString::transcode(attrPt->getNamedItem(xmlStr)->getNodeValue());
            stream.clear();
            stream.str(charAux);
            stream >> offset;
            XMLString::release(&xmlStr);
            XMLString::release(&charAux);
            NoisyDofMover::SetDefaultOffset(offset);

            xmlStr = XMLString::transcode("peak_time");
            charAux = XMLString::transcode(attrPt->getNamedItem(xmlStr)->getNodeValue());
            stream.clear();
            stream.str(charAux);
            stream >> peakTime;
            XMLString::release(&xmlStr);
            XMLString::release(&charAux);
            NoisyDofMover::SetDefaultPeakTime(peakTime);

            foundPositionalError = true;
        }
        delete[] cstr_nodeName;
    }

    // Ready to create a dof mover
    if (foundPositionalError || foundNoise)
    {
        VART::NoisyDofMover* noisyDofMoverPtr;

        noisyDofMoverPtr = jointMPtr->AddNoisyDofMover(dofID, inTime, fTime, fPos);
        if (foundNoise)
            noisyDofMoverPtr->SetNoise(noiseAmplitude, noiseWaveLenght);
        //~ if (foundPositionalError)
        //~ {
            //~ noisyDofMoverPtr->SetOvershoot(overshoot, peakTime);
            //~ noisyDofMoverPtr->SetOffset(offset);
        //~ }
    }
    else
        jointMPtr->AddDofMover(dofID, inTime, fTime, fPos);
}

VART::XmlAction::~XmlAction()
{
}

ostream& VART::operator<<(ostream& output, const VART::XmlAction& action)
{
    list<JointMover*>::const_iterator iter = action.jointMoverList.begin();

    output << "<?xml version=\"1.0\"?>\n"
           << "<!DOCTYPE action SYSTEM \"vartAnimation.dtd\">\n"
           << "<action action_name=\"" << action.description << "\" speed=\"" << action.speed
           << "\" cycle=\"" << boolalpha << action.cycle  << "\">\n";
    // for each joint mover
    while (iter != action.jointMoverList.end())
    {
        output << (**iter);
        ++iter;
    }
    output << "</action>\n";
    return output;
}

// Source Code for XmlAction::Source

//~ VART::XmlAction::Source::Source()
//~ {
    //~ scenePtr = NULL;
    //~ sceneNodePtr = NULL;
//~ }

VART::XmlAction::Source::Source(const Scene& scene)
{
    scenePtr = &scene;
    sceneNodePtr = NULL;
}

VART::XmlAction::Source::Source(const SceneNode& sNode)
{
    scenePtr = NULL;
    sceneNodePtr = &sNode;
}

VART::Joint* VART::XmlAction::Source::GetJointByName(const string& name) const
{
    if (scenePtr)
    {
        return dynamic_cast<Joint*>(scenePtr->GetObjectRec(name));
    }
    else
    {
        return dynamic_cast<Joint*>(sceneNodePtr->FindChildByName(name));
    }
}
