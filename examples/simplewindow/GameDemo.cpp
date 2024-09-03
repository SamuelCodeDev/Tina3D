#include "GameDemo.h"
#include "KeyCodes.h"

namespace Tina
{
    void GameDemo::Init()
    {
    }
    
    void GameDemo::Update()
    {
        if(input->KeyPress(VK_ESCAPE))
            window->Close();
    }

    void GameDemo::Finalize()
    {
    }
    
    void GameDemo::Draw()
    {
    }
}