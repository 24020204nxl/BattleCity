#ifndef BULLETS_H_INCLUDED
#define BULLETS_H_INCLUDED
#include "constants.h"

class Bullets{
public:
    int x,y;
    int dx,dy;
    int dirX,dirY;
    bool active;
    bool inBush;
    SDL_Rect rect;
    Bullets():x(0),y(0),dirX(0),dirY(0){};
    Bullets(int startX,int startY,int dirX,int dirY);
    void move();
    void render(SDL_Renderer* renderer);
};

#endif // BULLETS_H_INCLUDED
