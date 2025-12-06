#include "GameEngine.h"
#include <ctime>
#include <cstdlib>

int main() {
    srand(static_cast<unsigned>(time(0)));
    GameEngine game;
    game.run();
    return 0;
}
