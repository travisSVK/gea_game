#include "GameSystem.hpp"


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
        float newTime = game.GetElapsedTime();
        float dt = newTime - lastTime;
        dt = dt * 1.0f;
        lastTime = newTime;

        game.Update(dt);
        game.Draw();
    }

    // clean up
    game.Destroy();

    return 0;
}
