#pragma once
#include "Texture.h"
#include "AnimatingObject.h"
#include "Timer.h"
#include <vector>
class Character {
private:
	int direction;
	vector<vector<LTexture*>> frames;
	bool isDeath;
	bool isMoving;
	float x;
	float y;
	int width;
	int height;
	float speed;
	float* xChange;
	float* yChange;
	LTimer stepTimer;
	int numFrames_;
	int frameDuration_;
	int currentFrame_;
	int frameCounter_;
	SDL_Renderer* renderer;
public:
	Character(SDL_Renderer* renderer,const vector<vector<LTexture*>>& textureFrames, int numFrames, int frameDuration, int x, int y, int width=-1, int height=-1, float speed=50);
	void checkCollision(const vector<AnimatingObject*> v);
	void updateDirection();
	void updateCoordinate();
	void updateIfDeath();
	void updateAll();
	void Draw();
	SDL_Rect getBoundingRect();
	void setCoordinate(int x, int y);
};