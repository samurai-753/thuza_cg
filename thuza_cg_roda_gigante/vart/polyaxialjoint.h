/// \file polyaxialjoint.h
/// \brief Header file for V-ART class "PolyaxialJoint".
///  \version $Revision: 1.3 $

#ifndef VART_POLYAXIALJOINT_H
#define VART_POLYAXIALJOINT_H

#include "vart/joint.h"
#include "vart/dof.h"

namespace VART {
/// \class PolyaxialJoint polyaxialjoint.h
/// \brief A joint with three rotational degrees of freedom.
///
/// FixMe: Put detailed description here.
    class PolyaxialJoint : public Joint {
        public:
            PolyaxialJoint();
            
            /// \brief Creates a new copy of an PolyaxialJoint.
            PolyaxialJoint(const VART::PolyaxialJoint& joint);
            
            /// \brief Copy the polyaxialJoint data to another PolyaxialJoint.
            VART::PolyaxialJoint& operator=(const VART::PolyaxialJoint& joint);
            
            /// \brief Returns a copy of an PolyaxialJoint. Every derived class must reimplements 
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();
            
            virtual ~PolyaxialJoint() {}
            void SetFlexionTo(float value);
            void SetAductionTo(float value);
            void SetTwistTo(float value);
            /// \brief Moves the first DOF.
            void Flex(float variance) { dofList.front()->Move(variance); }
            void Adduct(float variance);
            void Twist(float variance);
            void AddDof(Dof* dof);
    }; // end class declaration
} // end namespace
#endif
