/// \file modifier.h
/// \brief Header file for V-ART class "Modifier".
/// \version $Revision: 1.2 $

#ifndef VART_MODIFIER_H
#define VART_MODIFIER_H

#include "vart/bezier.h"

namespace VART {
    class Dof;
/// \class Modifier modifier.h
/// \brief A modifier controls how one DOF influences other DOF
///
/// FixMe: Put a detailed description here.
    class Modifier {
        public:
            Modifier();
            Modifier( Dof **dofs, Curve *mins, Curve *maxs, int numD );
            ~Modifier();
            void    SetMaxList( Curve *list );
            void    SetMinList( Curve *list );
            void    SetDofList( Dof **list );
            Curve *GetMinPonderatorList();
            Curve *GetMaxPonderatorList();
            float   GetMin();
            float   GetMax();
        private:
            Curve *maxPonderatorList;
            Curve *minPonderatorList;
            Dof       **dofList;
            int         numDofs;
    }; // end class declaration
} // end namespace
#endif
