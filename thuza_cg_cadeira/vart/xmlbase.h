/// \file xmlbase.h
/// \brief Header file for V-ART class "XmlBase".
/// \version $Revision: 1.6 $

#ifndef VART_XMLBASE_H
#define VART_XMLBASE_H

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <string>

namespace VART {
/// \class XmlBase xmlbase.h
/// \brief Basic methods for reading XML files.
///
/// FixMe: Put Detailed description here
    class XmlBase
    {
        public:
        // PUBLIC METHODS
        protected:
        // PROTECTED NESTED CLASSES
            /// \brief Provides pretty errors messages for Xerces
            class ErrorHandler : public XERCES_CPP_NAMESPACE::ErrorHandler
            {
                friend class XmlScene;
                public:
                    ErrorHandler();
                    ~ErrorHandler();
                    bool getSawErrors() const;
                    void warning(const XERCES_CPP_NAMESPACE::SAXParseException& e);
                    void error(const XERCES_CPP_NAMESPACE::SAXParseException& e);
                    void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& e);
                    void resetErrors();

                private :
                    ErrorHandler(const ErrorHandler&);
                    void operator=(const ErrorHandler&);
                    //This is set if we get any errors, and is queryable via a getter
                    //method. Its used by the main code to suppress output if there are
                    //errors.
                    bool fSawErrors;
            }; // XmlBase::ErrorHandler

            /// \brief Class for auto conversion from Xerces strings to C strings.
            class TempCString
            {
                public :
                    TempCString(const XMLCh* const toTranscode)
                    {
                        // Call the private transcoding method
                        cString = XERCES_CPP_NAMESPACE::XMLString::transcode(toTranscode);
                    }
                    ~TempCString()
                    {
                        XERCES_CPP_NAMESPACE::XMLString::release(&cString);
                    }
                    /// Check against a C string
                    bool operator== (const char* other) const;

                    bool GetValue(int* valuePtr) const;
                    bool GetValue(float* valuePtr) const;
                    bool GetValue(bool* valuePtr) const;

                    /// Automatic cast to char*
                    operator const char* () const
                    {
                        return cString;
                    }
                private :
                    //This is the local code page form of the string.
                    char* cString;
            }; // XmlBase::TempCString

            /// \brief Class for auto conversion from C strings to Xerces strings.
            class XercesString
            {
                public:
                    XercesString(const char* cString)
                    {
                        xString = XERCES_CPP_NAMESPACE::XMLString::transcode(cString);
                    }
                    ~XercesString()
                    {
                        XERCES_CPP_NAMESPACE::XMLString::release(&xString);
                    }
                    /// Automatic cast to XMLCh*
                    operator const XMLCh* () const
                    {
                        return xString;
                    }
                private:
                    XMLCh* xString;
            }; // XmlBase::XercesString;

        // PROTECTED STATIC METHODS
            /// \brief Gets an item value for certain node.
            /// \param nodePtr   [in] node that holds the item
            /// \param itemName  [in] name of wanted item
            /// \param valuePtr [out] value of wanted item
            /// \return false on errors.
            static bool GetAttributeValue(XERCES_CPP_NAMESPACE::DOMNamedNodeMap* nodePtr,
                                          const char itemName[], float* valuePtr);

            /// \brief Gets an item value for certain node.
            /// \param nodePtr   [in] node that holds the item
            /// \param itemName  [in] name of wanted item
            /// \param valuePtr [out] value of wanted item
            /// \return false on errors.
            static bool GetAttributeValue(XERCES_CPP_NAMESPACE::DOMNamedNodeMap* nodePtr,
                                          const char itemName[], std::string* valuePtr);

            /// \brief Gets an item value for certain node.
            /// \param nodePtr   [in] node that holds the item
            /// \param itemName  [in] name of wanted item
            /// \param valuePtr [out] value of wanted item
            /// \return false on errors.
            static bool GetAttributeValue(XERCES_CPP_NAMESPACE::DOMNamedNodeMap* nodePtr,
                                          const char itemName[], bool* valuePtr);

        // PROTECTED METHODS
            /// \brief Creates an unitialized object.
            XmlBase();

            /// \brief Checks if the file comforms to DTD.
            bool ParseFile(const std::string& fileName);

            /// \brief Terminates Xerces objects.
            void Terminate();
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            XERCES_CPP_NAMESPACE::XercesDOMParser* parserPtr;
            XERCES_CPP_NAMESPACE::DOMDocument* documentPtr;
    }; // end class declaration
} // end namespace

#endif
