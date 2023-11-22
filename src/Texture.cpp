#include "Texture.h"
LTexture::LTexture(SDL_Renderer* renderer)
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	gRenderer = renderer;
}
LTexture::LTexture(SDL_Renderer* renderer,string path) {
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	gRenderer = renderer;
	loadFromFile(path);
	
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(string path)
{
	//Get rid of preexisting texture;
	free();
	//The final texture
	SDL_Texture* newTexture = NULL;
	newTexture = IMG_LoadTexture(gRenderer, path.c_str());
	if (newTexture == NULL) cout << "Fail to load texture" << endl;
	mTexture = newTexture;
	SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip, int width , int height , SDL_RendererFlip flip, double angle, SDL_Point* center)
{
	SDL_Rect renderQuad;
	//Set rendering space and render to screen
	if (width == -1)	renderQuad = { x, y, mWidth, mHeight };
	else renderQuad = { x,y,width,height };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}
int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}