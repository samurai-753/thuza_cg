/// \file texture.cpp
/// \brief Implementation file for V-ART class "Texture".
/// \version $Revision: 1.4 $

#include "vart/texture.h"
#include <cassert>
#include <iostream>

using std::cerr;
using std::clog;
using std::string;

bool VART::Texture::notInitalized = true;
bool VART::Texture::hasWhiteTexture = false;
unsigned int VART::Texture::whiteTextureId = 0;

VART::Texture::Texture()
    : hasTexture(false), textureId(0)
// Creates a texture with no data that does not affect the redering of a material.
{
    if (notInitalized)
        Initialize();
    WhiteTexture();
}

VART::Texture::Texture(const string& fileName)
{
    if (notInitalized)
        Initialize();
    LoadFromFile(fileName);
}

VART::Texture::Texture(const VART::Texture& texture)
{
    this->operator=(texture);
}

bool VART::Texture::HasTextureLoad() const {
    cerr << "\aWarning: Texture::HasTextureLoad() is deprecated.\n";
    return hasTexture;
}

VART::Texture& VART::Texture::operator=(const VART::Texture& texture)
{
    textureId = texture.textureId;
    hasTexture = texture.hasTexture;
    return *this;
}

bool VART::Texture::LoadFromFile(const std::string& fileName)
{
    // The following symbols of devIL match OpenGL's:
    // IL_COLOUR_INDEX, IL_COLOR_INDEX, IL_ALPHA, IL_RGB, IL_RGBA, IL_BGR,
    // IL_BGRA, IL_LUMINANCE, IL_LUMINANCE_ALPHA

    // Reads the image data, and assign it to width, height and imageData variables.
    // This is dependent of the image reader library.
#ifdef IL_LIB
    ILuint ilTextName;

    ilGenImages(1, &ilTextName);
    ilBindImage(ilTextName);
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilLoadImage( fileName.c_str() );
    ILenum error = ilGetError();
    if (error != IL_NO_ERROR)
    {
        cerr << "Error while loading image file in Texture::LoadFromFile.\n"
             << "ilLoadImage returned " << error << "\n";
        throw 1;
    }
    unsigned int width = ilGetInteger(IL_IMAGE_WIDTH);
    unsigned int height = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    ILubyte* imageData = ilGetData();
    error = ilGetError();
    if (error == IL_NO_ERROR)
    {
        clog << "Image data from " << fileName << " loaded successfully.\n";
    }
    else
    {
        cerr << "Error while getting image data in Texture::LoadFromFile.\n"
             << "Caught error: " << error << "\n";
        throw 2;
    }
#else
#error No image handling library defined. Try compiling with -DIL_LIB.
#endif //IL_LIB

#ifdef VART_OGL
    //If we have the image data, generate a OpenGL texture.
    if(imageData != NULL)
    {
        hasTexture = true;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        assert(glGetError()==GL_NO_ERROR);
    }
    else
    {
        cerr << "Error binding texture in Texture::LoadFromFile.\n"
             << "Texture file: '" << fileName << "'\n"
             << "imageData: " << imageData << "\n";
        throw 3;
    }
#else
#error No rendering library defined. Try compiling with -DVART_OGL.
#endif //VART_OGL

#ifdef IL_LIB
    //Free the image data.
    ilDeleteImages(1, &ilTextName);
#endif //IL_LIB

    return true;
}

VART::Texture::~Texture()
{
    // FixMe: Manage reference count so that texture objects can be copied while texture data
    // remains valid in the renderer.

//#ifdef VART_OGL
    //if(textureId)
        //glDeleteTextures(1, &textureId);
//#endif //VART_OGL
}

bool VART::Texture::DrawOGL() const
{
#ifdef VART_OGL
    static bool textureIsEnabled = false;
    if (textureIsEnabled)
    {
        if (hasTexture)
        {
            glBindTexture(GL_TEXTURE_2D, textureId);
            //std::cout << "DrawOGL textureId: " << textureId << ".\n";
        }
        else
            {
                glDisable(GL_TEXTURE_2D);
                //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                textureIsEnabled = false;
            }
    }
    else
    { // texture is not enabled
        if (hasTexture)
        {
                glEnable(GL_TEXTURE_2D);
                textureIsEnabled = true;
                glBindTexture(GL_TEXTURE_2D, textureId);
        }
    }
#endif //VART_OGL
    return true;
}

void VART::Texture::WhiteTexture()
{
    if( VART::Texture::hasWhiteTexture )
        return;
#ifdef VART_OGL
    unsigned char data[3]={255,255,255};
    glGenTextures( 1, &whiteTextureId );
    glBindTexture( GL_TEXTURE_2D, whiteTextureId );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
    VART::Texture::hasWhiteTexture=true;
#endif //VART_OGL
}

void VART::Texture::Initialize()
{
    notInitalized = false;
#ifdef IL_LIB
    ilInit();
    clog << "libDevIL Initialized.\n";
#endif //IL_LIB
#ifdef VART_OGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    // FixMe: The user will not always want GL_REPLACE
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    clog << "OpenGL texture Initialized.\n";
#endif //VART_OGL
}
