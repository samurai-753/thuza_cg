/// \file point.h
/// \brief Header file for V-ART class "Point".
/// \version $Revision: 1.2 $

#ifndef VART_POINT_H
#define VART_POINT_H

namespace VART {
/// \class Point point.h
/// \brief Abstract class that provides a common base for point and vector classes.
/// \deprecated All derived classes, except for Point4D have been terminated. This class is no
///             longer needed.
    class Point {
        public:
            virtual ~Point() {};
            virtual float GetX() {return 0;};
            virtual float GetY() {return 0;};
            virtual float GetZ() {return 0;};
            virtual float GetW() {return 0;};
            virtual void GetXY(float &x, float &y) {};
            virtual void GetXZ(float &x, float &z) {};
            virtual void GetYZ(float &y, float &z) {};
            virtual void GetXYZ(float &x, float &y, float &z) {};
            virtual void GetXYZW(float &x, float &y, float &z, float &w) {};
            virtual void SetX(float x) {};
            virtual void SetY(float y) {};
            virtual void SetZ(float z) {};
            virtual void SetW(float z) {};
            virtual void SetXY(float x, float y) {};
            virtual void SetXZ(float x, float z) {};
            virtual void SetYZ(float y, float z) {};
            virtual void SetXYZ(float x, float y, float z) {};
            virtual void SetXYZW(float x, float y, float z, float w) {};
    }; // end class declaration
} // end namespace
#endif  // VART_POINT_H
