#include "GameWindow.h"


int main()
{
    GameWindow window(512, 512, "Bullet Kaos");

    if (!window.Initialize())
    {
        return -1;
    }

    window.game_main_loop();
    window.Shutdown();

    return 0;
}
