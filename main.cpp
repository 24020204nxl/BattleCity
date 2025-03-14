#include<iostream>
#include<SDL.h>
using namespace std;
int main(int argc, char* argv[])
{
 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
 SDL_RenderDrawPoint(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
 SDL_RenderDrawLine(renderer, 100, 100, 200, 200);
 SDL_Rect filled_rect;
 filled_rect.x = SCREEN_WIDTH - 400;
 filled_rect.y = SCREEN_HEIGHT - 150;
 filled_rect.w = 320;
 filled_rect.h = 100;
 SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
 SDL_RenderFillRect(renderer, &filled_rect);
 //Khi thông thường chạy với môi trường bình thường ở nhà
 SDL_RenderPresent(renderer);
 //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
 //SDL_UpdateWindowSurface(window);
    return 0;
}
