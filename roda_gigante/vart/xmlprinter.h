/// \file xmlprinter.h
/// \brief Header file for V-ART class "XmlPrinter".
/// \version $Revision$

#ifndef VART_XMLPRINTER_H
#define VART_XMLPRINTER_H

#include "vart/snoperator.h"
#include <iostream>

namespace VART {
    class Joint;
/// \class XmlPrinter xmlprinter.h
/// \brief ...
///
/// FixMe: Put Detailed description here
    class XmlPrinter : public SNOperator
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            XmlPrinter();
            virtual void OperateOn(SceneNode* nodePtr);
            virtual void OperateOn(Joint* nodePtr);
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
            static std::ostream* outputPtr;
        // PROTECTED ATTRIBUTES
    }; // end class declaration
} // end namespace

#endif
