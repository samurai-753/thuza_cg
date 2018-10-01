#ifndef VART_ARROW_H
#define VART_ARROW_H

#include "meshobject.h"

namespace VART {
	class Arrow : public MeshObject {
		public:
		 	// PUBLIC METHODS
			Arrow(double length);
			Arrow(Point4D point, Point4D direction);
			
			static float relativeHeadRadius;
			
		private:
			// PRIVATE METHODS
			void Inicializa(double lenght);
			
		protected:
			// PROTECTED ATRIBUTES
			static float relativeRadius;
			static float relativeBaseLength;
		
	}; // end class declaration
} // end namespace
#endif
