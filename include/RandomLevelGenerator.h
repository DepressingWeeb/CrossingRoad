#pragma once
#include "road.h"
#include "Character.h"
#include <chrono>
#include <random>
class RandomLevelGenerator {
private:
	vector<Road*> roadVector;
	Character* player;
	TTF_Font* font;
	int roadHeight;
	int level;
	int difficulty;//difficulty,increase by 1 for each level passed
	int currScore;//score for the player in the current level, increase by 1 for each "dangerous" road passed
	int lastLevelScore;//total score till the last level
	LTexture* scoreTexture;//for render score to screen
	int totalScore;//totalScore=lastLevelScore+currScore
	const float baseSpeed = 50.f; //base speed: pixel/s of vehicle when difficulty is 0, actual speed=baseSpeed*(1+0.2*diff)
	//number of vehicle each road = sqrt(diff+1)*2
public:
	RandomLevelGenerator(int difficulty,int roadHeight,Character* player);
	void generateNewLevel();
	void Update();
	void Draw();
	void ToFile();
	void FromFile();
};