#include "point.h"
#include <cfloat>
#include <cmath>

using namespace std;

Point::Point (double x, double y)
    : mX(x), mY(y)
{}

Point Point::operator+(const Point& p) const
{
    return Point(mX + p.mX, mY + p.mY);
}

Point Point::operator-(const Point& p) const
{
    return Point(mX - p.mX, mY - p.mY);
}

Point Point::operator*(double s) const
{
    return Point(mX * s, mY * s);
}

bool Point::Origin() const
{
    return (fabs(mX + mY) < DBL_EPSILON);
}

void Point::Read(const string& prompt)
{
    cout << prompt;
    cin >> mX >> mY;
}

ostream& operator << (ostream& os, const Point& p)
{
    return os << "(" << p.mX << ", " << p.mY << ")";
}

