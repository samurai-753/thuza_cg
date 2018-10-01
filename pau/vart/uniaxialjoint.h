/// \file uniaxialjoint.h
/// \brief Header file for V-ART class "UniaxialJoint".
/// \version $Revision: 1.4 $

#ifndef VART_UNIAXIALJOINT_H
#define VART_UNIAXIALJOINT_H

#include "vart/dof.h"
#include "vart/joint.h"

namespace VART {
/// \class UniaxialJoint uniaxialjoint.h
/// \brief A joint with one rotational degrees of freedom.
///
/// FixMe: Put detailed description here.
    class UniaxialJoint : public Joint {
        public:
            /// \brief Creates an uninitialized biaxial (1 DOF) joint.
            ///
            /// Before using the joint, one DOF should be added to it.
            UniaxialJoint();

            /// \brief Creates a copy of an UniaxialJoint.
            UniaxialJoint(const VART::UniaxialJoint& joint);

            /// \brief Copies all data of an UniaxialJoint to another.
            VART::UniaxialJoint& operator=(const VART::UniaxialJoint& joint);

            /// \brief Returns a copy of an Joint. Every derived class must reimplements 
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();

            //~ BiaxialJoint( Joint *parent );
            //~ BiaxialJoint( Joint *parent, Dof *dof1, Dof *dof2 );

            virtual ~UniaxialJoint();

            /// \brief Sets the first DOF.
            /// \param value [in] A number in the range [0:1] - zero means "set to minimal angle"
            /// and 1 means "set to maximal angle".
            void SetFlexionTo( float value );

            /// \brief Moves the first DOF.
            /// \param variance [in] A number in the range [0:1] - will be added to the first DOF's
            /// position (see Dof::GetCurrent).
            void Flex(float variance) { dofList.front()->Move(variance); }

            /// \brief Add a DOF to the joint.
            /// \param dof [in] A pointer to the DOF that is to be added.
            ///
            /// Only one DOF can be added to a uniaxial joint. Joint's destructor will
            /// dealocate DOF marked as autoDelete.
            virtual void AddDof(Dof* dof);
            virtual Dof* AddDof(const Point4D& vec, const Point4D& pos, float min, float max);
    }; // end class declaration
    // FixMe: This class is not implementated yet.
    class HingeJoint : public UniaxialJoint {
    };

    // FixMe: This class is not implementated yet.
    class PivotJoint : public UniaxialJoint {
    };
} // end namespace
#endif
