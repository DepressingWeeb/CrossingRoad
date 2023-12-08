#pragma once
#include "road.h"
#include "Character.h"
#include <chrono>
#include <random>
class EndlessLevelGenerator {
private:
	vector<Road*> roadVector; 
	vector<pair<int,int>> roadPosVector;//contains the y-pos of road based on level upper pos y 
	Character* player;
	TTF_Font* font;
	LTexture* scoreTexture;//for render score to screen
	int roadHeight;
	int difficulty;//difficulty,increase by 1 for each dangerous road
	int totalScore;//totalScore=lastLevelScore+currScore
	//number of vehicle each road = sqrt(diff/(SCREEN_WIDTH/roadHeight)+1)*2
	float baseSpeed;//speed of vehicle
	float levelSpeed;//speed of level
	float levelUpperPosY;//levelUpperPosY at start of game =0 and decrease as the level moves. For example,levelUpperPosY after 6 road = -roadHeight*6, after 12 road = -roadHeight*12
	LTimer stepTimer;
	std::mt19937_64 generator;//random engine
	std::uniform_int_distribution<int> distribution;
	void generateFirstLevel();
	void generateNewRoad();
public:
	EndlessLevelGenerator(int difficulty, int roadHeight, Character* player,float baseSpeed=50.f,float levelSpeed=20.f);
	~EndlessLevelGenerator();
	bool Update();
	void Draw();
	void ToFile();
	void FromFile();
};