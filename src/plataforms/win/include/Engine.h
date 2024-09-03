#ifndef ENGINE_H
#define ENGINE_H

#include "Game.h"
#include "Timer.h"

namespace Tina
{
    class Engine final
    {
    private:
        static Timer timer;
        static bool paused;

        double FrameTime() noexcept;
        int32 Loop() noexcept;

    public:
        static Graphics * graphics;
        static Window * window;
        static Input * input;
        static Game * game;
        static double frameTime;

        Engine() noexcept;
        ~Engine() noexcept;

        int32 Start(Game* game);

        static void Pause() noexcept;
        static void Resume() noexcept;

        static LRESULT CALLBACK EngineProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };

    inline void Engine::Pause() noexcept
    { paused = true; timer.Stop(); }

    inline void Engine::Resume() noexcept
    { paused = false; timer.Start(); }
}

#endif