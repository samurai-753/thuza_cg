/// \file /home/bruno/data/cvs/vpat/vart/addresslocator.h
/// \brief Header file for V-ART class "AddressLocator".
/// \version $Revision: 1.1 $

#ifndef VART_ADDRESSLOCATOR_H
#define VART_ADDRESSLOCATOR_H

#include "vart/snlocator.h"

namespace VART {
/// \class AddressLocator /home/bruno/data/cvs/vpat/vart/addresslocator.h
/// \brief Locates a SceneNode by its memory address.
///
/// FixMe: Put Detailed description here
        class AddressLocator : public SNLocator
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            AddressLocator(const SceneNode* targetPtr);
            virtual void OperateOn(const SceneNode* nodePtr);
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            const SceneNode* targetAddress;
    }; // end class declaration
} // end namespace

#endif
