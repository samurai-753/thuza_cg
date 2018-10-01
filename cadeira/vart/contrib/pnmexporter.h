/// \file /home/bruno/data/cvs/vpat/vart/contrib/pnmexporter.h
/// \brief Header file for V-ART class "PNMExporter".
/// \version $Revision: 1.1 $

#ifndef VART_PNMEXPORTER_H
#define VART_PNMEXPORTER_H

#include "vart/contrib/viewerglutogl.h"

namespace VART {
/// \class PNMExporter pnmexporter.h
/// \brief Exports the frame buffer to a PNM file.
/// \bug The frame buffer size needs to be mannualy set. Beware of screen resizes.
/// Please fix this by creating a resize handler for viewers.
/// \bug PNMs are as bad as BMPs. Saving several frames will eat all your HD space.
/// Please fix this by creating a PNG exporter :-).
///
/// A PNMExporter is a ViewerGlutOGL::DrawHandler that reads the frame buffer and writes it to a file.
/// Created to make videos from V-ART display.
    class PNMExporter : public ViewerGlutOGL::DrawHandler
    {
        public:
        // PUBLIC STATIC METHODS
        // PUBLIC METHODS
            PNMExporter();
            virtual ~PNMExporter();
            /// \brief Changes the buffer size.
            void SetBufferSize(int columns, int lines);
            /// \brief Reads frame buffer and writes to file.
            ///
            /// Output goes to file "frame00000.pnm" where "00000" gets incremented
            /// on every call.
            virtual void OnDraw();
        protected:
        // PROTECTED STATIC METHODS
        // PROTECTED METHODS
        // PROTECTED STATIC ATTRIBUTES
        // PROTECTED ATTRIBUTES
            /// buffer for image data
            unsigned int* dataPtr;
            /// number of columns in buffer
            int numColumns;
            /// number of lines in buffer
            int numLines;
    }; // end class declaration
} // end namespace

#endif
