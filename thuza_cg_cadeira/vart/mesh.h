/// \file mesh.h
/// \brief Header file for V-ART class "Mesh".
/// \version $Revision: 1.3 $

#ifndef VART_MESH_H
#define VART_MESH_H

#include "vart/point4d.h"
#include "vart/material.h"
#include <vector>

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/gl.h>
#endif

namespace VART {
/// \class Mesh mesh.h
/// \brief A mesh is part of an graphical object, in which faces are made of
/// vertices according to some rule of formation.
///
/// The mesh is the basic building block of an mesh object (MeshObject). It has a single
/// material, and indexes to vertices that define faces according to a rule (on
/// a triangle strip, for instance, the first 3 indexes from a face, and then each
/// new index from a new face together with the last 2 indexes).
/// A mesh degenerates to a single polygon if the mesh object has no pratical
/// rule of formation.
    class Mesh {
        friend std::ostream& operator<<(std::ostream& output, const Mesh& mesh);
        public:
        // PUBLIC TYPES
            enum MeshType { NONE, POINTS, LINES, LINE_STRIP, LINE_LOOP, TRIANGLES, TRIANGLE_STRIP,
                            TRIANGLE_FAN, QUADS, QUAD_STRIP, POLYGON };
            friend std::ostream& operator<<(std::ostream& output, const MeshType mt);
        // PUBLIC METHODS
            /// Creates an uninitialized mesh.
            Mesh();
            Mesh(const Mesh& mesh);
            //~ Mesh(MeshType type);
            Mesh& operator=(const Mesh& mesh);

            /// \brief Draws the mesh assuming that its MeshObject is optimized.
            /// \return false if V-ART was not compiled with OpenGL support.
            bool DrawInstanceOGL() const;

            // \brief Draws the mesh assuming that its MeshObject is unoptimized.
            // \param vertVec [in] The vector of vertices from the parent MeshObject.
            // \return false if V-ART was not compiled with OpenGL support.
            //~ bool DrawOglUnoptimized(const std::vector<Point4D>& vertVec) const;

            /// Returns the mesh type as OpenGL enum.
            #ifdef VART_OGL
            GLenum GetOglType() const { return GetOglType(type); }
            #endif

            void IncrementIndices(unsigned int increment);

        // PUBLIC ATTRIBUTES
            /// indexes of the vertices (start at 0) defining faces
            std::vector<unsigned int> indexVec;
            /// indexes of the normals (for unoptimized meshes)
            std::vector<unsigned int> normIndVec;
            Material material;
            MeshType type;
        private:
            #ifdef VART_OGL
            /// \brief Converts from V-ART MeshType to OpenGL GLenum (for mesh types).
            static GLenum GetOglType(MeshType type);
            #endif
    }; // end class declaration
} // end namespace
#endif
