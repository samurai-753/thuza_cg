/// \file /home/bruno/data/cvs/vpat/vart/descriptionlocator.h
/// \brief Header file for V-ART class "DescriptionLocator".
/// \version $Revision: 1.1 $

#ifndef VART_DESCRIPTIONLOCATOR_H
#define VART_DESCRIPTIONLOCATOR_H

#include "vart/snlocator.h"
#include <string>

namespace VART {
/// \class DescriptionLocator /home/bruno/data/cvs/vpat/vart/descriptionlocator.h
/// \brief Locates a SceneNode by its description.
///
/// FixMe: Put Detailed description here
    class DescriptionLocator : public SNLocator
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            DescriptionLocator(const std::string& description);
            virtual void OperateOn(const SceneNode* snPtr);
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
        std::string targetDescription;
    }; // end class declaration
} // end namespace

#endif
