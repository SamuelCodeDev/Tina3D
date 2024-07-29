#include "GameDemo.h"
#include "KeyCodes.h"

namespace Tina
{
    void GameDemo::Init()
    {
    }

    void GameDemo::Update()
    {
        if (input->TKeyPress(VK_Q))
            window->Close();
    }

    void GameDemo::Display()
    {
    }
    
    void GameDemo::Finalize()
    {
    }
}