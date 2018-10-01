/// \file interpolator.h
/// \brief Header file for V-ART class "Interpolator".
/// \version $Revision: 1.5 $

// There is no associated implementation file (no interpolator.cpp).

#ifndef VART_INTERPOLATOR_H
#define VART_INTERPOLATOR_H

namespace VART {
/// \class Interpolator interpolator.h
/// \brief Interpolator representation
///
/// A interpolator is a function whose domain is [0..1] and range is [0..1].
/// It is used to describe how a DOF (see Dof) should be changed in time inside an 
/// animation (see Action).
    class Interpolator {
        public:
        // PUBLIC TYPES
            enum TypeID { LINEAR, EASE_IN_EASE_OUT, RANGED, HERMITE };
        // PUBLIC METHODS
            Interpolator() {}
            virtual ~Interpolator() {}
            // \brief Evaluates the interpolator
            // \param n [in] A number in range [0..1]. Usually related to a linear time function.
            // \return A new interpolator value that has specific properties.
            // An interpolator basically converts a linear interpolation (GetValue input) into other, more
            // sophisticated interpolation (GetValue output). Each derived class implents a
            // specific mapping.
            //virtual float GetValue(float n) const = 0;

            /// \brief Evaluates a goal position for DOF interpolation
            /// \param linearIndex [in] A number in range [0..1]. Usually related to a linear time function.
            /// \param initialPos [in] A number in range [0..1]. DOF's initial movement position.
            /// \param range [in] DOF's position range on movement.
            /// \return DOF's current position.
            /// An interpolator basically converts a linear interpolation (GetValue input) into other, more
            /// sophisticated interpolation (GetValue output). Each derived class implents a
            /// specific mapping.
            virtual float GetValue(float linearIndex, float initialPos, float range) const = 0;
            virtual TypeID GetID() const = 0;
    }; // end class declaration
} // end namespace

#endif
