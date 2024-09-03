#include "Game.h"
#include "Engine.h"

namespace Tina
{
    Graphics *& Game::graphics = Engine::graphics;
    Window *& Game::window = Engine::window;
    Input *& Game::input = Engine::input;
    double & Game::frameTime = Engine::frameTime;

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
}