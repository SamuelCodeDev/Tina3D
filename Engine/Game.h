#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Input.h"

#ifdef _WIN32
	using TinaWindow = Tina::Windows::Window;
	using Input = Tina::Windows::Input;
#elif __linux__
	using TinaWindow = Tina::Linux::Window;
	using Input = Tina::Linux::Input;
#endif

namespace Tina
{
	class Game
	{
	protected:
		static TinaWindow *& window;
		static Input *& input;

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