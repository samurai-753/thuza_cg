#ifndef CLIP_RECTANGLE_H
#define CLIP_RECTANGLE_H

#include "line.h"
#include <string>

class ClipRectangle
{
    friend std::ostream& operator << (std::ostream& os, const ClipRectangle& r);
    public:
        ClipRectangle() : mInitialized(false) {}
        ClipRectangle(double xmin, double xmax, double ymin, double ymax);
        void Set(double xmin, double xmax, double ymin, double ymax);
        Line* Clip(const Line& l);
        void Read(const std::string& prompt);
        bool Initialized() { return mInitialized; }
        double XMin() { return mXmin; }
        double XMax() { return mXmax; }
        double YMin() { return mYmin; }
        double YMax() { return mYmax; }

    private:
        bool Positive(double n) { return n > 0; }

        double mXmin;
        double mXmax;
        double mYmin;
        double mYmax;
        bool mInitialized;
};

#endif


