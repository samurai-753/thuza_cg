/// \file texture.h
/// \brief Header file for V-ART class "Texture".
/// \version $Revision: 1.4 $

#ifndef VART_TEXTURE_H
#define VART_TEXTURE_H

#include <string>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef VART_OGL
#include <GL/gl.h>
#endif

#ifdef IL_LIB
#include <IL/il.h>
#endif

namespace VART {
/// \class Texture texture.h
/// \brief 2D image to use as texture.
///
/// A texture is a 2D image to use as a texture in a polygon surface.
/// According to OpenGL Specification, the width and the height
/// of the image must be an power of 2 (ie.: 256x64, 512x512).
/// <br>
/// <br>
/// Note that OpenGl requires that texture mapping is enabled before you draw
/// a texture in a scene. V-Art does NOT enable it automaticaly. To enable 
/// texture mapping, the programmer should do it in the application's main routine, 
/// with code like "glEnable(GL_TEXTURE_2D); glEnableClientState(GL_TEXTURE_COORD_ARRAY);".
/// But be carefully. Use of "glEnableClientState(GL_TEXTURE_COORD_ARRAY)" with a
/// scene that has mesh objects with no texture coordinates can generate errors 
/// during calls for OpenGl "glDrawElements" function.
/// <br>
/// <br>
/// The image types that V-Art suports depends on image reader library,
/// that we use in this implementation of VART::Texture class. At this
/// moment, we used the Developer Image Library (http://openil.sourceforge.net/).
/// According to the Developer Image Library Reference, the supported image types 
/// are: .bmp, .cut, .dds, .doom, .gif, .ico, .jpg, .lbm, .mdl, .mng, .pal, 
/// .pbm, .pcd, .pcx, .pgm, .pic, .png, .ppm, .psd, .psp, .raw, .sgi, .tga, 
/// .tif and .hdr files. Note that to use the Developer Image Library support,
/// you must compile V-Art with IL_LIB symbol.
    class Texture {
        public:

            /// \brief Creates an unitialized texture.
            ///
            /// The texture created holds no texture data and, because this, 
            /// dont generate any side effects on any material that use it.
            Texture();

            /// \brief Creates a texture from an image file.
            Texture(const std::string& fileName);
            
            /// \brief Creates a copy of an texture.
            Texture(const Texture& texture);
            
            /// \brief Copies texture data.
            Texture& operator=(const Texture& texture);

            /// \brief Loads a texture from a file.
            ///
            /// Reads a image file and convert it to a graphic texture.

            /// \return True if fileName could be read. False if the file could not
            /// be read, or if V-Art was not compiled with OpenGL or an image reader 
            /// library suport.

            /// Note that the supported formats deppends on image reader library
            /// used in this implementation. See the Texture class description for 
            /// more details.
            bool LoadFromFile(const std::string& fileName);

            /// \brief Indicates if a texture has been loaded previously.
            /// \deprecated Use HasData instead.
            ///
            /// If a texture has been loaded previously with 
            /// 'VART::Texture::LoadFromFile()' method, returns true. Otherwise,
            /// returns false.
            bool HasTextureLoad() const;

            /// \brief Indicates if a texture object contains data.
            ///
            /// A texture contains data if it has been loaded from an image file using
            /// 'VART::Texture::LoadFromFile()' or if the data has been procedurally
            /// generated. There are no methods to generate procedural textures yet.
            bool HasData() const { return hasTexture; };

            /// \brief Destructor class.
            ///
            /// Deletes all texture data.
            virtual ~Texture();

            /// \brief Sets the texture to draw with the next object.
            ///
            /// Until another call to VART::Texture::DrawOGL, the current
            /// texture is used in subsequent drawing of objects.
            bool DrawOGL() const;

        private:
            /// Creates a white texture of size 1x1, to use when no image data has been
            /// loaded to use as texture.
            static void WhiteTexture();

            /// The default white texture OpenGl id.
            static unsigned int whiteTextureId;

            /// Indicates if a white texture has been created.
            static bool hasWhiteTexture;

            /// Indicates if a texture image is loaded in current Texture instance object.
            bool hasTexture;

            /// The openGl texture identifier.
            unsigned int textureId;

//            /// \brief Reference count to a texture
//            unsigned int refCount;

            /// \brief Status of Texture initialization.
            ///
            /// Texture mapping requires external libraries that often require initialization.
            /// Constructors use this value to know if such initialization is necessary, so that
            /// is done once, when the first texture object is created, releaving the programmer
            /// from such burden.
            static bool notInitalized;

            /// \brief Initializes texture dependencies.
            ///
            /// Initialization must be done once. See notInitalized.
            static void Initialize();

    }; // end class declaration
} // end namespace

#endif
