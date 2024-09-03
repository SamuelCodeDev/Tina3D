#ifndef DXUT_TIMER_H
#define DXUT_TIMER_H

#include <windows.h>
#include "Types.h"

namespace Tina
{
    class Timer final
    {
    private:
        static inline LARGE_INTEGER freq {};
        LARGE_INTEGER start, end;
        bool stoped;
        
    public:
        Timer() noexcept;

        void Start() noexcept;
        void Stop() noexcept;
        double Reset() noexcept;
        
        double Elapsed() noexcept;
        bool Elapsed(const double secs) noexcept;
        
        uint64 Stamp() noexcept;
        
        double Elapsed(const uint64 stamp) noexcept;
        bool Elapsed(const uint64 stamp, const double secs) noexcept;
    }; 

    inline bool Timer::Elapsed(const double secs) noexcept
    { return (Elapsed() >= secs ? true : false); }

    inline bool Timer::Elapsed(const uint64 stamp, const double secs) noexcept
    { return (Elapsed(stamp) >= secs ? true : false); }

    inline uint64 Timer::Stamp() noexcept
    {
        QueryPerformanceCounter(&end);
        return end.QuadPart;
    }
}

#endif