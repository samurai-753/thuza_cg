/// \file xmlbase.cpp
/// \brief Implementation file for V-ART class "XmlBase".
/// \version $Revision: 1.4 $

#include "vart/xmlbase.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <iostream>
#include <sstream>
#include <cstring>

using XERCES_CPP_NAMESPACE::XercesDOMParser;
using XERCES_CPP_NAMESPACE::XMLPlatformUtils;
using XERCES_CPP_NAMESPACE::XMLException;
using XERCES_CPP_NAMESPACE::OutOfMemoryException;
using XERCES_CPP_NAMESPACE::DOMException;
using XERCES_CPP_NAMESPACE::DOMImplementation;
using namespace std;

VART::XmlBase::XmlBase()
{
    parserPtr = NULL;
    documentPtr = NULL;
}

bool VART::XmlBase::ParseFile(const std::string& fileName)
{
    static XercesDOMParser::ValSchemes ValScheme = XercesDOMParser::Val_Auto; //Val_Never Val_Always
    bool bDoNamespaces       = false;
    bool bDoSchema           = false;
    bool bSchemaFullChecking = false;
    bool bDoCreate           = false;
    ErrorHandler* errorHPtr;

    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch(const XMLException &toCatch)
    {
        cerr << "Error during Xerces-c Initialization.\n"
            << "  Exception message:"
            << TempCString(toCatch.getMessage()) << endl;
        return false;
    }

    if (parserPtr)
        delete parserPtr;

    parserPtr = new XercesDOMParser;
    parserPtr->setValidationScheme(ValScheme);
    parserPtr->setDoNamespaces(bDoNamespaces);
    parserPtr->setDoSchema(bDoSchema);
    parserPtr->setValidationSchemaFullChecking(bSchemaFullChecking);
    parserPtr->setCreateEntityReferenceNodes(bDoCreate);


    // Create the error handler and install it
    errorHPtr = new ErrorHandler;
    parserPtr->setErrorHandler(errorHPtr);

    //  Parse the XML file, catching any XML exceptions that might propogate
    //  out of it.
    try
    {
        parserPtr->parse(fileName.c_str());
    }
    catch (const OutOfMemoryException&)
    {
        cerr << "OutOfMemoryException" << endl;
        delete errorHPtr;
        return false;
    }
    catch (const XMLException& e)
    {
        cerr << "An error occurred during parsing\n   Message: "
             << TempCString(e.getMessage()) << endl;
        delete errorHPtr;
        return false;
    }
    catch (const DOMException& e)
    {
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        cerr << "\nDOM Error during parsing: '" << fileName << "'\n"
             << "DOMException code is:  " << e.code << endl;
        if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
             cerr << "Message is: " << TempCString(errText) << endl;
        delete errorHPtr;
        return false;
    }
    catch (...)
    {
        cerr << "An error occurred during parsing\n " << endl;
        delete errorHPtr;
        return false;
    }
    if (errorHPtr->getSawErrors())
    {
        //  Clean up the error handler. The parser does not adopt handlers
        //  since they could be many objects or one object installed for multiple
        //  handlers.
        delete errorHPtr;
        return false;
    }
    else
    {
        //...clean up the error handler. The parser does not adopt handlers
        //  since they could be many objects or one object installed for multiple
        //  handlers.
        delete errorHPtr;

        // Since parsing was sucessful, intialize the document pointer, i.e.:
        // extract the DOM tree, get the list of all the elements.
        documentPtr = parserPtr->getDocument();
        return true;
    }
}

void VART::XmlBase::Terminate()
{
    //Delete the parser itself.  Must be done prior to calling Terminate, below.
    if (parserPtr)
        delete parserPtr;
    //Call the termination method
    XMLPlatformUtils::Terminate();

}

//static
bool VART::XmlBase::GetAttributeValue(XERCES_CPP_NAMESPACE::DOMNamedNodeMap* nodePtr,
                                      const char itemName[], float* valuePtr)
{
    
    TempCString str(nodePtr->getNamedItem(XercesString(itemName))->getNodeValue());
    return str.GetValue(valuePtr);
}

//static
bool VART::XmlBase::GetAttributeValue(XERCES_CPP_NAMESPACE::DOMNamedNodeMap* nodePtr,
                                      const char itemName[], string* valuePtr)
{
    (*valuePtr) = TempCString(nodePtr->getNamedItem(XercesString(itemName))->getNodeValue());
    return true;
}

//static
bool VART::XmlBase::GetAttributeValue(XERCES_CPP_NAMESPACE::DOMNamedNodeMap* nodePtr,
                                      const char itemName[], bool* valuePtr)
{
    TempCString str(nodePtr->getNamedItem(XercesString(itemName))->getNodeValue());
    return str.GetValue(valuePtr);
}

bool VART::XmlBase::TempCString::operator== (const char* other) const
{
    return (strcmp(cString,other) == 0);
}

bool VART::XmlBase::TempCString::GetValue(int* valuePtr) const
{
    istringstream inputStream;

    inputStream.str(cString);
    inputStream >> (*valuePtr);
    return inputStream.good();
}


bool VART::XmlBase::TempCString::GetValue(float* valuePtr) const
{
    istringstream inputStream;

    inputStream.str(cString);
    inputStream >> (*valuePtr);
    return inputStream.good();
}

bool VART::XmlBase::TempCString::GetValue(bool* valuePtr) const
{
    string value(cString);
    if ((value == "true") || (value == "1"))
    {
        *valuePtr = true;
        return true;
    }
    if ((value == "false") || (value == "0"))
    {
        *valuePtr = false;
        return true;
    }
    return false;
}

VART::XmlBase::ErrorHandler::ErrorHandler() : fSawErrors(false)
{
}

VART::XmlBase::ErrorHandler::~ErrorHandler()
{
}

//Overrides of the SAX ErrorHandler interface
void VART::XmlBase::ErrorHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nError at file " << TempCString(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << TempCString(e.getMessage()) << endl;
}

void VART::XmlBase::ErrorHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nFatal Error at file " << TempCString(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << TempCString(e.getMessage()) << endl;
}

void VART::XmlBase::ErrorHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException& e)
{
    cerr << "\nWarning at file '" << TempCString(e.getSystemId())
         << "', line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << TempCString(e.getMessage()) << endl;
}

void VART::XmlBase::ErrorHandler::resetErrors()
{
    fSawErrors = false;
}

bool VART::XmlBase::ErrorHandler::getSawErrors() const
{
    return fSawErrors;
}
