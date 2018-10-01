/// \file boundingbox.h
/// \brief Header file for V-ART class "BoundingBox".
/// \version $Revision: 1.6 $

#ifndef VART_BOUNDINGBOX_H
#define VART_BOUNDINGBOX_H

#include "vart/point4d.h"
#include "vart/color.h"
#include <GL/glut.h>
#include <iostream>

/// \namespace VART
/// \brief V-ART namespace;
///
/// All V-ART classes and variables are declared inside this namespace.
namespace VART {
    class Transform;
/// \class BoundingBox boundingbox.h
/// \brief Axis aligned bounding box.
///
/// FixMe: A detailed class description is needed here.
    class BoundingBox {
        /// Output operator
        // FixMe: Perhaps this will cause trouble on an application with no console...
        friend std::ostream& operator<<(std::ostream& output, const BoundingBox& box);
        public:
            /// Creates an uninitialized bounding box.
            BoundingBox();
            BoundingBox(double minX, double minY, double minZ,
                          double maxX, double maxY, double maxZ);
            BoundingBox(const BoundingBox& box);

            BoundingBox& operator=(const BoundingBox& box);
            void ProcessCenter();

            /// \brief Applies transformation to a BoundingBox.
            /// \param trans [in] transformation matrix to be applied in bbox
            void ApplyTransform(const Transform& trans);

            double GetGreaterX() const;
            double GetGreaterY() const;
            double GetGreaterZ() const;

            /// \brief Returns the length of the greater edge.
            double GetGreaterEdge() const;
            /// \brief Returns the length of the smaller edge.
            double GetSmallerEdge() const;
            /// \brief Returns the length of the X edge.
            double GetEdgeX() const    { return greaterX-smallerX; }
            /// \brief Returns the length of the Y edge.
            double GetEdgeY() const    { return greaterY-smallerY; }
            /// \brief Returns the length of the Z edge.
            double GetEdgeZ() const    { return greaterZ-smallerZ; }

            void SetGreaterX(double v);
            void SetGreaterY(double v);
            void SetGreaterZ(double v);

            double GetSmallerX() const;
            double GetSmallerY() const;
            double GetSmallerZ() const;

            void SetSmallerX(double v);
            void SetSmallerY(double v);
            void SetSmallerZ(double v);

            void SetBoundingBox(double minX, double minY, double minZ,
                                double maxX, double maxY, double maxZ);

            const Point4D& GetCenter() const;

            /// Updates the bounding box if any given coordinate is outside it.
            void ConditionalUpdate(double x, double y, double z);
            /// Updates the bounding box if any given coordinate is outside it.
            void ConditionalUpdate(const Point4D& point);

            /// Merges a bounding with another, expanding it.
            void MergeWith(const BoundingBox& box);
            /// Draws a bounding box.
            bool DrawInstanceOGL() const;
            /// \brief Sets the bounding box visibility.
            void SetVisibility(const bool value)    { visible = value; };
            /// Toggles the visibility.
            void ToggleVisibility();
            /// Test intersection among AABBs
            bool testAABBAABB(BoundingBox &b);
            /// Test if a point is included in the bbox
            bool testPoint( VART::Point4D p );
            /// Indicates wether the bounding box is visible.
            bool visible;
            /// \brief Sets the bounding box color.
            void SetColor(const Color& value)     { color = value; }
            /// Copies geometry data from other bounding box.
            void CopyGeometryFrom(const BoundingBox& box);
            /// Test against other bounding box and update itself if its outside the given bbox.
            void CutBBox( const BoundingBox& box );
        private:
            double greaterX, greaterY, greaterZ;
            double smallerX, smallerY, smallerZ;
            Point4D center;
            Color color;
    }; // end class declaration
} // end namespace
#endif  // VART_BOUNDINGBOX_H
