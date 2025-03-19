#include "wall.h"
#include "game.h"
Wall::Wall(int startX,int startY,WallType Thetype){
    x=startX;
    y=startY;
    type=Thetype;
    active=true;
    rect.x = x;
    rect.y = y;
    rect.w = TITLE_SIZE;
    rect.h = TITLE_SIZE;

}
void Wall::render(SDL_Renderer* renderer){
    if(!active) return;
    if(spritesheet){
        SDL_RenderCopy(renderer,spritesheet,&scrRect,&rect);
    }
}
void Wall::setSpriteSheet(SDL_Texture* sheet,SDL_Rect source){
    spritesheet=sheet;
    scrRect=source;
}
