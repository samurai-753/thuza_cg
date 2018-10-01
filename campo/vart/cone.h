/// \file cone.h
/// \brief Header file for V-ART class "Cone".
/// \version $Revision: 1.0 $

#ifndef VART_CONE_H
#define VART_CONE_H

#include "vart/graphicobj.h"
#include "vart/material.h"

namespace VART {
/// \class Cone cone.h
/// \brief A cone.
///
/// Cones are created along the z axis, with bottom at zero.
/// Cones are considered a special case of cylinder, where the top or bottom has zero radius.
// It is up to the Draw methods to make the above statement true.
    class Cone : public GraphicObj {
        public:
            /// \brief Bitmask for cone parts.
            ///
            /// The bit meanings from right (lower) to left are: bottom disc and sides.
            /// IDs may combied using operators '&' and '|'.
            enum PartsID { NONE, BOTTOM, SIDES, NO_SIDES, NO_BOTTOM, ALL };

            /// \brief Creates an uninitialized cone.
            Cone();
            Cone( const VART::Cone& cone );
            VART::Cone & operator = (const VART::Cone& cone);

            /// \brief Returns a copy of an cone. Every derived class must reimplements 
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();

            Cone( float fHi, float fRad );
            Cone( float fHi, float fRad, bool bS, bool bB );

            /// Computes the bounding box.
            virtual void ComputeBoundingBox();

            void SetHeight(float h);
            /// Sets the value of bottom radius
            void SetRadius( float r );

            /// Material assignment.
            void SetMaterial(const Material& mat) { material = mat; }

            /// \brief Sets which parts are visible.
            ///
            /// All three bits present in the bitmask are copied to internal visibility state.
            void SetPartsVisibility(PartsID parts);

            /// \brief Toogle visibility of marked parts.
            ///
            /// Toggle visibility of all parts whose bits are set in given bitmask.
            void TogglePartsVisibilty(PartsID parts);

            /// \brief Returns internal visibility state as PartsID.
            PartsID GetPartsVisibility();

            /// \deprecated Use SetPartsVisibility or TogglePartsVisibilty instead.
            void    ShowSide( bool yesno );
            /// \deprecated Use SetPartsVisibility or TogglePartsVisibilty instead.
            void    ShowBottom( bool yesno );
            float   GetHeight();
            /// Returns the bottom radius
            float GetRadius();
            /// Returns of copy of the cone's material.
            Material GetMaterial() const { return material; }
            /// \deprecated Use GetPartsVisibility instead.
            bool    ShowSide();
            /// \deprecated Use GetPartsVisibility instead.
            bool    ShowBottom();
        private:
        // PRIVATE METHODS
            /// \brief Draws the cone using OpenGL engine.
            /// \return false if V-ART has not been compiled with OpenGL support.
            bool DrawInstanceOGL() const;
        // PRIVATE ATTRIBUTES
            float height;
            float topRadius;
            float btRadius;
            bool sidesVisible;
            bool topVisible;
            bool bottomVisible;
            Material material;
    }; // end class declaration
} // end namespace

// OPERATORS FOR PartsID

inline VART::Cone::PartsID operator &(VART::Cone::PartsID a, VART::Cone::PartsID b) {
    return VART::Cone::PartsID(static_cast<int>(a) & static_cast<int>(b));
}

inline VART::Cone::PartsID operator |(VART::Cone::PartsID a, VART::Cone::PartsID b) {
    return VART::Cone::PartsID(static_cast<int>(a) | static_cast<int>(b));
}

#endif
