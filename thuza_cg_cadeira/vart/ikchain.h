/// \file ikchain.h
/// \brief Header file for V-ART class "IKChain".
/// \version $Revision$

#ifndef VART_IKCHAIN_H
#define VART_IKCHAIN_H

#include "vart/sgpath.h"
#include "vart/point4d.h"
#include "vart/dof.h"

namespace VART {
/// \class IKChain ikchain.h
/// \brief Inverse Kinematic Chain
///
/// Describes an inverse kinematics chain to be used on some IK solver. An IK chain is a sequence
/// of DOFs and an end effector (position + orientation).
    class IKChain
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            /// \brief Main constructor
            /// \param path  [in] A SGPath that contains all joints in chain.
            /// \param eePos [in] End Effector Position.
            /// \param eeOri [in] End Effector Orientation.
            IKChain(SGPath path, Point4D eePos, Point4D eeOri);

            /// \brief Sets the position of End Effector
            void SetEEPosition(const Point4D& eePos);

            /// \brief Sets the target position
            void SetTargetPosition(const Point4D& target) { targetPos = target; }

            /// \brief Adjusts the chain towards solution
            void MoveTowardsSolution();
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            /// \brief Chain (list) of DOFs.
            std::list<Dof*> dofChain;
            /// \brief Position of end effector
            Point4D eePosition;
            /// \brief Orientation of end effector
            ///
            /// The "real" orientation is defined by three vectors: one vector from the last dof in
            /// the chain and the EE position, one given (eeOrientation) and the cross product of
            /// the previous two.
            Point4D eeOrientation;
            /// \brief Target position
            Point4D targetPos;
    }; // end class declaration
} // end namespace

#endif
