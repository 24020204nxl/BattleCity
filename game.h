#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <vector>
#include <SDL.h>
#include <fstream>
#include <iostream>
#include <string>
#include "constants.h"
#include "wall.h"
#include "tank.h"
#include "bullets.h"
using namespace std;
enum TrangthaiGame {MENU, SINGLEPLAYER, MULTIPLAYER,PAUSE, EXIT};
class Game{
public:
    SDL_Window* window;
    bool running;
    SDL_Renderer* renderer;
    SDL_Texture* spritesheet;
    SDL_Texture* loadTexture(const string &path);
    TrangthaiGame gstate=MENU;
    TrangthaiGame prevState;
    int menuSelection=0;
    int currentLevel=1;
    const int maxLevel=3;
    int pauseSelection=0;
    vector<Wall> walls;
    vector<EnemyTank> enemies;
    PlayerTank player;
    PlayerTank player2;
    int enemynumber=5;
    Game();
    void saveGame();
    void handleEvent();
    void handleMenuEvent();
    void handlePauseEvent();
    void renderPause(int pauseSelection);
    void loadLevel(int level);
    void renderMenu(const int &menuSelection);
    void render();
    void generateWall(const string &mapFile);
    void spawnEnemyTank();
    void run();
    void update();
    ~Game();
};

#endif // GAME_H_INCLUDED
