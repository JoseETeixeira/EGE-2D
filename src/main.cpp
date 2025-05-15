#include "Engine.h"

int main()
{
    Engine engine;
    if (!engine.Init())
    {
        return -1;
    }
    engine.Run();
    return 0;
}