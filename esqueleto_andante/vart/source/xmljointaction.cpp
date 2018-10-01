/// \file xmljointaction.cpp
/// \brief Implementation file for V-ART class "XmlJointAction".
/// \version $Revision: 1.1 $

#include "vart/xmljointaction.h"
#include "vart/joint.h"
#include "vart/noisydofmover.h"
#include "vart/jointmover.h"
#include "vart/linearinterpolator.h"
#include "vart/sineinterpolator.h"
#include "vart/rangesineinterpolator.h"
#include "vart/descriptionlocator.h"
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMElement.hpp>


//#include <iostream>
using namespace std;

VART::XmlJointAction::XmlJointAction()
{
    cout << "XmlJointAction::XmlJointAction()" << endl;
}

void VART::XmlJointAction::ParseDofMovement(JointMover* jointMPtr,
                                            XERCES_CPP_NAMESPACE::DOMNode* dofMovItemPtr)
// Called by XmlJointAction::CreateJointMover to create a dof mover from a XML entry.
{
    using namespace XERCES_CPP_NAMESPACE;

    DOMNamedNodeMap* dofAttrs = dofMovItemPtr->getAttributes();
    // read initial time
    float inTime;
    GetAttributeValue(dofAttrs, "initialTime", &inTime);
    // read final time
    float fTime;
    GetAttributeValue(dofAttrs, "finalTime", &fTime);
    // read final position
    float fPos;
    GetAttributeValue(dofAttrs, "finalPosition", &fPos);
    // Read value for dofID
    string dofIDString;
    GetAttributeValue(dofAttrs, "dofID", &dofIDString);
    VART::Joint::DofID dofID;
    if ((dofIDString == "FLEX") || (dofIDString == "FLEXION"))
        dofID = VART::Joint::FLEXION;
    else
    {
        if ((dofIDString == "ADDUCT") || (dofIDString == "ADDUCTION"))
            dofID = VART::Joint::ADDUCTION;
        else
        {
            if (dofIDString == "TWIST")
                dofID = VART::Joint::TWIST;
            else
            {
                cerr << "XmlAction::ParseDofMovement: Unknown DOF ID (" << dofIDString << ")."
                     << endl;
                exit (1);
            }
        }
    }

    // Treat child elements of dof movers
    DOMNodeList* dofMovElements = dofMovItemPtr->getChildNodes();
    float noiseAmplitude;
    float noiseWaveLenght;
    bool foundNoise = false;           // any noise elements found inside the dof mover?
    bool foundPositionalError = false; // any positional error elements found inside the dof mover?
    // for each child node of the dof mover (may be "noise" or "error"):
    for (unsigned int count = 0; count < dofMovElements->getLength(); ++count)
    {
        string nodeName(TempCString(dofMovElements->item(count)->getNodeName()));
        if (nodeName == "noise")
        {
            DOMNamedNodeMap* noiseAttrs = dofMovElements->item(count)->getAttributes();
            GetAttributeValue(noiseAttrs, "amplitude", &noiseAmplitude);
            GetAttributeValue(noiseAttrs, "length", &noiseWaveLenght);
            foundNoise = true;
        }
        else
            if (nodeName == "error")
            {
                DOMNamedNodeMap* errorAttrs = dofMovElements->item(count)->getAttributes();

                float overshoot;
                GetAttributeValue(errorAttrs, "overshoot", &overshoot);
                NoisyDofMover::SetDefaultOvershoot(overshoot);

                float offset;
                GetAttributeValue(errorAttrs, "offset", &offset);
                NoisyDofMover::SetDefaultOffset(offset);

                float peakTime;
                GetAttributeValue(errorAttrs, "peak_time", &peakTime);
                NoisyDofMover::SetDefaultPeakTime(peakTime);

                foundPositionalError = true;
            }
    }

    // Ready to create a dof mover
    if (foundPositionalError || foundNoise)
    {
        VART::NoisyDofMover* noisyDofMoverPtr;

        noisyDofMoverPtr = jointMPtr->AddNoisyDofMover(dofID, inTime, fTime, fPos);
        if (foundNoise)
            noisyDofMoverPtr->SetNoise(noiseAmplitude, noiseWaveLenght);
    }
    else
        jointMPtr->AddDofMover(dofID, inTime, fTime, fPos);
}

