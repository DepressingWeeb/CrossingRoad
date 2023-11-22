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
	int difficulty;
public:
	RandomLevelGenerator(int difficulty,int roadHeight,Character* player);
	void generateNewLevel();
	void Update();
	void Draw();
};