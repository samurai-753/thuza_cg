/// \file graphicobj.h
/// \brief Header file for V-ART class "GraphicObj".
/// \version $Revision: 1.8 $

#ifndef VART_GRAPHICOBJ_H
#define VART_GRAPHICOBJ_H

#include "vart/scenenode.h"
#include "vart/boundingbox.h"

namespace VART {
/// \class GraphicObj graphicobj.h
/// \brief An scene node that is associated with a shape
///
/// Abstract class for graphical objects, that is, objects that have shapes to be
/// drawn. A graphical object may, or may not, be visible without affecting scene
/// graph traversal.
    class GraphicObj : public SceneNode {
        public:
        // PUBLIC TYPES
            enum ShowType { LINES, LINES_AND_NORMALS, POINTS, POINTS_AND_NORMALS, FILLED };

        // PUBLIC METHODS
            GraphicObj();

            /// Makes the object visible.
            void Show();
            /// Makes the object invisible.
            void Hide();
            /// Toggles the object's visibility.
            void ToggleVisibility();

            /// Toggles the recursive object's visibility.
            void ToggleRecVisibility();

            /// Checks whether the object is visible.
            bool IsVisible();

            /// Computes the bounding box.
            virtual void ComputeBoundingBox() = 0;

            /// \brief Computes the recursive bounding box.
            ///
            /// This method requires a correct bounding box, therefore it should usually
            /// be called after ComputeBoundingBox.
            void ComputeRecursiveBoundingBox();

            /// Returns the bounding box.
            const BoundingBox& GetBoundingBox() const { return bBox; }

            /// \brief Sets the bounding box visibility.
            void SetBBoxVisibility(bool value)    { bBox.visible = value; };

            /// \brief Sets the bounding box color of graphics object.
            void SetBBoxColor(VART::Color value) { bBox.SetColor(value); }

            /// Returns the recursive bounding box.
            const BoundingBox& GetRecursiveBoundingBox() const { return recBBox; }

            /// Toggles the bounding box visibility.
            void ToggleBBoxVisibility();

            /// Toggles the recursive bounding box visibility.
            void ToggleRecBBoxVisibility();

            /// \brief Returns the pick name
            ///
            /// The pick name is used when picking objects with the mouse. Selection
            /// methods return the pick name which can be searched for in the scene graph.
            unsigned int PickName() const { return pickName; }

            /// \brief Draws and object, setting pick info
            ///
            /// This method should be called in selection mode in order to identify objects
            /// that are selected by the mouse (see Scene::Pick).
            virtual void DrawForPicking() const;

        // PUBLIC ATTRIBUTES
            /// \brief Defines how to show the object
            ShowType howToShow;

        protected:
            bool show;
            BoundingBox bBox;
            BoundingBox recBBox; // recursive bounding box

            /// A number that uniquely identifies the object for picking purposes.
            unsigned int pickName;

    }; // end class declaration
} // end namespace
#endif
