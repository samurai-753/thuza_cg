/// \file pnmexporter.cpp
/// \brief Implementation file for V-ART class "PNMExporter".
/// \version $Revision: 1.1 $

#include "vart/contrib/pnmexporter.h"
#include <sstream>
#include <cassert>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

VART::PNMExporter::PNMExporter() :
    dataPtr(NULL)
{
}

VART::PNMExporter::~PNMExporter()
{
    if (dataPtr)
        delete[] dataPtr;
}

void VART::PNMExporter::SetBufferSize(int columns, int lines)
{
    if (dataPtr)
        delete[] dataPtr;
    dataPtr = new unsigned int[columns*lines];
    numColumns = columns;
    numLines = lines;
}

void VART::PNMExporter::OnDraw()
// virtual
{
    assert(dataPtr && "No memory allocated for PNMExporter!");
    static int counter = 0;
    static ostringstream oss;
    oss.str("frame");
    oss.fill('0');
    oss << "frame" << setw(5) << counter << ".pnm";
    ++counter;
    ofstream outFile(oss.str().c_str());
    outFile << "P6\n#Creator: V-ART framework PNMExporter class\n"
            << numColumns << " " << numLines << "\n255\n";
    glReadPixels(0,0,numColumns,numLines,GL_RGBA,GL_UNSIGNED_BYTE,dataPtr);
    char* pixelPtr;
    for (int lin = numLines; lin > 0; --lin)
    {
        pixelPtr = reinterpret_cast<char*>(dataPtr + ((lin-1)*numColumns));
        for (int col = 0; col < numColumns; ++col)
        {
            outFile.write(pixelPtr,3);
            pixelPtr += 4;
        }
    }
}
