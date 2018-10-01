#ifndef POINT_H
#define POINT_H

#include <string>
#include <iostream>


class Point
{
    friend std::ostream& operator << (std::ostream& os, const Point& p);
    public:
        Point() {}
        Point(double x, double y);

        Point operator+(const Point& p) const;
        Point operator-(const Point& p) const;
        Point operator*(double s) const;

        void Read(const std::string& prompt);
        bool Origin() const;

        double mX;
        double mY;

    private:

};
#endif

