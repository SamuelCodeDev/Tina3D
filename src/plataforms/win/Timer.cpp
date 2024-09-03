#include "Timer.h"

namespace Tina
{
    Timer::Timer() noexcept : start{}, end{}, stoped{false}
    {
        if (!freq.QuadPart)
            QueryPerformanceFrequency(&freq);
    }

    void Timer::Start() noexcept
    {
        if (stoped)
        {
            // resumes time counting
            //
            //      <--- elapsed ---->
            // ----|------------------|------------> time
            //    start               end     
            
            uint64 elapsed = end.QuadPart - start.QuadPart;
            
            QueryPerformanceCounter(&start); 
            start.QuadPart -= elapsed;

            stoped = false;
        }
        else
        {
            QueryPerformanceCounter(&start);
        }
    }

    void Timer::Stop() noexcept
    {
        if (!stoped)
        {
            QueryPerformanceCounter(&end);
            stoped = true;
        }
    }

    double Timer::Reset() noexcept
    {
        uint64 elapsed;

        if (stoped)
        {
            elapsed = end.QuadPart - start.QuadPart;
            QueryPerformanceCounter(&start); 
            stoped = false;
        }
        else
        {
            QueryPerformanceCounter(&end);
            elapsed = end.QuadPart - start.QuadPart;
            start = end;
        }

        return elapsed / static_cast<double>(freq.QuadPart);    
    }

    double Timer::Elapsed() noexcept
    {
        uint64 elapsed;

        if (stoped)
        {
            elapsed = end.QuadPart - start.QuadPart;
        }
        else
        {
            QueryPerformanceCounter(&end);
            elapsed = end.QuadPart - start.QuadPart;
        }

        return elapsed / static_cast<double>(freq.QuadPart);
    }

    double Timer::Elapsed(const uint64 stamp) noexcept
    {
        uint64 elapsed;

        if (stoped)
        {
            elapsed = end.QuadPart - stamp;
        }
        else
        {
            QueryPerformanceCounter(&end);
            elapsed = end.QuadPart - stamp;
        }

        return elapsed / static_cast<double>(freq.QuadPart);
    }
}