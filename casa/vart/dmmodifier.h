/// \file /home/bruno/data/cvs/vpat/vart/dmmodifier.h
/// \brief Header file for V-ART class "DMModifier".
/// \version $Revision: 1.1 $

#ifndef VART_DMMODIFIER_H
#define VART_DMMODIFIER_H

namespace VART {
    class NoisyDofMover;
/// \class DMModifier /home/bruno/data/cvs/vpat/vart/dmmodifier.h
/// \brief An object that modifies noisy dof movers
///
/// A Noisy Dof Mover Modifier is used to modify Noisy Dof Movers inside an Action.
/// In order to use a DMModifier, derive the class, implementing the Modify() method.
    class DMModifier
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            DMModifier() {}
            virtual ~DMModifier() {}
            virtual void Modify(NoisyDofMover* moverPtr) = 0;
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
    }; // end class declaration
} // end namespace

#endif
