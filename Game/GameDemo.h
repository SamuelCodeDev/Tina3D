#ifndef GAMEDEMO_H
#define GAMEDEMO_H

#include "All.h"

namespace Tina
{
	class GameDemo : public Game
	{
	private:

	public:
		void Init() override;
		void Update() override;
		void Display() override;
		void Finalize() override;
	};
}

#endif