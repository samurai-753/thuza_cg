/// \file joint.h
/// \brief Header file for V-ART class "Joint".
/// \version $Revision: 1.7 $


#ifndef VART_JOINT_H
#define VART_JOINT_H

#ifdef VISUAL_JOINTS
#include "vart/material.h"
#endif

#include "vart/transform.h"
#include <list>
#include <iostream> // for XmlPrintOn

namespace VART {
    class Dof;
/// \class Joint joint.h
/// \brief Representation of joints
///
/// A joint can be seen as a special kind of geometric transformation. It deals
/// with the complexities of real articulations, producing geometric transformations
/// that can be delivered to the graphics pipeline. A joint is a collecion of DOFs
/// (see Dof) in which their order is important - the transformation of a joint is
/// (... DOF3 * DOF2 * DOF1), that is, DOF1's transform, followed by DOF2's transform...
/// In the current implementation, when DOFs change, their hierarchically inferior
/// DOFs are transformed,
/// that is, on a 3-DOF joint, changing the 2nd DOF tranforms the 3rd DOF. Their order
/// is defined by AddDof method. On future implementations this should change to allow
/// creation of more sophisticated joints.
/// Joints may not share DOFs, see Dof for an explanation.
/// Compile with symbol VISUAL_JOINTS if you want to see DOFs for debugging purposes.
    class Joint : public Transform {
        public:
            enum DofID { FLEXION, ADDUCTION, TWIST };
            /// Creates an uninitialized joint.
            Joint();
            Joint(const Joint& j);
            virtual ~Joint();

            /// \brief Returns a copy of a Joint. Every derived class must reimplement
            /// this method, to avoid errors with VART::SceneNode::RecursiveCopy.
            virtual VART::SceneNode * Copy();

            /// \brief Copy the dofList atribute from an joint, setting the ownerJoint
            /// atribute of copied dof to 'this' joint instance.
            virtual void CopyDofListFrom( VART::Joint& joint );

            const Joint& operator=(const Joint& j);
            unsigned short int GetNumDofs() const { return dofList.size(); }

            /// \brief Adds a Dof.
            /// \param vec [in] A vector.
            ///        Together with "pos", defines the rotation axis of the new Dof.
            /// \param pos [in] A point.
            ///        Together with "vec", defines the roatation axis of the new Dof.
            /// \param min [in] An angle (radians). Minimal allowed rotation.
            /// \param max [in] An angle (radians). Maximal allowed rotation.
            /// \return a pointer to the newly created Dof.
            ///
            /// Creates a new Dof with given parameters. Attaches the Dof to itself.
            /// The memory will be released upon Joint destruction.
            virtual Dof* AddDof(const Point4D& vec, const Point4D& pos, float min, float max);

            /// \brief Adds a Dof.
            virtual void AddDof(Dof *dof);

            /// \brief Updates the LIM, based on DOFs' situation.
            ///
            /// Not of interest to the application programmer. This method is called by
            /// DOFs when they are updated in order to keep the joint updated as well.
            void MakeLim();

            /// \brief Returns a joint's DOF.
            ///
            /// Returns a read-only version of a DOF from the joint.
            /// Requires the existence of given Dof. See DofID constants.
            const Dof& GetDof(DofID dof) const;

            /// \brief Returns all DOFs.
            ///
            /// Adds pointers to all DOFs to the given list.
            void GetDofs(std::list<Dof*>* dofListPtr);

            /// \brief Returns the DofID of some member Dof.
            DofID GetDofID(const Dof* dofPtr) const;

            /// Put the joint in a rest position
            void SetAtRest();

            /// \brief Moves a joint's DOF.
            /// \return True if the specified DOF exists.
            /// \param dof [in] Specifies which DOF should be modified.
            /// \param variance [in] Specifies how much the DOF should move (using [0:1] position range).
            bool MoveDof(DofID dof, float variance);

            /// Checks whether a DOF exists in the joint.
            bool HasDof(DofID dof);

            /// Returns type identification of the node.
            virtual TypeID GetID() { return JOINT; }

            /// \brief Outputs XML representation of the scene.
            virtual void XmlPrintOn(std::ostream& os, unsigned int indent) const;

    #ifdef VISUAL_JOINTS
            virtual bool DrawOGL() const;
    #endif

        protected:
        // PROTECTED METHODS
    #ifdef VISUAL_JOINTS
            /// \brief Returns materials for the visual representation of DOFs.
            static const Material& GetMaterial(int num);
    #endif
        // PROTECTED ATTRIBUTES
            std::list<Dof*> dofList;
    }; // end class declaration
    std::ostream& operator<<(std::ostream& output, const Joint::DofID& dofId);
    std::istream& operator>>(std::istream& input,  Joint::DofID& dofId);
} // end namespace
#endif
