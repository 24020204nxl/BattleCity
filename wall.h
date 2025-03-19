#ifndef WALL_H_INCLUDED
#define WALL_H_INCLUDED
#include <SDL.h>
enum WallType{BRICK,STEEL,BUSH};
class Wall{
public:
    int x,y;
    WallType type;
    SDL_Rect rect;
    SDL_Texture* spritesheet=nullptr;
    SDL_Rect scrRect;
    bool active;
    Wall(int startX, int startY, WallType Thetype);
    WallType getType();
    void render(SDL_Renderer* renderer);
    void setSpriteSheet(SDL_Texture* sheet,SDL_Rect source);
};

#endif // WALL_H_INCLUDED
