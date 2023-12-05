#pragma once
#include "road.h"
#include "Character.h"
#include <chrono>
#include <random>
class RandomLevelGenerator {
private:
	vector<Road*> roadVector;
	Character* player;
	int roadHeight;
	int level;
	int difficulty;//difficulty,increase by 1 for each level passed
	const float baseSpeed = 50.f; //base speed: pixel/s of vehicle when difficulty is 0, actual speed=baseSpeed*(1+0.2*diff)
	//number of vehicle each road = sqrt(diff+1)*2
public:
	RandomLevelGenerator(int difficulty,int roadHeight,Character* player);
	void generateNewLevel();
	void Update();
	void Draw();
};