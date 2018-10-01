/// \file callback.h
/// \brief Header file for V-ART class "CallBack".
/// \version $Revision: 1.1 $

// This is an interface only class. There is no implementation file.

#ifndef VART_CALLBACK_H
#define VART_CALLBACK_H

namespace VART {
/// \class CallBack callback.h
/// \brief An object that is activated in a callback-like scheme.
///
/// A callback object is an object that implements the Activate() method. Whenever a V-ART class
/// needs to run unknown code at unkown objects, it calls the Activate of a callback object.
/// In order to use a call-back, derive the class, adding attributes (usually pointers to the
/// objects that will be activated) and reimplement the Activate method.
    class CallBack
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            virtual void Activate()=0;
            virtual ~CallBack() {}
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
    }; // end class declaration
} // end namespace

#endif
