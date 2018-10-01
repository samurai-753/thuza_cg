/// \file biaxialjoint.h
/// \brief Header file for V-ART class "BiaxialJoint".
/// \version $Revision: 1.3 $

#ifndef VART_BIAXIALJOINT_H
#define VART_BIAXIALJOINT_H

#include "vart/dof.h"
#include "vart/joint.h"

namespace VART {
/// \class BiaxialJoint biaxialjoint.h
/// \brief A joint with two rotational degrees of freedom.
///
/// FixMe: Put detailed description here.
    class BiaxialJoint : public Joint {
        public:
            /// \brief Creates an uninitialized biaxial (2 DOFs) joint.
            ///
            /// Before using the joint, two DOFs should be added to it.
            BiaxialJoint();
            BiaxialJoint(const VART::BiaxialJoint& joint);
            VART::BiaxialJoint& operator=(const VART::BiaxialJoint& joint);

            /// \brief Returns a copy of an biaxialJoint. Every derived class must 
            /// reimplements this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();

            //~ BiaxialJoint( Joint *parent );
            //~ BiaxialJoint( Joint *parent, Dof *dof1, Dof *dof2 );

            virtual ~BiaxialJoint();

            /// \brief Sets the first DOF.
            /// \param value [in] A number in the range [0:1] - zero means "set to minimal angle"
            /// and 1 means "set to maximal angle".
            void SetFlexionTo( float value );

            /// \brief Moves the first DOF.
            /// \param variance [in] A number in the range [0:1] - will be added to the first DOF's
            /// position (see Dof::GetCurrent).
            void Flex(float variance) { dofList.front()->Move(variance); }

            /// \brief Sets the second DOF.
            /// \deprecated Spelling has been corrected to SetAdductionTo.
            void SetAductionTo( float value );

            /// \brief Sets the second DOF.
            /// \param value [in] A number in the range [0:1] - zero means "set to minimal angle"
            /// and 1 means "set to maximal angle".
            void SetAdductionTo( float value );

            /// \brief Moves the second DOF.
            /// \param variance [in] A number in the range [0:1] - will be added to the second
            /// DOF's position (see Dof::GetCurrent).
            void Adduct(float variance);

            /// \brief Add a DOF to the joint.
            /// \param dof [in] A pointer to the DOF that is to be added.
            ///
            /// At most two DOFs can be added to a biaxial joint. Joint's destructor will
            /// dealocate DOFs marked as autoDelete.
            void AddDof(Dof* dof);
        private:
    }; // end class declaration

    /// FixMe: This class is not implementated yet.
    class CondylarJoint : public BiaxialJoint {
    };

    /// FixMe: This class is not implementated yet.
    class SaddlelJoint : public BiaxialJoint {
    };

    /// FixMe: This class is not implementated yet.
    class EllipsoidJoint : public BiaxialJoint {
    };
} // end namespace
#endif
