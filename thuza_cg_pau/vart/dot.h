/// \file dot.h
/// \brief Header file for V-ART class "Dot".
/// \version $Revision: 1.3 $

#ifndef VART_DOT_H
#define VART_DOT_H

#include "vart/graphicobj.h"
#include "vart/color.h"
#include "vart/point4d.h"

namespace VART {
/// \class Dot dot.h
/// \brief A graphical point.
///
/// A dot is a point that can rendered on screen as part of a scene graph.
    class Dot : public GraphicObj {
        public:
        // PUBLIC METHODS
            /// \brief Creates an initialized point.
            Dot();
            
            /// \brief Creates a new copy of an dot.
            Dot(const VART::Dot& dot);
            
            /// \brief Copies all data from an dot to another.
            VART::Dot& operator=(const VART::Dot& dot);
            
            /// \brief Returns a copy of an dot. Every derived class must reimplements 
            //  this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();

            /// \brief Creates a point at a specific location.
            Dot(const Point4D& location);

            /// \brief Computes the bounding box.
            virtual void ComputeBoundingBox();

        // PUBLIC ATTRIBUTES
            /// \brief The point's position.
            Point4D position;

            /// \brief The size (in pixels) of the point on screen.
            float size; // FixMe: better turn it into a class attribute

            /// \brief The point's color.
            Color color;

        protected:
            /// Draws the point.
            virtual bool DrawInstanceOGL() const;
    }; // end class declaration
} // end namespace
#endif
