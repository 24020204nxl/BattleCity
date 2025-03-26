#ifndef BULLETS_H_INCLUDED
#define BULLETS_H_INCLUDED
#include "constants.h"


class Bullets{
public:
    int x,y;
    int dx,dy;
    int dirX,dirY;
    Direction direction;
    bool active;
    bool inBush;
    SDL_Rect rect;
    Bullets():x(0),y(0),direction(UP){};
    Bullets(int startX,int startY,Direction directionX);
    void move();
    void render(SDL_Renderer* renderer);
};
class Lazer{
public:
    int x,y;
    SDL_Rect rect;
    bool active;
    bool canCollide=false;
    int currentFrame;
    int frameCount;
    int animationSpeed;
    SDL_Texture* lazerFrames[4];
    Lazer() : x(0), y(0), active(false) {}
    Lazer(int startX,int startY);
    ~Lazer();
    void activate(int _x,int _y) {
        x=_x;
        y=_y;
        active = true;
    }
    void deactivate() {
        active = false;
    }
    void update();
    void render(SDL_Renderer* renderer);
    void loadFrames(SDL_Renderer* renderer);
};
#endif // BULLETS_H_INCLUDED
