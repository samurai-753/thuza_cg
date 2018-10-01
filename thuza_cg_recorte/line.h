#ifndef LINE_H
#define LINE_H

#include "point.h"
#include <string>
#include <iostream>

class Line
{
    friend std::ostream& operator << (std::ostream& os, const Line& l);
    public:
        Line() {}
        Line(const Point& p0, const Point& p1);

        // Return line coordinates at parametric position t
        Point Coordinates(double t) const;

        void Read(const std::string& prompt);
        void calculaRecorte();
        // The points the define this line
        Point mP0;
        Point mP1;
};
#endif
			

