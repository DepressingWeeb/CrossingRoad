#pragma once
#include "Texture.h"
#include "AnimatingObject.h"
#include <vector>
class Character {
private:
	int direction;
	vector<vector<LTexture*>> frames;
	bool isDeath;
	bool isMoving;
	int x;
	int y;
	int width;
	int height;
	int speed;
	int* xChange;
	int* yChange;
	int numFrames_;
	int frameDuration_;
	int currentFrame_;
	int frameCounter_;
	SDL_Renderer* renderer;
public:
	Character(SDL_Renderer* renderer, vector<vector<LTexture*>>& textureFrames, int numFrames, int frameDuration, int x, int y, int width, int height, int speed);
	void checkCollision(const vector<AnimatingObject>& v);
	void updateDirection();
	void updateCoordinate();
	void updateIfDeath();
	void updateAll();
	void Draw();

};