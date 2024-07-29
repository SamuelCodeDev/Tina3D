#ifndef ENGINE_H
#define ENGINE_H

#include "Game.h"

namespace Tina
{
    class EngineDesc
    {
    protected:
        //static Timer timer;
        //static bool paused;

    public:
        //static Graphics* graphics;
        static TinaWindow* window;
        //static Input* input;
        static Game* game;
        //static double frameTime;

        //static void Pause() noexcept;
        //static void Resume() noexcept;
    };

    //inline void EngineDesc::Pause() noexcept
    //{ paused = true; timer.Stop(); }

    //inline void EngineDesc::Resume() noexcept
    //{ paused = false; timer.Start(); }
}

#ifdef _WIN32

namespace Tina::Windows
{
    class Engine final : public EngineDesc
    {
    private:
        //double FrameTime();
        int32 Loop() noexcept;

    public:
        Engine() noexcept;
        ~Engine() noexcept;

        int32 Start(Game* game);

        static LRESULT CALLBACK EngineProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}

#elif __linux__

namespace Tina::Linux
{
    class Engine final : public EngineDesc
    {
    private:
        //double FrameTime();
        int32 Loop() noexcept;

    public:
        Engine() noexcept;
        ~Engine() noexcept;

        int32 Start(Game* game);
    };
}

#endif

#endif