/// \file time.h
/// \brief Header file for V-ART class "Time".
/// \version $Revision: 1.8 $

#ifndef VART_TIME_H
#define VART_TIME_H

#include <iostream>

namespace VART {
/// \class Time time.h
/// \brief Elapsed time representation
///
/// Time represents the length of a time interval.
    class Time {
        friend std::ostream& operator<<(std::ostream& output, const Time& t);
        public:
        // PUBLIC TYPES
        // PUBLIC METHODS
            /// \brief Creates an unitialized object.
            Time();
            /// \brief Copy constructor.
            Time(const Time& t);
            /// \brief Cast constructor from double.
            Time(double newSeconds);
            /// \brief Set to current time.
            ///
            /// Makes the active object hold the elapsed time since "the epoch". In sloppy terms,
            /// it could be said that this method set the active object to hold current time. "Epoch"
            /// is some fixed point in time determined by the operating system (usually
            /// 0h of Jan 1, 1970).
            void Set();
            /// \brief Returns the number of whole microseconds in elapsed time.
            unsigned int WholeMicroseconds() const;
            /// \brief Returns the number of whole miliseconds in elapsed time.
            unsigned int WholeMilliseconds() const;
            /// \brief Returns the number of whole seconds in elapsed time.
            unsigned int WholeSeconds() const;
            /// \brief Returns elapsed time (number of seconds) as float.
            float AsFloat() const { return static_cast<float>(seconds); }
        // PUBLIC OPERATORS
            Time operator-(const Time& initialTime) const;
            bool operator>=(const Time& t) const { return seconds >= t.seconds; }
            bool operator<=(const Time& t) const { return seconds <= t.seconds; }
            bool operator>(const Time& t) const { return seconds > t.seconds; }
            bool operator<(const Time& t) const { return seconds < t.seconds; }
            void operator+=(double secs) { seconds += secs; }
            void operator-=(const Time& t) { seconds -= t.seconds; }
        // PUBLIC STATIC METHODS
            /// \brief Returns current time.
            ///
            /// Since Time represents elapsed time, NOW actually is the elapsed time since
            /// Epoch. See Set().
            static const Time& NOW();
        protected:
            /// \brief Number of seconds in elapsed time.
            double seconds;
        private:
    }; // end class declaration
} // end namespace

#endif
