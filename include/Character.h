#pragma once
#include "Texture.h"
#include "AnimatingObject.h"
#include "Timer.h"
#include <vector>
class Character {
private:
	int direction;
	vector<vector<LTexture*>> frames;//should not be deleted when destructor calls
	bool isDeath;
	bool isMoving;
	float x;
	float y;
	int width;
	int height;
	float speed;
	float levelSpeed;//in endless mode,the level moving speed
	float* xChange;
	float* yChange;
	LTimer stepTimer;
	int numFrames_;
	int frameDuration_;
	int currentFrame_;
	int frameCounter_;
	SDL_Renderer* renderer;//Also should not be deleted
public:
	Character(SDL_Renderer* renderer,const vector<LTexture*>& textureFrames, int numFrames, int frameDuration, int x, int y, int width=-1, int height=-1, float speed=50,float levelSpeed=0);
	~Character();
	bool checkDangerousCollision(const vector<SDL_Rect>& dangerousObjBoundRectVector);
	void updateDirection();
	void updateAnimation();
	void updateCoordinate(const vector<SDL_Rect>& safeObjBoundRectVector);
	void updateIfDeath();
	//void updateAll();
	void Draw();
	SDL_Rect getBoundingRect();
	pair<float, float> getCoordinate();
	void setCoordinate(float x, float y);
	void setXCoordinate(float x);
	void setLevelSpeed(float newSpeed);
};