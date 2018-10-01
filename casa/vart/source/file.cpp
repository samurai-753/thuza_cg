/// \file file.cpp
/// \brief Implementation file for V-ART class "File".
/// \version $Revision: 1.1 $

#include "vart/file.h"

using namespace std;

std::string VART::File::GetPathFromString(const std::string& fileName)
{
    std::string path("");
    std::size_t pos;

#ifdef __linux__
    pos = fileName.find_last_of('/');
    if( pos!=string::npos && pos<fileName.size() )
    {
        path = fileName.substr(0, pos+1 );
    }
#endif

#ifdef WIN32
    //Windows systems acept the character '/' as a directory separator.
    //Replace with the correct directory separator ('\').
    std::string file( fileName );

    pos = file.find_last_of('/');
    while(pos!=string::npos)
    {
        if( pos<file.size() )
        {
            file.replace( pos, 1, "\\" );
        }
        pos = file.find_last_of('/');
    }
    pos = file.find_last_of('\\');
    if( pos!=string::npos && pos<file.size() )
    {
        path = file.substr(0, pos+1 );
    }
#endif

    return path;
}
