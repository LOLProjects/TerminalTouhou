#include "FakeTerminal.h"

int main()
{
    FakeTerminal ft(sf::Vector2u(80, 24));

    if (!ft.init())
        return 1;

    ft.run();

    ft.stop();

    return 0;
}
