#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <algorithm>
#include <SDL_mixer.h>
#include <string>
enum Direction {UP, DOWN, LEFT, RIGHT};
const int SCREEN_HEIGHT=600;
const int SCREEN_WIDTH=800;
const int TITLE_SIZE=40;
const int MAP_WIDTH=SCREEN_WIDTH/TITLE_SIZE;
const int MAP_HEIGHT=SCREEN_HEIGHT/TITLE_SIZE;


#endif // CONSTANTS_H_INCLUDED
