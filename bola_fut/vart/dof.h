/// \file dof.h
/// \brief Header file for V-ART class "Dof".
/// \version $Revision: 1.6 $

#ifndef VART_DOF_H
#define VART_DOF_H

#include "vart/point4d.h"
#include "vart/transform.h"
#include "vart/bezier.h"
#include "vart/memoryobj.h"
#include <string>
#include <list>
#include <iostream> // for XmlPrintOn

namespace VART {
    class Modifier;
    class Joint;
/// \class Dof dof.h
/// \brief Degree Of Freedom - basic component of a Joint.
///
/// A DOF is an axis of rotation, that may move along a 3D curve (the evoluta). The rotation
/// of a DOF has a limited range that may change according to external elements, this is
/// controlled by the rangeModifier (Modifier). There may exist a hierachy of DOFs, so
/// that by changing one DOF, hierarchically lower DOFs are rotated. DOFs may not be shared
/// among joints because they have a single pointer to the owner joint and because the
/// joint destructor may destroy DOFs marked as autoDelete.
    class Dof : public MemoryObj {
        public:
        // PUBLIC METHODS
            Dof();
            Dof(const Dof& dof);
            /// \brief Creates and initializes a DOF.
            /// \param vec [in] A vector. Together with "pos", defines the rotation axis.
            /// \param pos [in] A point. Together with "vec", defines the roatation axis.
            /// \param min [in] An angle (radians). Minimal allowed rotation.
            /// \param max [in] An angle (radians). Maximal allowed rotation.
            /// The DOF is created so that "current position" refers to zero rotation. If roatation
            /// range does not allow zero rotation, then the programmer should manually fix this
            /// using MoveTo.
            Dof(const Point4D& vec, const Point4D& pos, float min, float max);
            ~Dof();
            Dof& operator=(const Dof& dof);
            void SetDescription(const std::string& desc);
            const std::string& GetDescription() const { return description; }
            Point4D GetAxis() const;
            Point4D GetOrigin() const;
            Point4D GetPosition() const { return position; }
            Bezier* GetEvoluta() { return evoluta; }
            void GetLim(Transform* resultPtr);
            const Transform& GetLim() const;
            float GetMin() const;
            float GetMax() const;
            float GetCurrentMin() const;
            float GetCurrentMax() const;
            /// \brief Returns DOF's rest position.
            float GetRest() const;
            Joint* GetOwnerJoint() const;
            void SetEvoluta( Bezier *evol );
            void SetLim(const Transform& t);
            void SetAxis(Point4D vec);
            /// \brief Sets DOF's minimal state.
            /// \param min [in] An angle in radians (for a rotational dof).
            ///
            /// DOFs translate to simple transforms (usually rotations). They have a minimal and
            /// a maximal state. In case of a rotational DOF, the minimal state refers to the
            /// minimal angle (in radians) that the DOF may represent as a rotation.
            void SetMin(float min);
            /// \brief Sets DOF's maximal state.
            /// \param max [in] An angle in radians (for a rotational dof).
            ///
            /// DOFs translate to simple transforms (usually rotations). They have a minimal and
            /// a maximal state. In case of a rotational DOF, the maximal state refers to the
            /// maximal angle (in radians) that the DOF may represent as a rotation.
            void SetMax(float max);
            void SetOwnerJoint( Joint *ow );
            /// \brief Sets DOF's current position.
            /// \param pos [in] A number in the range [0:1]
            ///
            /// A value of zero means minimal rotation (see GetMin, SetMin) around the initial
            /// axis position (see GetAxis, GetOrigin, GetEvoluta and related "Set" methods).
            /// A value of one means maximal rotation. Other values are linear.
            void MoveTo(float pos);
            /// \brief Sets DOF's current position.
            /// \param pos [in] A number in the range [0:1]
            /// \param newPriority [in] Priority for DOF update (greater means greater priority)
            ///
            /// Moves the DOF to given position if current update has priority over last update.
            void MoveTo(float pos, unsigned int newPriority);

