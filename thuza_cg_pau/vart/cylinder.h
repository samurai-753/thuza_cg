/// \file cylinder.h
/// \brief Header file for V-ART class "Cylinder".
/// \version $Revision: 1.3 $

#ifndef VART_CYLINDER_H
#define VART_CYLINDER_H

#include "vart/graphicobj.h"
#include "vart/material.h"

namespace VART {
/// \class Cylinder cylinder.h
/// \brief A cylinder or cone.
///
/// Cylinders are created along the z axis, with bottom at zero and top at height.
/// Cones are considered a special case of cylinder, where the top or bottom has zero radius.
// It is up to the Draw methods to make the above statement true.
    class Cylinder : public GraphicObj {
        public:
            /// \brief Bitmask for cylinder parts.
            ///
            /// The bit meanings from right (lower) to left are: bottom disc, sides and top disc.
            /// IDs may combied using operators '&' and '|'.
            enum PartsID { NONE, BOTTOM, SIDES, NO_TOP, TOP, NO_SIDES, NO_BOTTOM, ALL };

            /// \brief Creates an uninitialized cylinder.
            Cylinder();
            Cylinder( const VART::Cylinder& cylinder );
            VART::Cylinder & operator = (const VART::Cylinder& cylinder);

            /// \brief Returns a copy of an cylinder. Every derived class must reimplements 
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();

            Cylinder( float fHi, float fRad );
            Cylinder( float fHi, float fRad, bool bS, bool bT, bool bB );

            /// Computes the bounding box.
            virtual void ComputeBoundingBox();

            void SetHeight(float h);
            /// Sets the value of top and bottom radius
            void SetRadius( float r );
            /// Sets the value of the top radius
            void SetTopRadius(float r) { topRadius = r; }
            /// Sets the value of the bottom radius
            void SetBottomRadius(float r){ btRadius = r; }

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
            void    ShowTop( bool yesno );
            /// \deprecated Use SetPartsVisibility or TogglePartsVisibilty instead.
            void    ShowBottom( bool yesno );
            float   GetHeight();
            /// Returns the top radius
            float GetTopRadius();
            /// Returns the bottom radius
            float GetBottomRadius();
            /// Returns of copy of the cylinder's material.
            Material GetMaterial() const { return material; }
            /// \deprecated Use GetPartsVisibility instead.
            bool    ShowSide();
            /// \deprecated Use GetPartsVisibility instead.
            bool    ShowTop();
            /// \deprecated Use GetPartsVisibility instead.
            bool    ShowBottom();
        private:
        // PRIVATE METHODS
            /// \brief Draws the cylinder using OpenGL engine.
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
inline VART::Cylinder::PartsID operator &(VART::Cylinder::PartsID a, VART::Cylinder::PartsID b) {
    return VART::Cylinder::PartsID(static_cast<int>(a) & static_cast<int>(b));
}

inline VART::Cylinder::PartsID operator |(VART::Cylinder::PartsID a, VART::Cylinder::PartsID b) {
    return VART::Cylinder::PartsID(static_cast<int>(a) | static_cast<int>(b));
}

#endif
