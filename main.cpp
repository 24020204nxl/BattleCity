#include <sdl.h>
#include <iostream>
#include "game.h"
#include <SDL_image.h>
using namespace std;
//vong lap game
int main(int argc, char* argv[]) {
    Game game;
    if (game.running) {
        game.run();
    }
    return 0;
}

