/// \file color.cpp
/// \brief Implementation file for V-ART class "Color".
/// \version $Revision: 1.3 $

#include "vart/color.h"
#include <cassert>
#include <cstdlib>

#include <cmath>
#include <iostream>
using namespace std;

typedef unsigned char uchar;

VART::Color::Color() {
    rgba[0] = 0;
    rgba[1] = 0;
    rgba[2] = 0;
    rgba[3] = 255;
}

VART::Color::Color(uchar r, uchar g, uchar b, uchar a) {
// a defaults to 255 (see header)
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}

VART::Color::Color(const VART::Color& color) {
    rgba[0] = color.rgba[0];
    rgba[1] = color.rgba[1];
    rgba[2] = color.rgba[2];
    rgba[3] = color.rgba[3];
}

VART::Color VART::Color::HSV(uchar h_, uchar s_, uchar v_)
// Conversion from http://www.cs.rit.edu/~ncs/color/t_convert.html
{
    if (s_ == 0) // achromatic (grey)
        return VART::Color(v_, v_, v_, 255);
        
    float h = h_ / 42.5; // sector 0 to 5
    float s = s_ / 255.0;
    float v = v_ / 255.0;

    int i = floor(h);
    float f = h - i;     // fractional part of h
    uchar p = static_cast<uchar>(v * (1 - s) * 255);
    uchar q = static_cast<uchar>(v * (1 - s * f) * 255);
    uchar t = static_cast<uchar>(v * (1 - s * (1 - f)) * 255);

    switch(i) {
        case 0:
            return VART::Color(v_, t, p, 255);
        case 1:
            return VART::Color(q, v_, p, 255);
        case 2:
            return VART::Color(p, v_, t, 255);
        case 3:
            return VART::Color(p, q, v_, 255);
        case 4:
            return VART::Color(t, p, v_, 255);
        default:
            return VART::Color(v_, p, q, 255);
    }
}


const VART::Color& VART::Color::BLACK() {
    static const VART::Color black(0,0,0,255);
    return black;
}

const VART::Color& VART::Color::RED() {
    static const VART::Color red(255,0,0,255);
    return red;
}

const VART::Color& VART::Color::GREEN() {
    static const VART::Color green(0,255,0,255);
    return green;
}

const VART::Color& VART::Color::BLUE() {
    static const VART::Color blue(0,0,255,255);
    return blue;
}

const VART::Color& VART::Color::MAGENTA() {
    static const VART::Color magenta(255,0,255,255);
    return magenta;
}

const VART::Color& VART::Color::CYAN() {
    static const VART::Color cyan(0,255,255,255);
    return cyan;
}

const VART::Color& VART::Color::YELLOW() {
    static const VART::Color yellow(255,255,0,255);
    return yellow;
}

const VART::Color& VART::Color::WHITE() {
    static const VART::Color white(255,255,255,255);
    return white;
}

VART::Color VART::Color::RANDOM() {
    VART::Color color;
    unsigned char byte;
    byte = static_cast<unsigned char>(rand() % 256);
    color.SetR(byte);
    byte = static_cast<unsigned char>(rand() % 256);
    color.SetG(byte);
    byte = static_cast<unsigned char>(rand() % 256);
    color.SetB(byte);
    color.SetA(255);
    return color;
}

void VART::Color::SetRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}

void VART::Color::GetRGBA(unsigned char* r, unsigned char* g,
                      unsigned char* b, unsigned char* a) const {
    *r = rgba[0];
    *g = rgba[1];
    *b = rgba[2];
    *a = rgba[3];
}

void VART::Color::Get(float rgbaVec[4]) const {
    rgbaVec[0] = rgba[0]/255.0f;
    rgbaVec[1] = rgba[1]/255.0f;
    rgbaVec[2] = rgba[2]/255.0f;
    rgbaVec[3] = rgba[3]/255.0f;
}
VART::Color& VART::Color::operator=(const VART::Color& color) {
    rgba[0] = color.rgba[0];
    rgba[1] = color.rgba[1];
    rgba[2] = color.rgba[2];
    rgba[3] = color.rgba[3];
    return *this;
}

VART::Color VART::Color::operator+(const VART::Color& color) const {
    VART::Color c(rgba[0]+color.rgba[0], rgba[1]+color.rgba[1],
              rgba[2]+color.rgba[2], rgba[3]+color.rgba[3]);
    return c;
}

VART::Color VART::Color::operator-(const VART::Color& color) const {
    VART::Color c(rgba[0]-color.rgba[0], rgba[1]-color.rgba[1],
              rgba[2]-color.rgba[2], rgba[3]-color.rgba[3]);
    return c;
}

bool VART::Color::operator==(const VART::Color& color) const {
    return ( rgba[0] == color.rgba[0] &&
             rgba[1] == color.rgba[1] &&
             rgba[2] == color.rgba[2] &&
             rgba[3] == color.rgba[3] );
}

bool VART::Color::operator!=(const VART::Color& color) const {
    return ( rgba[0] != color.rgba[0] ||
             rgba[1] != color.rgba[1] ||
             rgba[2] != color.rgba[2] ||
             rgba[3] != color.rgba[3] );
}

void VART::Color::GetScaled(float escalar, float result[4]) const {
    assert(escalar >= 0);
    assert(escalar <= 1);
    result[0] = rgba[0]/255.0f * escalar;
    result[1] = rgba[1]/255.0f * escalar;
    result[2] = rgba[2]/255.0f * escalar;
    result[3] = rgba[3]/255.0f;
}

void VART::Color::GetScaled(float escalar, VART::Color* result) const {
    assert(escalar >= 0);
    assert(escalar <= 1);
    result->rgba[0] = static_cast<unsigned char>(rgba[0] * escalar);
    result->rgba[1] = static_cast<unsigned char>(rgba[1] * escalar);
    result->rgba[2] = static_cast<unsigned char>(rgba[2] * escalar);
    result->rgba[3] = rgba[3];
}

namespace VART {
    ostream& operator<<(ostream& output, const Color& c)
    {
        output << "{" << static_cast<int>(c.rgba[0]) << " "
            << static_cast<int>(c.rgba[1]) << " " << static_cast<int>(c.rgba[2])
            << " " << static_cast<int>(c.rgba[3]) << "}";
        return output;
    }
}
