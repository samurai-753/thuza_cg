/// \file translationaction.h
/// \brief Header file for V-ART class "TranslationAction".
/// \version $Revision: 1.1 $

#ifndef VART_TRANSLATIONACTION_H
#define VART_TRANSLATIONACTION_H

#include "vart/transformaction.h"
#include "vart/point4d.h"

namespace VART {
/// \class TranslationAction translationaction.h
/// \brief ...
///
/// FixMe: Put Detailed description here
    class TranslationAction : public TransformAction
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            TranslationAction();
            virtual ~TranslationAction() {}
            void SetTranslation(double tx, double ty, double tz);
            void SetTranslation(const Point4D& vector);
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
            virtual void Move();
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            /// \brief A vector that describes the final Translation.
            Point4D goal;
    }; // end class declaration
} // end namespace

#endif
