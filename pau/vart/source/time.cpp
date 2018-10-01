/// \file time.cpp
/// \brief Implementation file for V-ART class "Time".
/// \version $Revision: 1.5 $

#include "vart/time.h"

#ifdef __linux__  // compiling on a Linux system?
#include <sys/time.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

VART::Time::Time()
{
}

VART::Time::Time(const Time& t) : seconds(t.seconds)
{
}

VART::Time::Time(double newSeconds) : seconds(newSeconds)
{
}

unsigned int VART::Time::WholeMicroseconds() const
{
    return static_cast<unsigned int>(seconds * 1000000);
}

unsigned int VART::Time::WholeMilliseconds() const
{
    return static_cast<unsigned int>(seconds * 1000);
}

unsigned int VART::Time::WholeSeconds() const
{
    return static_cast<unsigned int>(seconds);
}

VART::Time VART::Time::operator-(const VART::Time& initialTime) const
{
    return VART::Time(seconds - initialTime.seconds);
}

void VART::Time::Set()
{
#ifdef __linux__
    static timeval temp;
    gettimeofday(&temp, NULL);
    seconds = static_cast<double>(temp.tv_sec) + (static_cast<double>(temp.tv_usec)/1000000.0f);
#endif

#ifdef WIN32
    LARGE_INTEGER perfCounter;
    LARGE_INTEGER perfFrequency;

    QueryPerformanceFrequency(&perfFrequency);

    QueryPerformanceCounter(&perfCounter);
    seconds = (((double)perfCounter.QuadPart) / ((double) perfFrequency.QuadPart));
#endif
}

const VART::Time& VART::Time::NOW()
// static method
{
    static VART::Time time;
    time.Set();
    return time;
}

namespace VART {
    ostream& operator<<(ostream& output, const Time& t)
    {
        output << t.seconds;
        return output;
    }
}
