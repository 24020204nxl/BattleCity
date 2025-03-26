#ifndef TANK_H_INCLUDED
#define TANK_H_INCLUDED
#include <vector>
#include "wall.h"
#include "constants.h"
#include "bullets.h"
#include "audio.h"
using namespace std;
enum BattackType{IDLE, BULLET, LAZER};
class Wall;
class EnemyTank;
class PlayerTank{
public:
    int x,y;
    int dirX,dirY;
    int ShootDelay=20;
    Direction direction=UP;
    SDL_Rect scrRect;
    SDL_Texture* spritesheet;
    int explosionFrame=-1;
    bool active;
    SDL_Rect rect;
    vector<Bullets> bullets;
    bool inBush=true;
    PlayerTank(): x(0),y(0),dirX(0),dirY(0),spritesheet(nullptr){}
    PlayerTank(int startX,int startY);
    void setSpriteSheet(SDL_Texture* sheet,SDL_Rect source);
    void move(int dx,int dy,const vector<Wall>&walls,const vector<EnemyTank>enemies);
    void render(SDL_Renderer* renderer);
    void shoot();
    void updateBullets(const vector<Wall>&walls);
};
class EnemyTank{
public:
    int x,y;
    int dirX,dirY;
    Direction direction;
    SDL_Rect rect;
    SDL_Rect scrRect;
    SDL_Texture* spritesheet;
    vector<Bullets> bullets;
    bool active;
    int moveDelay,shootDelay;
    int randomMoveTimer;
    int explosionFrame=-1;
    EnemyTank(): x(0),y(0),dirX(0),dirY(0),spritesheet(nullptr){}
    EnemyTank(int startX,int startY);
    void setSpriteSheet(SDL_Texture* sheet,SDL_Rect source);
    void move(const vector<Wall>&walls,const vector<EnemyTank>enemies,PlayerTank &player,PlayerTank &player2);
    void render(SDL_Renderer* renderer);
    void shoot();
    void updateBullets(const vector<Wall>&walls,const vector<EnemyTank>enemies,const PlayerTank &player,const PlayerTank &player2);
    void randomDirection(int screenheight);
};
class Boss{
public:
    int x,y;
    int health=5;
    int shootDelay,lazerDelay;
    int currentFrame=0;
    int frameCount;
    int animationSpeed;
    bool active=false;
    int explosionFrame = -1;
    int explosionCounter = 0;
    int explosionSpeed = 5;
    SDL_Texture* explosionTexture = nullptr;
    void loadExplosionTexture(SDL_Renderer* renderer);
    Lazer lazer;
    BattackType atkType=IDLE;
    SDL_Rect rect;
    SDL_Rect scrRect;
    SDL_Texture* bossFrames[8];
    vector<Bullets> bullets;
    Boss(): x(0),y(0){}
    Boss(int _x, int _y);
    void loadFrames(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void shoot();
    void shootLazer();
    void takeDamage();
    void updateAnimation();
    void updateBullets(const vector<Wall>&walls,const PlayerTank &player,const PlayerTank &player2);
    ~Boss();
};

#endif // TANK_H_INCLUDED
