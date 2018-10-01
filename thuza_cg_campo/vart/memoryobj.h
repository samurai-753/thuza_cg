/// \file memoryobj.h
/// \brief Header file for V-ART class "MemoryObj".
/// \version $Revision: 1.3 $

#ifndef VART_MEMORYOBJ_H
#define VART_MEMORYOBJ_H

namespace VART {
/// \class MemoryObj memoryobj.h
/// \brief A "Memory Object" is a V-ART object that has some memory allocation
/// attributes, allowing V-ART to auto deallocate its memory space.
///
/// A V-ART program typically needs to allocate memory for several scene objects that
/// should be deallocated upon application exit. V-ART code typically follows the rule that
/// memory allocated by the programmer should be deallocated by her, while memory allocated
/// by itself should be deallocated by itself. However, creating lots of graphical objects,
/// joints and DOFs, and keeping track of their memory positions is tedious hard work, that
/// may be avoided using some kind of memory management. The use of the autoDelete flag
/// helps creating a fast, user-controlled memory management.
///
/// \bug GLUT applications must call glutMainLoop which never returns, therefore stdlib's
/// exit function must be called to end an application. This prevents stack objects from
/// being destructed, which makes the creation on memory management policies based on
/// destructors a lot harder. It is recommended that key objects in the application (the
/// scene, the viewer, the lights) are declared "static" to ensure activation of their
/// destructors upon application exit.
    class MemoryObj {
        public:
            /// \brief Default constructor.
            ///
            /// Initializes the autoDelete flag with false, i.e.: all Memory Objects arent deallocated
            /// by default upon application termination.
            MemoryObj();
            
            /// \brief Copy constructor
            ///
            /// Initialize the autoDelete flag with the same value of obj, i.e.: the new memory objects 
            /// are deallocated if the original object are deallocated.
            MemoryObj(const VART::MemoryObj& obj);
            bool autoDelete;
    }; // end class declaration
} // end namespace

#endif