            /// \brief Gets DOF's current position.
            float GetCurrent() const;

            /// \brief Changes DOF
            ///
            /// Changes how much the DOF is "bent"
            /// \param variance [in] How much to change the "current position".
            /// \sa MoveTo()
            void Move(float variance) { MoveTo(currentPosition+variance); }

            void SetRest( float rest );

            /// \brief Initializes a DOF previouly created with default constructor
            ///
            /// Newly created DOFs are set at zero rotation. Use GetCurrent to find
            /// initial positial.
            /// \param vec [in] Rotation vector;
            /// \param pos [in] DOF position (defines an axis along with "vec");
            /// \param min [in] Minimal bending angle in radians;
            /// \param max [in] Maximal bending angle in radians;
            void Set(const Point4D& vec, const Point4D& pos, float min, float max);

            void Rest();
            void SetRangeModifier( Modifier *m );
            Modifier* GetRangeModifier();

            /// \brief Apply internal transform to some external transform.
            ///
            /// Pre-multiply the internal transform to the argument. As Transform
            /// is a pre-multiplication matrix, the resulting transform has the effect
            /// of its previous value followed by the DOF's internal transform. This
            /// method is called by Joint when it is collection DOFs' transforms in
            /// order to compose its final transform.
            /// \param ptrTrans [in,out] where to apply internal transform.
            void ApplyTransformTo(Transform* ptrTrans) const;

            /// \brief Outputs XML representation of the scene.
            void XmlPrintOn(std::ostream& os, unsigned int indent) const;

            /// \brief Reconfigure DOF to match a target direction.
            ///
            /// Given a state (some direction representing current DOF configuration), puts
            /// itself in a configuration that would make state as close as possible to target.
            void Reconfigure(const Point4D& state, const Point4D& target);
    #ifdef VISUAL_JOINTS
            bool DrawInstanceOGL() const;
        // PUBLIC STATIC ATTRIBUTES
            static float axisSize;
    #endif
        // PUBLIC STATIC METHODS
            /// \brief Resets priorities of all DOF instances
            ///
            /// For every instance of Dof, sets its priority to zero. Should be called at
            /// every render cycle, in a z-buffer-like scheme.
            static void ClearPriorities();
        // PUBLIC ATTRIBUTES

        protected:
        // PROTECTED METHODS
            void ComputeLIM();
        // PROTECTED ATTRIBUTES
            /// Together with "axis", defines the rotation axis. Relative to the parent reference system.
            Point4D position;
            /// Together with "position", defines the rotation axis. Relative to the parent reference system.
            Point4D axis;
            /// \brief Priority of last DOF change
            ///
            /// When several elements try to update a DOF, the priority attribute controls
            /// which of them will really affect the DOF. Lower numbers mean lower priority.
            unsigned int priority;
        private:
        // PRIVATE ATTRIBUTES
            std::string description;// Name of the Dof; often related to the dof's type of motion
            Bezier* evoluta; // 3D path related to the axis position along its rotation
            Transform lim; // Local Instance Matrix
            float minAngle;           // Min base angle in rad.
            float maxAngle;           // Max base angle in rad.
            float currentMinAngle;            // Min angle in rad currently valid.
            float currentMaxAngle;            // Max angle in rad currently valid.
            float confortMinAngle;            // Min angle in rad for comfortable position.
            float confortMaxAngle;            // Max angle in rad for comfortable position.
            Modifier* rangeModifier; // Entity used to change the motion range along motion.
            float currentPosition; //A real number from 0 to 1
            float restPosition;           //Another real number from 0 to 1
            Joint* ownerJoint;            //Reference to the joint where this dof is set up
        // PRIVATE STATIC ATTRIBUTES
            // List of all instances of the class
            static std::list<Dof*> instanceList;
    }; // end class declaration
} // end namespace
#endif
