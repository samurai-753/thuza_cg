/// \file scaleaction.h
/// \brief Header file for V-ART class "ScaleAction".
/// \version $Revision: 1.1 $

#ifndef VART_SCALEACTION_H
#define VART_SCALEACTION_H

#include "vart/transformaction.h"

namespace VART {
/// \class ScaleAction scaleaction.h
/// \brief ...
///
/// FixMe: Put Detailed description here
    class ScaleAction : public TransformAction
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            ScaleAction();
            virtual ~ScaleAction() {}
            void SetScale(double sX, double sY, double sZ);
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
            virtual void Move();
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            double goalX;
            double goalY;
            double goalZ;
    }; // end class declaration
} // end namespace

#endif
