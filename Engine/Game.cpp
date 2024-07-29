#include "Game.h"
#include "Engine.h"
#include <chrono>
#include <thread>
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

namespace Tina
{
    TinaWindow *& Game::window = EngineDesc::window;

    Game::Game() noexcept
    {
    }

    Game::~Game()
    {
    }

    void Game::Draw()
    {
    }

    void Game::Display()
    {
    }

    //void Game::OnPause()
    //{ sleep_for(milliseconds(10)); }
}