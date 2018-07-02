#include <windows.h>
#include <SDL2/SDL.h>

#include <evolution.h>

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    evolution evo;
    evo.run();

    return 0;
}
