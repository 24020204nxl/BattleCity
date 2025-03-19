#include "bullets.h"
Bullets::Bullets(int startX,int startY,int dirX,int dirY){
    x=startX;
    y=startY;
    dx=dirX;
    dy=dirY;
    active=true;
    inBush=false;
    rect={x,y,10,10};
}
void Bullets::move(){
    x+=dx;
    y+=dy;
    rect.x=x;
    rect.y=y;
    if(x<TITLE_SIZE||x>SCREEN_WIDTH-TITLE_SIZE||y>SCREEN_HEIGHT-TITLE_SIZE||y<TITLE_SIZE){
        active=false;
    }
}
void Bullets::render(SDL_Renderer* renderer){
    if(active&&!inBush){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer,&rect);
    }
}
