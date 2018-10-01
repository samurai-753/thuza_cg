/// \file PolyLine.h
/// \brief Header file for VART class "PolyLine".
/// \version $Revision: 1.1 $

#ifndef VART_POLYLLINE_H
#define VART_POLYLLINE_H

#include "vart/graphicobj.h"
#include "vart/material.h"
#include <vector>

/// \namespace VART
/// \brief VART namespace;
///
/// All VART classes and variables are declared inside this namespace.
namespace VART {
    /// \brief A PolyLine is a sequence of lines, defined by a list of points.
    class PolyLine : public VART::GraphicObj
    {
    public:
    // PUBLIC TYPES
        enum OrganizationType { LINES, LINE_STRIP, LINE_LOOP };

    // PUBLIC METHODS
        /// \brief Creates a empty PolyLine.
        PolyLine();

        /// \brief Creates a new copy of an PolyLine.
        PolyLine(const VART::PolyLine& polyLine);

        /// \brief Destroy all PolyLine data.
        ~PolyLine();

        /// \brief Copies all polyLine data to another PolyLine.
        VART::PolyLine& operator=(const VART::PolyLine& polyLine);

        /// \brief Return a copy of the PolyLine.
        virtual VART::SceneNode* Copy();

        /// \brief Clear all vertices in the PolyLine.
        void Clear();

        /// \brief Adds a vertex at the end of the list of vertices.
        void AddVertex(VART::Point4D& vertex) { vertexVec.push_back( vertex ); }

        /// \brief Sets the vertex at index i.
        void Setvertex(unsigned int i, VART::Point4D vertex) { vertexVec[i] = vertex; }

        /// \brief Computes the bounding box of the PolyLine.
        virtual void ComputeBoundingBox();

        /// \brief Assigns a material to all lines of the PolyLine.
        void SetMaterial(const VART::Material& material) { this->material = material; }

        /// \brief Returns the current material of the PolyLine.
        const VART::Material& GetMaterial() { return material; }

        /// \brief Sets the width of the lines.
        void SetLineWidth(float width) { this->width = width; }

        /// \brief Returns the width of the lines.
        float GetLineWidth() { return width; }

        virtual bool DrawOGL() const;

    // PUBLIC ATTRIBUTES
        /// The vertex organization
        OrganizationType organization;
    protected:
#ifdef VART_OGL
        /// \brief Returns the organization type as a GLenum
        GLenum GLOrganizationType() const;
#endif
    private:
        /// The width of all lines.
        float width;
        /// The list of points of all lines.
        std::vector< VART::Point4D > vertexVec;
        /// The material of the lines.
        VART::Material material;
    };
} // end namespace
#endif  // VART_POLYLLINE_H
