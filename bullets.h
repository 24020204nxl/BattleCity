#ifndef BULLETS_H_INCLUDED
#define BULLETS_H_INCLUDED
#include "constants.h"


class Bullets{
public:
    int x,y; //vi tri ban dau
    int dx,dy; //vi tri sau do
    int dirX,dirY;
    Direction direction; //huong
    bool active; //Ktra hoat dong
    bool inBush;//Ktra trong bui co
    SDL_Rect rect;
    Bullets():x(0),y(0),direction(UP){}; //Khoi tao mac dinh
    Bullets(int startX,int startY,Direction directionX);
    void move();
    void render(SDL_Renderer* renderer);
};
class Lazer{
public:
    int x,y;
    SDL_Rect rect;
    bool active;
    bool canCollide=false; //Ktra dieu kien va cham
    int currentFrame;
    int frameCount;
    int animationSpeed;
    SDL_Texture* lazerFrames[4];
    Lazer() : x(0), y(0), active(false) {}
    Lazer(int startX,int startY);
    ~Lazer();//Ham huy
    //Ham khoi dong lazer
    void activate(int _x,int _y) {
        x=_x;
        y=_y;
        rect.x=_x;
        rect.y=_y;
        rect.w=60;
        rect.h=400;
        active = true;
    }
    //Ham tat lazer
    void deactivate() {
        active = false;
    }
    void update();
    void render(SDL_Renderer* renderer);
    void loadFrames(SDL_Renderer* renderer);
};
#endif // BULLETS_H_INCLUDED
