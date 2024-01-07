#pragma once
#include "road.h"
#include "Character.h"
#include "LevelGenerator.h"
#include <chrono>
#include <random>
class RandomLevelGenerator:public LevelGenerator {
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
	float baseSpeed;//base speed: pixel/s of vehicle when difficulty is 0, actual speed=baseSpeed*(1+0.2*diff)
	//number of vehicle each road = sqrt(diff+1)*2
	std::mt19937_64 generator;//random engine
	std::uniform_int_distribution<int> distribution;
	int terrainID;
	NormalVehicle* cloudEffect;//special effect random for each level
	bool hasCloud;
	void generateNewLevel();
public:
	RandomLevelGenerator(int difficulty,int roadHeight,Character* player,int terrainID=0,float baseSpeed=50.f);
	RandomLevelGenerator(Character* player,std::ifstream& in);
	~RandomLevelGenerator();
	bool Update() override; //return true if update performed,0 if the character is dead
	int getScore() override;
	void Draw() override;
	void ToFile(std::ostream& out) override;
};