/// \file box.cpp
/// \brief Implementation file for V-ART class "Box".
/// \version $Revision: 1.0 $

#ifdef WIN32
#include <windows.h>
#endif
#ifdef VART_OGL
#include <GL/glu.h>
#endif
#include "vart/box.h"
#include <iostream>

#include "vart/mesh.h"
#include "vart/meshobject.h"
#include "vart/file.h"
#include <sstream>
#include <cassert>
#include <fstream>
#include <cstdlib>


using namespace std;

VART::Box::Box() : VART::MeshObject() {
}

void VART::Box::MakeBox(double minX,
                        double maxX,
                        double minY,
                        double maxY, double minZ, double maxZ, bool oneMesh) {
    assert((minX <= maxX) && (minY <= maxY) && (minZ <= maxZ));
    // each vertex must repeat 3 times because there must be a vertex/normal correspondence
    // of 1:1
    double coordinateArray[] = { minX,minY,minZ, //0
                                 minX,maxY,minZ, //1
                                 maxX,maxY,minZ, //2
                                 maxX,minY,minZ, //3
                                 minX,minY,maxZ, //4
                                 minX,maxY,maxZ, //5
                                 maxX,maxY,maxZ, //6
                                 maxX,minY,maxZ, //7
                                 minX,minY,minZ, //8
                                 minX,maxY,minZ, //9
                                 maxX,maxY,minZ, //10
                                 maxX,minY,minZ, //11
                                 minX,minY,maxZ, //12
                                 minX,maxY,maxZ, //13
                                 maxX,maxY,maxZ, //14
                                 maxX,minY,maxZ, //15
                                 minX,minY,minZ, //16
                                 minX,maxY,minZ, //17
                                 maxX,maxY,minZ, //18
                                 maxX,minY,minZ, //19
                                 minX,minY,maxZ, //20
                                 minX,maxY,maxZ, //21
                                 maxX,maxY,maxZ, //22
                                 maxX,minY,maxZ };  //23
    double* endOfCoordinateArray = coordinateArray + sizeof(coordinateArray)/sizeof(double);
    unsigned int indexArray[] = { 0,1,2,3, // back face
                                  4,7,6,5, // front face
                                  11,10,14,15, // right face
                                  13,9,8,12, // left face
                                  22,18,17,21, // top face
                                  23,20,16,19 }; // bottom face
    unsigned int* endOfIndexArray = indexArray + sizeof(indexArray)/sizeof(int);
    double normalArray[] = { 0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1,
                             0,0,1, 0,0,1, 0,0,1, 0,0,1,
                             -1,0,0, -1,0,0, 1,0,0, 1,0,0,
                             -1,0,0, -1,0,0, 1,0,0, 1,0,0,
                             0,-1,0, 0,1,0, 0,1,0, 0,-1,0,
                             0,-1,0, 0,1,0, 0,1,0, 0,-1,0 };
    double* endOfNormalArray = normalArray + sizeof(normalArray)/sizeof(double);
    float textArray[] = { 1,0,0, 1,1,0, 0,1,0, 0,0,0,
                           0,0,0, 0,1,0, 1,1,0, 1,0,0,
                           0,0,0, 0,1,0, 1,1,0, 1,0,0,
                           1,0,0, 1,1,0, 0,1,0, 0,0,0,
                           0,0,0, 0,1,0, 1,1,0, 1,0,0,
                           0,1,0, 0,0,0, 1,0,0, 1,1,0 };
    float* endOfTextArray = textArray + sizeof(textArray)/sizeof(float);

    vertCoordVec.clear();
    normCoordVec.clear();
    textCoordVec.clear();
    meshList.clear();
    vertCoordVec.assign(coordinateArray,endOfCoordinateArray);
    normCoordVec.assign(normalArray,endOfNormalArray);
    textCoordVec.assign(textArray,endOfTextArray);
    if (oneMesh) { // One mesh cube
        VART::Mesh mesh;
        mesh.type = VART::Mesh::QUADS;
        mesh.indexVec.assign(indexArray,endOfIndexArray);
        mesh.material = VART::Material::DARK_PLASTIC_GRAY(); // default material
        meshList.push_back(mesh);
    }
    else { // six meshes cube
        unsigned int* index = indexArray;
        for(int i = 0 ; i < 6 ; ++i, index += 4){
            VART::Mesh mesh;
            mesh.type = VART::Mesh::QUADS;
            mesh.indexVec.assign(index, index + 4);
            mesh.material = VART::Material::DARK_PLASTIC_GRAY(); // default material
            meshList.push_back(mesh);
        }
    }

    ComputeBoundingBox();
    ComputeRecursiveBoundingBox();
}

void VART::Box::SetMaterialBoxFace(const VART::Material& mat, int numberFace){
    // Set a material for an specific face of the box or for all faces.
    // numberFace = 0 -> back face
    // numberFace = 1 -> front face
    // numberFace = 2 -> right face
    // numberFace = 3 -> left face
    // numberFace = 4 -> top face
    // numberFace = 5 -> bottom face
    // numberFace = 6 -> all faces

    if ((numberFace >= 0) && (numberFace <= 5)) {
        list<VART::Mesh>::iterator iter = meshList.begin();
        for (int i = 0 ; i < numberFace; ++iter, ++i);

        iter->material = mat;
    }
    else if (numberFace == 6)
        this->SetMaterial(mat);
}

