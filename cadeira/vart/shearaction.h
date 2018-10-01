/// \file shearaction.h
/// \brief Header file for V-ART class "ShearAction".
/// \version $Revision: 1.1 $

#ifndef VART_SHEARACTION_H
#define VART_SHEARACTION_H

#include "vart/transformaction.h"

namespace VART {
/// \class ShearAction shearaction.h
/// \brief ...
///
/// FixMe: Put Detailed description here
    class ShearAction : public TransformAction
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            ShearAction();
            virtual ~ShearAction() {}
            void SetShear(double shX, double shY);
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
            virtual void Move();
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            double goalX;
            double goalY;
    }; // end class declaration
} // end namespace

#endif
