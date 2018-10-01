/// \file xmlprinter.cpp
/// \brief Implementation file for V-ART class "XmlPrinter".
/// \version $Revision$

#include "vart/xmlprinter.h"
#include "vart/scenenode.h"
#include "vart/dof.h"

using namespace std;

ostream* VART::XmlPrinter::outputPtr = &cout;

VART::XmlPrinter::XmlPrinter()
{
}

void VART::XmlPrinter::OperateOn(SceneNode* nodePtr)
// virtual
{
    cerr << "Unimplemented XmlPrint for " << nodePtr->GetID() << endl;
}

void VART::XmlPrinter::OperateOn(Joint* jointPtr)
// virtual
{
    ostream& output = *outputPtr;
    list<Dof*>::const_iterator dofIter = dofList.begin();
    list<SceneNode*>::const_iterator iter = childList.begin();
    string indentStr(indent,' ');

    output << indentStr << "<joint description=\"" << description << "\" type=\"";
    switch (GetNumDofs())
    {
        case 1:
            output << "uniaxial";
            break;
        case 2:
            output << "biaxial";
            break;
        default:
            output << "poliaxial";
    }
    output << "\">\n";
    //~ while (dofIter != dofList.end())
    //~ {
        //~ (*dofIter)->XmlPrintOn(output, indent+2);
        //~ ++dofIter;
    //~ }
    //~ while (iter != childList.end())
    //~ {
        //~ (*iter)->XmlPrintOn(output, indent+2);
        //~ ++iter;
    //~ }
    output << indentStr << "</joint>\n";
    output << flush;
}
