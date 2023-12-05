#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <iostream>
using namespace std;
class LTexture{
public:
	//Initializes variables
	LTexture(SDL_Renderer* renderer);
	LTexture(SDL_Renderer* renderer,string path);
	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor,TTF_Font* font);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip=NULL,int width=-1,int height=-1,  SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0, SDL_Point* center = NULL);

	//Gets image dimensions
	int getWidth();
	int getHeight();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

protected:
	//The actual hardware texture
	SDL_Texture* mTexture;
	SDL_Renderer* gRenderer;
	//Image dimensions
	int mWidth;
	int mHeight;
};