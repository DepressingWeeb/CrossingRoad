#include "global.h"

extern const int SCREEN_WIDTH = 1000;
extern const int SCREEN_HEIGHT = 640;
extern const SDL_Rect SCREEN = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
extern SDL_Window* window = SDL_CreateWindow("Crossing Road", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
extern SDL_Renderer* gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);