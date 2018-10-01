/// \file point4d.cpp
/// \brief Implementation file for V-ART class "Point4d".
/// \version $Revision: 1.6 $

#include "vart/point4d.h"
#include <cmath>

using namespace std;

// Default "less then" comparison uses "LexicographicalLess".
VART::Point4D::tCompareOper VART::Point4D::operatorLess = &VART::Point4D::LexicographicalLess;
float VART::Point4D::xWeight = 1;
float VART::Point4D::yWeight = 1;
float VART::Point4D::zWeight = 1;
double VART::Point4D::delta = 0.00001;

VART::Point4D::Point4D()
{
    vetCoord[0]=vetCoord[1]=vetCoord[2]=0;
    vetCoord[3]=1;
}

VART::Point4D::Point4D(const VART::Point4D &point)
{
    vetCoord[0] = point.vetCoord[0];
    vetCoord[1] = point.vetCoord[1];
    vetCoord[2] = point.vetCoord[2];
    vetCoord[3] = point.vetCoord[3];
}

VART::Point4D::Point4D(double x, double y, double z, double w)
// w = 1 (point) by default (see constructor signature)
{
    SetXYZW(x, y, z, w);
}

const VART::Point4D& VART::Point4D::X()
{
    static const VART::Point4D x(1.0,0.0,0.0,0.0);
    return x;
}

const VART::Point4D& VART::Point4D::Y()
{
    static const VART::Point4D y(0.0,1.0,0.0,0.0);
    return y;
}

const VART::Point4D& VART::Point4D::Z()
{
    static const VART::Point4D z(0.0,0.0,1.0,0.0);
    return z;
}

const VART::Point4D& VART::Point4D::DOWN()
{
    static const VART::Point4D down(0.0,-1.0,0.0,0.0);
    return down;
}

const VART::Point4D& VART::Point4D::ORIGIN()
{
    static const VART::Point4D origin(0.0,0.0,0.0,1.0);
    return origin;
}

void VART::Point4D::SetXY(double x, double y)
{
    vetCoord[0] = x;
    vetCoord[1] = y;
}

void VART::Point4D::SetXYZ(double x, double y, double z)
{
    vetCoord[0] = x;
    vetCoord[1] = y;
    vetCoord[2] = z;
}

void VART::Point4D::SetXYZW(double x, double y, double z, double w)
{
    vetCoord[0] = x;
    vetCoord[1] = y;
    vetCoord[2] = z;
    vetCoord[3] = w;
}

void VART::Point4D::Normalize()
// requires that x,y,z and w are not all zeros
{
    if (vetCoord[3] == 0)
    {
        // vector normalization
        double norma = Length();

        vetCoord[0] /= norma;
        vetCoord[1] /= norma;
        vetCoord[2] /= norma;
        vetCoord[3] = 0; // this seems to avoid some round off erros
    }
    else
    {
        // point normalization
        vetCoord[0] /= vetCoord[3];
        vetCoord[1] /= vetCoord[3];
        vetCoord[2] /= vetCoord[3];
        vetCoord[3] = 1;
    }
}

double VART::Point4D::Length() const
{
    return sqrt(vetCoord[0]*vetCoord[0] + vetCoord[1]*vetCoord[1] + vetCoord[2]*vetCoord[2]);
}

double VART::Point4D::DotProduct(const VART::Point4D& p) const
{
    return vetCoord[0]*p.vetCoord[0]+vetCoord[1]*p.vetCoord[1]+vetCoord[2]*p.vetCoord[2];
}

VART::Point4D VART::Point4D::CrossProduct(const VART::Point4D& p) const
{
    return VART::Point4D(GetY()*p.GetZ()-GetZ()*p.GetY(), GetZ()*p.GetX()-GetX()*p.GetZ(), GetX()*p.GetY()-GetY()*p.GetX(), 0);
}

double VART::Point4D::AngleTo(const VART::Point4D& p) const
// Note: This method requires both vectors to be normalized!
{
    double dp = DotProduct(p);
    // Sometimes, the Dot Product is greater then 1 due to floating point errors.
    if (dp > 1.0)
        return 0;
    else
    {
        if (dp < -1.0)
            return 3.1415926535897932384626433832795;
        return acos(dp);
    }
}

double VART::Point4D::AngleTo(const Point4D& p, const Point4D& ref) const
// Note: This method requires all three vectors to be normalized!
{
    double absoluteValue = AngleTo(p);
    Point4D crossProduct = CrossProduct(p);
    if (ref.AlmostEqual(crossProduct))
        return  absoluteValue;
    else
        return -absoluteValue;
}

double VART::Point4D::GenericAngleTo(const VART::Point4D& p) const
{
    // Sometimes, due to floating point errors, cossine is out of [-1..1], need to check...
    double cosValue = DotProduct(p) / (Length()*p.Length());
    if (cosValue > 1)
        return 0;
    else
    {
        if (cosValue < -1)
            return 3.1415926535897932384626433832795;
        return acos(cosValue);
    }
}

VART::Point4D VART::Point4D::operator-() const
{
    // To keep the nature of the point it seems to me that the w coordinate should
    // not be touched.
    return VART::Point4D(-vetCoord[0], -vetCoord[1], -vetCoord[2], vetCoord[3]);
}

VART::Point4D VART::Point4D::operator-(const VART::Point4D& point) const
{
    return VART::Point4D(vetCoord[0]-point.vetCoord[0],
                     vetCoord[1]-point.vetCoord[1],
                     vetCoord[2]-point.vetCoord[2],
                     vetCoord[3]-point.vetCoord[3]);
}

