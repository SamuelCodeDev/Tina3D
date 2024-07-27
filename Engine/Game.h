#ifndef GAME_H
#define GAME_H

#include "Window.h"

#ifdef _WIN32
	using Window = Tina::Windows::Window;
#elif __linux__
    using Window = Tina::Linux::Window;
#endif

namespace Tina
{
	class Game
	{
    protected:
        static Window*& window;

    public:
        Game() noexcept;
        virtual ~Game();

        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Finalize() = 0;

        virtual void Draw();
        virtual void Display();
        //virtual void OnPause();
	};
}

#endif