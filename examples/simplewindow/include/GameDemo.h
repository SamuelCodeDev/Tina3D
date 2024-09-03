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
		void Finalize() override;
		void Draw() override;
    };
}

#endif