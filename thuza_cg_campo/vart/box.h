/// \file box.h
/// \brief Header file for V-ART class "Box".
/// \version $Revision: 1.0 $

#ifndef VART_BOX_H
#define VART_BOX_H

#include "vart/meshobject.h"
#include "vart/material.h"

#include "vart/mesh.h"
#include "vart/point4d.h"
#include "vart/graphicobj.h"
#include "vart/transform.h"
#include <vector>
#include <list>
#include <map>

namespace VART {
/// \class Box box.h
/// \brief A box.
    class Box : public MeshObject {
        public:
            // FACE ENUM
            enum faceEnum { BACK_FACE=0, FRONT_FACE=1, RIGHT_FACE=2, LEFT_FACE=3,
                            TOP_FACE=4, BOTTOM_FACE=5, ALL_FACES=6};

            // PUBLIC METHODS
            /// \brief Creates an uninitialized box
            Box();
            /// \brief Creates a box aligned with the 3 reference planes (XY, XZ and YZ).
            void MakeBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ, bool oneMesh=true);

            /// \brief Assigns a material to the mesh of one (or all) face of a box object.
            void SetMaterialBoxFace(const Material& mat, int numberFace=6);
    }; // end class declaration
} // end namespace
#endif
