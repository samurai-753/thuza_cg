/// \file color.h
/// \brief Header file for V-ART class "Color".
/// \version $Revision: 1.3 $

#ifndef VART_COLOR_H
#define VART_COLOR_H

#include <iostream>

namespace VART {
/// \class Color color.h
/// \brief RGBA color representation.
///
/// Colors are quadruplets of Red (R), Green (G), Blue (B) and Alpha (A) components
/// where A is the opacity of a color. Note that OpenGL requires that blending is
/// enabled in order to make actual use of the alpha value - this is NOT done automatically
/// by V-ART to produce faster drawings when blending is not needed. To enable blending
/// the programmer should do it in the application's main routine, with code like
/// "glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);".
/// Enabling blending may not be enough in some cases: OpenGL uses traditional
/// frame-buffer pixel blending which requires that farther fragments be drawn first in
/// order to achieve correct blending results. In other words, translucent objects may
/// appear opaque if drawn before the objects behind them.
    class Color {
        friend std::ostream& operator<<(std::ostream& output, const Color& c);
        public:
            /// Creates a black, opaque color.
            Color();
            Color(const Color& color);
            Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
            void SetR(unsigned char r) { rgba[0] = r; }
            void SetG(unsigned char g) { rgba[1] = g; }
            void SetB(unsigned char b) { rgba[2] = b; }
            void SetA(unsigned char a) { rgba[3] = a; }
            void SetRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
            unsigned char GetR() const { return rgba[0]; }
            unsigned char GetG() const { return rgba[1]; }
            unsigned char GetB() const { return rgba[2]; }
            unsigned char GetA() const { return rgba[3]; }
            void GetRGBA(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a) const;
            /// Returns RGBA componts as a vector of floats.
            void Get(float rgbaVec[4]) const;
            Color& operator=(const Color& color);
            Color operator+(const Color& color) const;
            Color operator-(const Color& color) const;
            bool operator==(const Color& color) const;
            bool operator!=(const Color& color) const;

            /// \brief Multiplies RGB components, keeps A component.

            /// Use this method to make a color darker, by multipling it by a number in
            /// the range 0..1.
            /// \param escalar [in] Number to multiply against RGB components. Should be
            /// in the range 0..1 in order to garantee that the result is in the range
            /// 0..1. No checking on the range is done.
            /// \param result [out] Result of multiplication as RGBA float vector.
            /// Components should be in the range 0..1, but no clamping is done.
            void GetScaled(float escalar, float result[4]) const;

            /// \brief Multiplies RGB components, keeps A component.

            /// Use this method to make a color darker, by multipling it by a number in
            /// the range 0..1.
            /// \param escalar [in] Number to multiply against RGB components. Should be
            /// in the range 0..1 in order to garantee that the result is in the range
            /// 0..1. No checking on the range is done.
            /// \param result [out] Result of multiplication as Color.
            /// Components should be in the range 0..1, but no clamping is done.
            void GetScaled(float escalar, Color* result) const;
        // PUBLIC STATIC METHODS
            /// HSV named Constructor
            static Color HSV(unsigned char h_, unsigned char s_, unsigned char v_);
            /// Black opaque color
            static const Color& BLACK();
            /// Red opaque color
            static const Color& RED();
            /// Green opaque color
            static const Color& GREEN();
            /// Blue opaque color
            static const Color& BLUE();
            /// Magenta opaque color
            static const Color& MAGENTA();
            /// Cyan opaque color
            static const Color& CYAN();
            /// Yellow opaque color
            static const Color& YELLOW();
            /// White opaque color
            static const Color& WHITE();

            /// \brief Returns a randomly chosen color.

            /// You may want to initialize the random number generator with something like:
            /// srand(static_cast<unsigned int>(time(NULL))) before generating random colors.
            static Color RANDOM();
        private:
        // PRIVATE ATTRIBUTES
            /// Internal representation.
            unsigned char rgba[4];
            // Note: OpenGL's glLight... and glMaterial... only have int/float versions.
        // PRIVATE STATIC METHODS
    }; // end class declaration
} // end namespace
#endif // _COLOR_H
