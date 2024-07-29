#ifndef GAME_H
#define GAME_H

#include "Window.h"

#ifdef _WIN32
	using TinaWindow = Tina::Windows::Window;
#elif __linux__
    using TinaWindow = Tina::Linux::Window;
#endif

namespace Tina
{
	class Game
	{
    protected:
        static TinaWindow *& window;

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