VART::Point4D VART::Point4D::operator+(const VART::Point4D& vector) const
// This operator should not be used to sum two points. No checking is done, however,
// because clipping algorithms benefit from operating with vetors made from the
// subtraction of non-normalized points. There is no easy way to know the nature
// data being processed here.
{
    return VART::Point4D(vetCoord[0]+vector.vetCoord[0],
                         vetCoord[1]+vector.vetCoord[1],
                         vetCoord[2]+vector.vetCoord[2],
                         vetCoord[3]+vector.vetCoord[3]);
}

void VART::Point4D::operator+=(const VART::Point4D& vector)
// Requires that the active object (a point) be normalized.
{
    vetCoord[0] += vector.vetCoord[0];
    vetCoord[1] += vector.vetCoord[1];
    vetCoord[2] += vector.vetCoord[2];
}

VART::Point4D VART::Point4D::operator*(double escalar) const
{
    return VART::Point4D(vetCoord[0]*escalar,
                         vetCoord[1]*escalar,
                         vetCoord[2]*escalar,
                         vetCoord[3]*escalar);
}

void VART::Point4D::operator*=(double escalar)
{
    vetCoord[0]*=escalar;
    vetCoord[1]*=escalar;
    vetCoord[2]*=escalar;
    vetCoord[3]*=escalar;
}

bool VART::Point4D::operator==(const VART::Point4D& point) const
{
    return (vetCoord[0] == point.vetCoord[0]) &&
           (vetCoord[1] == point.vetCoord[1]) &&
           (vetCoord[2] == point.vetCoord[2]) &&
           (vetCoord[3] == point.vetCoord[3]);
}

bool VART::Point4D::operator<(const VART::Point4D& p) const
{
    return (this->*operatorLess)(p);
}

bool VART::Point4D::operator!=(const VART::Point4D& point) const
{
    return (vetCoord[0] != point.vetCoord[0]) ||
           (vetCoord[1] != point.vetCoord[1]) ||
           (vetCoord[2] != point.vetCoord[2]) ||
           (vetCoord[3] != point.vetCoord[3]);
}

void VART::Point4D::operator=(const VART::Point4D& point)
{
    vetCoord[0] = point.vetCoord[0];
    vetCoord[1] = point.vetCoord[1];
    vetCoord[2] = point.vetCoord[2];
    vetCoord[3] = point.vetCoord[3];
}

VART::Point4D VART::Point4D::operator/( double escalar ) const
{
    return VART::Point4D(vetCoord[0]/escalar,
                         vetCoord[1]/escalar,
                         vetCoord[2]/escalar,
                         vetCoord[3]/escalar);
}

bool VART::Point4D::LexicographicalLess(const VART::Point4D& p) const
{
    if (vetCoord[0] == p.vetCoord[0])
    {
        if (vetCoord[1] == p.vetCoord[1])
        {
            if (vetCoord[2] == p.vetCoord[2])
                return vetCoord[3] < p.vetCoord[3];
            else
                return vetCoord[2] < p.vetCoord[2];
        }
        else
            return vetCoord[1] < p.vetCoord[1];
    }
    else
        return vetCoord[0] < p.vetCoord[0];
}

bool VART::Point4D::XThenZLess(const Point4D& p) const
{
    if (vetCoord[0] == p.vetCoord[0])
        return vetCoord[2] < p.vetCoord[2];
    else
        return vetCoord[0] < p.vetCoord[0];
}

bool VART::Point4D::YThenXLess(const Point4D& p) const
{
    if (vetCoord[1] == p.vetCoord[1])
        return vetCoord[0] < p.vetCoord[0];
    else
        return vetCoord[1] < p.vetCoord[1];
}

bool VART::Point4D::YThenZLess(const Point4D& p) const
{
    if (vetCoord[1] == p.vetCoord[1])
        return vetCoord[2] < p.vetCoord[2];
    else
        return vetCoord[1] < p.vetCoord[1];
}

bool VART::Point4D::ZThenXLess(const Point4D& p) const
{
    if (vetCoord[2] == p.vetCoord[2])
        return vetCoord[0] < p.vetCoord[0];
    else
        return vetCoord[2] < p.vetCoord[2];
}

bool VART::Point4D::ZThenYLess(const Point4D& p) const
{
    if (vetCoord[2] == p.vetCoord[2])
        return vetCoord[1] < p.vetCoord[1];
    else
        return vetCoord[2] < p.vetCoord[2];
}

bool VART::Point4D::WeightedLess(const Point4D& p) const
{
    return WeightedSum() < p.WeightedSum();
}

bool VART::Point4D::WeightedGreater(const Point4D& p) const
{
    return WeightedSum() > p.WeightedSum();
}

float VART::Point4D::WeightedSum() const
{
    return (vetCoord[0] * xWeight) + (vetCoord[1] * yWeight) + (vetCoord[2] * zWeight);
}

bool VART::Point4D::AlmostEqual(const Point4D& v) const
{
    return abs( vetCoord[0] - v.vetCoord[0]
              + vetCoord[1] - v.vetCoord[1]
              + vetCoord[2] - v.vetCoord[2] ) < delta;
}

namespace VART
{
    ostream& operator<<(ostream& output, const Point4D& p)
    {
        output.setf(ios::fixed);
        output.precision(5);
        output << "(" << p.GetX() << "," << p.GetY() << "," << p.GetZ() << ","
               << p.GetW() << ")";
        output.unsetf(ios::fixed);
        return output;
    }
}
