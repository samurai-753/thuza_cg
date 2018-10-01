#ifndef VART_REFSYSTEM_H
#define VART_REFSYSTEM_H

#include "graphicobj.h"
#include "scenenode.h"

namespace VART {
	class RefSystem : public GraphicObj {
		public:
		 	// PUBLIC METHODS
			RefSystem();
			
			// hide the axis Z
			void HideZ();
		
			virtual VART::SceneNode * Copy();
		
			virtual void ComputeBoundingBox();
		
		private:
			// PRIVATE METHODS
			virtual bool DrawInstanceOGL() const;
			
		protected:
			// PROTECTED ATRIBUTES
			static double axisLength;
			Arrow axisX;
			Arrow axisY;
			Arrow axisZ;
			
	}; // end class declaration
} // end namespace

#endif
