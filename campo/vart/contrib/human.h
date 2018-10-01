/// \file human.h
/// \brief Header file for V-ART class "Human" (contrib).
/// \version $Revision: 1.6 $

#ifndef VART_HUMAN_H
#define VART_HUMAN_H

#include "vart/graphicobj.h"
#include "vart/joint.h"
#include "vart/point4d.h"
#include "vart/sgpath.h"
#include "vart/callback.h"
#include <string>
#include <list>
#include <map>

namespace VART {
    class Scene;
    class NoisyDofMover;
    class Action;
    class JointAction;
    class XmlJointAction;
    class DMModifier;
/// \class Human human.h
/// \brief Articulated object for humanoid representation
///
/// A human object is an articulated object that can be included in the scene graph.
    class Human : public GraphicObj
    {
        friend class StepManager;
        public:
            class StepManager : public VART::CallBack
            {
                public:
                    StepManager(Human* human);
                    virtual void Activate();
                private:
                    Human* humanPtr;
                    bool steppingRight;
            };
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            /// \brief Creates an uninitialized humanoid
            Human();
            Human(VART::Human& human);
            ~Human();

            virtual SceneNode* Copy();
            virtual void ComputeBoundingBox();
            virtual void TraverseDepthFirst(SNOperator* operatorPtr);
            virtual void LocateDepthFirst(SNLocator* locatorPtr) const;
            virtual void DrawForPicking() const;
            /// \brief Loads from a XML file
            ///
            /// Loads an articulated humanoid from an xml file. The newly createad human will have
            /// two active actions: rest and breathe.
            bool LoadFromFile(const std::string& fileName);

            /// \brief Adds the humanoid to a scene
            void PutIntoScene(Scene* scenePtr);

            //~ /// \brief Prints all noisy dof movers.
            //~ void PrintMovers();

            /// \brief Loads action from given file name
            XmlJointAction* LoadAction(const std::string& fileName);

            /// \brief Modifies registered actions
            void ModifyActions(DMModifier& modifier);

            void ComputeStickPosition(Point4D* resultPtr);

            void AdjustPosition();

            void ComputeTransform(Transform* transPtr);

            void Move(const Point4D& offset);

            void ResetPosition();

            void RotateOnY(float radians);

            void Walk(bool status);

            bool WalkTo(const Point4D& dest, const Point4D& orientation);

            void SwapLegs();

            void StickRightFoot();

            void StickLeftFoot();

            /// \brief Returns true if the human has reached its destination
            bool ReachedDestiantion() const;

            Point4D Position() const;

            bool IsWalking() const { return isWalking; }

            void ChangeAdjustment(const Point4D& displacement);

            void ActivateBreatheAction(bool status);

            void ActivateRestAction(bool status);

            //~ void XmlPrintOn(std::ostream& os, unsigned int indent) const { Transform::XmlPrintOn(os,indent); }
        protected:
            /// \brief Computes and returns the position of the right foot front
            void ComputeLocalStickPosition(Point4D* resultPtr);
            //~ typedef std::map< Joint::DofID, std::list<NoisyDofMover*> > DofID2Movers;
            //~ typedef std::map<Joint::DofID, float> DofID2Offset;
            //~ typedef std::map< std::string, DofID2Movers > JointName2IDs;
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
            virtual bool DrawInstanceOGL() const;
            /// \brief Builds its internal map of dof movers
            //~ void MapMoversFrom(Action& action);

            /// \brief Computes signed angle to given position
            /// \param [in]  pos        position
            /// \param [out] totalAngle total angle to position
            ///
            /// Returns a signed parcial angle to given position, as computed from human position
            /// (center -- not foot support position). If you do not want the total angle, pass
            /// NULL.
            float AngleToPosition(const Point4D& pos, float* totalAngle) const;
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            /// \brief Maps joint names to its noisydofmovers
            //~ JointName2IDs dofs;
            std::list<JointAction*> actionList;
            Point4D rfFront; // right foot front
            Point4D lfFront;
            Point4D rfBack;
            Point4D lfBack;
            Point4D* stickPositionPtr;
            /// \brief A transform that puts the human at his position and orientation.
            Transform position;
            /// \brief A transform that makes a point on human's geometry to stick at the ground.
            Transform adjustment;
            /// \brief A transform that puts the human vertical on Y, scales him to the right size
            ///        and puts his feet at Y = 0;
            Transform orientation;
            Joint* hipJointPtr; // root node of a human
            SGPath pathToRFoot; // from hip joint to right foot
            SGPath pathToLFoot; // from hip joint to left foot
            SGPath* pathToStickPosPtr;
            JointAction* moveLeftLegPtr;
            JointAction* moveRightLegPtr;
            JointAction* restPtr;
            JointAction* breathePtr;
            StepManager stepManager;
            /// A vector that points forward on the floor plane (world coordinates)
            Point4D forward;
            /// Maximal rotation (radians) during a single step
            float maxStepRotation;
            Point4D destination;
            /// currently walking?
            bool isWalking;
            // Affective State
            float anxiety;
            float anger;
            float joy;
            float fear;
            float disgust;
            float surprise;
        private:
    }; // end class declaration
} // end namespace

#endif
