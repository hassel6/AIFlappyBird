
#include "headers/GameEngine.h"

int main() {
    srand( time( NULL ) );

    int population = 300;
    GameEngine engine( population );

    engine.gameLoop();

    return 0;
}
