/// \file transformaction.h
/// \brief Header file for V-ART class "TransformAction".
/// \version $Revision: 1.1 $

#ifndef VART_TRANSFORMACTION_H
#define VART_TRANSFORMACTION_H

#include "vart/baseaction.h"

namespace VART {
    class Transform;
    class TransformAction : public BaseAction {
        public:
            TransformAction();
            virtual ~TransformAction() { }

            /// \brief Sets the Transform on which to act
            void SetTarget(Transform* target) { targetPtr = target; }
        protected:
            virtual void Move() = 0;
            Transform* targetPtr;
        private:
    }; // end class declaration
} // end namespace

#endif
