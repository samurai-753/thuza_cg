/// \file ./offsetmodifier.h
/// \brief Header file for V-ART class "OffsetModifier".
/// \version $Revision: 1.1 $

#ifndef VART_OFFSETMODIFIER_H
#define VART_OFFSETMODIFIER_H

#include "vart/joint.h"
#include "vart/dmmodifier.h"
#include <map>
#include <string>

namespace VART {
/// \class OffsetModifier offsetmodifier.h
/// \brief ...
///
/// FixMe: Put Detailed description here
    class OffsetModifier : public DMModifier
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            OffsetModifier();
            virtual void Modify(NoisyDofMover* moverPtr);
            void LoadFromFile(const std::string& fileName);
            float weight;
        protected:
            typedef std::map<Joint::DofID, float> DofID2Offset;
            typedef std::map<std::string, DofID2Offset> Joint2DofIDs;
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            Joint2DofIDs offsetMap;
    }; // end class declaration
} // end namespace

#endif
