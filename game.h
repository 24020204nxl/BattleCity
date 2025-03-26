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
#include "audio.h"
using namespace std;
enum GameState {MENU, SINGLEPLAYER, MULTIPLAYER, PAUSE, LOADSCREEN, VICTORY, LOSS, EXIT};
class Game{
public:
    SDL_Window* window;
    bool running;
    bool loadedGameFromSave=false;
    SDL_Renderer* renderer;
    SDL_Texture* spritesheet;
    SDL_Texture* loadTexture(const string &path);
    GameState gstate=MENU;
    GameState prevState;
    int menuSelection=0;
    int loadScreenSelection=0;
    int currentLevel=1;
    const int maxLevel=3;
    int pauseSelection=0;
    int loseSelection=0;
    int winSelection=0;
    vector<Wall> walls;
    vector<EnemyTank> enemies;
    PlayerTank player;
    PlayerTank player2;
    Boss boss=Boss(((MAP_WIDTH) / 2) * TITLE_SIZE,TITLE_SIZE);
    int enemynumber=1;
    Mix_Music* prevMusic = nullptr;
    Game();
    void saveGame();
    void loadGame();
    void handleEvent();
    void handleMenuEvent();
    void handleLoadEvent();
    void renderLoad(int loadScreenSelection);
    void handlePauseEvent();
    void renderPause(int pauseSelection);
    void handleLoss();
    void renderLoss(int loseSelection);
    void handleWin();
    void renderWin(int winSelection);
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
