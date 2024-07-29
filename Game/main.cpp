#include "GameDemo.h"

#ifdef _WIN32
auto APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow) -> int
{
    using namespace Tina;

    #ifdef _DEBUG
    try
    {
        Windows::Engine* engine = new Windows::Engine();
        engine->window->Mode(WINDOWED);
        engine->window->Size(800, 600);
        engine->window->Color(0, 122, 204);
        engine->window->Title("Triangle");
        //engine->window->Icon(IDI_ICON);
        //engine->window->Cursor(IDC_CURSOR);
        //engine->window->LostFocus(Engine::Pause);
        //engine->window->InFocus(Engine::Resume);

        int32 exit = engine->Start(new GameDemo());

        delete engine;
        return exit;
    }
    catch (Error & e)
    {
        MessageBox(nullptr, e.ToString().data(), "Triangle", MB_OK);
        return 0;
    }

    #elif NDEBUG
    
    Tina::Windows::Engine* engine = new Tina::Windows::Engine();
    engine->window->Mode(WINDOWED);
    engine->window->Size(800, 600);
    engine->window->Color(0, 122, 204);
    engine->window->Title("Triangle");
    //engine->window->Icon(IDI_ICON);
    //engine->window->Cursor(IDC_CURSOR);
    //engine->window->LostFocus(Engine::Pause);
    //engine->window->InFocus(Engine::Resume);

    int32 exit = engine->Start(new GameDemo());

    delete engine;

    return exit;

    #endif
}
#else
int main(int argc, char ** argv)
{
    using namespace Tina;

    using Engine = Tina::Linux::Engine;

    Engine* engine = new Engine();
    //engine->window->Mode(WINDOWED);
    engine->window->Size(800, 600);
    //engine->window->Color(0, 122, 204);
    engine->window->Title("Triangle");
    //engine->window->Icon(IDI_ICON);
    //engine->window->Cursor(IDC_CURSOR);
    //engine->window->LostFocus(Engine::Pause);
    //engine->window->InFocus(Engine::Resume);

    int32 exit = engine->Start(new GameDemo());

    delete engine;

    return exit;
}
#endif