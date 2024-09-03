#ifndef GAME_H
#define GAME_H

#include "Graphics.h"
#include "Window.h"
#include "Input.h"

namespace Tina
{
	class Game
	{
	protected:
		static Graphics *& graphics;
		static Window *& window;
		static Input *& input;
		static double & frameTime;

	public:
		Game() noexcept;
		virtual ~Game();

		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Finalize() = 0;

		virtual void Draw();
		virtual void Display();
		virtual void OnPause() { Sleep(10); }
	};
}

#endif