void VART::XmlJointAction::CreateJointMover(const VART::SceneNode& sNode,
                                            XERCES_CPP_NAMESPACE::DOMNode* xmlNodePtr)
{
    using namespace XERCES_CPP_NAMESPACE;

    static VART::LinearInterpolator linearInterpolator; // common interpolator for all joint movers
    static VART::SineInterpolator sineInterpolator;     // common interpolator for all joint movers

    DOMNamedNodeMap* jmAttrs = xmlNodePtr->getAttributes();
    // Read movement duration
    float moverDuration;
    GetAttributeValue(jmAttrs, "duration", &moverDuration);
    // Read target (joint) name
    string jointName;
    GetAttributeValue(jmAttrs, "joint_name", &jointName);
    // Get a pointer to target joint
    DescriptionLocator locator(jointName); // FixMe: Can we relay on description uniqueness?
    sNode.LocateBreadthFirst(&locator);
    Joint* targetJointPtr = const_cast<Joint*>(dynamic_cast<const Joint*>(locator.LocatedNode()));
    if (targetJointPtr)
    {
        JointMover* jointMoverPtr = AddJointMover(targetJointPtr, moverDuration, sineInterpolator);
        DOMNodeList* childrenList = xmlNodePtr->getChildNodes();
        // For each child of a joint movement (may be interpolator or dof movement):
        for (unsigned int i=0; i < childrenList->getLength(); ++i)
        {
            string name(TempCString(childrenList->item(i)->getNodeName()));
            if (name == "interpolation")
            {
                DOMNamedNodeMap* itpAttrs = childrenList->item(i)->getAttributes();
                // Read interpolation type
                string itpType;
                GetAttributeValue(itpAttrs, "type", &itpType);
                // Default is a SineInterpolator. Change interpolator if different from default.
                if (itpType != "ease-in_ease-out")
                {
                    if (itpType == "linear")
                        jointMoverPtr->SetInterpolator(linearInterpolator);
                    else
                    {
                        if (itpType == "range_sine")
                        {
                            // FixMe: Why isn't the RangeSineInterpolator static like others?
                            Interpolator* interpolatorPtr = new RangeSineInterpolator;
                            jointMoverPtr->SetInterpolator(*interpolatorPtr);
                        }
                        else
                            cerr << "Error: Unknown interpolator type: '" << itpType
                                 << "'. Using: 'ease-in_ease-out'." << endl;
                    }
                }
            }
            else // child (of joint mover) is a dof movement
            {
                if (name == "dof_movement")
                    ParseDofMovement(jointMoverPtr, childrenList->item(i));
            }
        }
    }
}

bool VART::XmlJointAction::LoadFromFile(const std::string& fileName, const VART::SceneNode& sNode)
{
    using namespace XERCES_CPP_NAMESPACE;

    //~ cout << "XmlJointAction loading from " << fileName << endl;
    if(ParseFile(fileName))
    {   // XML parser sucessfull
        // Get action attributes (getDocumentElement returns a pointer to the root element)
        DOMNamedNodeMap* actionAttrs = documentPtr->getDocumentElement()->getAttributes();
        // Read action name
        bool result = GetAttributeValue(actionAttrs, "action_name", &description);
        // Read action speed
        result |= GetAttributeValue(actionAttrs, "speed", &speedModifier);
        // Read cycle flag (boolean)
        result |= GetAttributeValue(actionAttrs, "cycle", &cyclic);
        // Load joint movers
        DOMNodeList* jmList = documentPtr->getDocumentElement()->getChildNodes();
        // For each joint movement:
        for (unsigned int jm = 0; jm < jmList->getLength(); ++jm)
        {
            string name(TempCString(jmList->item(jm)->getNodeName()));
            if (name == "joint_movement")
                CreateJointMover(sNode, jmList->item(jm));
        }
        Terminate();
        return result;
    }
    else
    {   //the parser has failled!
        Terminate();
        return false;
    }
}

