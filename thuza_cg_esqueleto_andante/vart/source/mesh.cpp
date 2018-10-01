/// \file mesh.cpp
/// \brief Implementation file for V-ART class "Mesh".
/// \version $Revision: 1.1 $

#include "vart/mesh.h"

using namespace std;

VART::Mesh::Mesh()
    : type(NONE) {}

VART::Mesh::Mesh(const VART::Mesh& mesh) {
    indexVec = mesh.indexVec;
    normIndVec = mesh.normIndVec;
    type = mesh.type;
    material = mesh.material;
}

VART::Mesh& VART::Mesh::operator=(const VART::Mesh& mesh) {
    indexVec = mesh.indexVec;
    normIndVec = mesh.normIndVec;
    type = mesh.type;
    material = mesh.material;
    return *this;
}

void VART::Mesh::IncrementIndices(unsigned int increment) {
    for (unsigned int i=0; i < indexVec.size(); ++i)
        indexVec[i] += increment;
}

#ifdef VART_OGL
GLenum VART::Mesh::GetOglType(MeshType type) {
    switch (type) {
        case POINTS:
            return GL_POINTS;
        case LINES:
            return GL_LINES;
        case LINE_STRIP:
            return GL_LINE_STRIP;
        case LINE_LOOP:
            return GL_LINE_LOOP;
        case TRIANGLES:
            return GL_TRIANGLES;
        case TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case QUADS:
            return GL_QUADS;
        case QUAD_STRIP:
            return GL_QUAD_STRIP;
        default:
            return GL_POLYGON;
    }
}
#endif


bool VART::Mesh::DrawInstanceOGL() const {
#ifdef VART_OGL
    bool result = material.DrawOGL();
    if (material.HasTexture())
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    else
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawElements(GetOglType(type), indexVec.size(), GL_UNSIGNED_INT, &indexVec[0]);
    return result;
#else
    return false;
#endif
}

//~ bool VART::Mesh::DrawOglUnoptimized(const vector<VART::Point4D>& vertVec) const {
//~ #ifdef VART_OGL
    //~ bool result = material.DrawOGL();
    //~ glBegin(GetOglType(type));
    //~ for (unsigned int i=0; i < indexVec.size(); ++i)
        //~ //glNormal3dv(normVec
        //~ glVertex4dv(vertVec[indexVec[i]].VetXYZW());
    //~ glEnd();
    //~ return result;
//~ #else
    //~ return false;
//~ #endif
//~ }

namespace VART {
    ostream& operator<<(ostream& output, const Mesh::MeshType mt) {
        switch (mt)
        {
            case VART::Mesh::POINTS:
                output << "POINTS";
                break;
            case VART::Mesh::LINES:
                output << "LINES";
                break;
            case VART::Mesh::LINE_STRIP:
                output << "LINE_STRIP";
                break;
            case VART::Mesh::LINE_LOOP:
                output << "LINE_LOOP";
                break;
            case VART::Mesh::TRIANGLES:
                output << "TRIANGLES";
                break;
            case VART::Mesh::TRIANGLE_STRIP:
                output << "TRIANGLE_STRIP";
                break;
            case VART::Mesh::TRIANGLE_FAN:
                output << "TRIANGLE_FAN";
                break;
            case VART::Mesh::QUADS:
                output << "QUADS";
                break;
            case VART::Mesh::QUAD_STRIP:
                output << "QUAD_STRIP";
                break;
            case VART::Mesh::POLYGON:
                output << "POLYGON";
                break;
            default:
                output << "NO MESH TYPE";
        }
        return output;
    }

    ostream& operator<<(ostream& output, const Mesh& mesh) {
        output << mesh.type << " indexVec: ";
        for (auto& index: mesh.indexVec)
            output << index << " ";
        //output << "material: " << mesh.material;
        return output;
    }
}
