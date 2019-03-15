#include "GameSystem.hpp"

const unsigned int FPS = 60;
int main(int argc, char* argv[])
{
    GameSystem game;
    if (!game.Init(800, 600))
    {
        return 1;
    }
    game.Create();

    float lastTime = game.GetElapsedTime();
    while (!game.GameOver())
    {
        float startTime = game.GetElapsedTime();
        game.Update();
        game.Draw();

        float elapsedTime = game.GetElapsedTime();
        int sleepTime = (1000 / FPS) - (elapsedTime - startTime);
        //SDL_Log("%d\n", sleepTime);
        if (sleepTime > 0)
        {
            SDL_Delay(sleepTime);
        }
    }

    // clean up
    game.Destroy();

    return 0;
}
