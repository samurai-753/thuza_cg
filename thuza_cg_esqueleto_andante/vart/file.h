/// \file file.h
/// \brief Header file for V-ART class "File".
/// \version $Revision: 1.1 $

#ifndef VART_FILE_H
#define VART_FILE_H

#include <string>

namespace VART {
/// \class File file.h
/// \brief A File is a system indepentend representation for methods that work with files.
///
/// The File class is a representation of a independent file system methods and
/// attributes.
    class File {
        public:
        // PUBLIC STATIC METHODS
            /// \Brief Returns the path to the directory that contains a file.
            /// Given a string that represents a path to a file, returns a string that
            /// represents a path to the directory that contains that file, by removing
            /// from fileName the name of the file. This method is used to read xml files
            /// or WaveFront .obj files from a directory different from the current.
            /// Note that in Linux systems, the character slash ('/') represents a directory
            /// separator. Otherwise, in Windows systems is used the character back slash ('\')
            /// for that.
            static std::string GetPathFromString(const std::string& fileName);
        private:
    }; // end class declaration
} // end namespace
#endif
