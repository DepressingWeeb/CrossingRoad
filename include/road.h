#pragma once
#include <vector>
#include <chrono>
#include <random>
#include "SDL.h"
#include "Texture.h"
#include "global.h"
#include "AnimatingObject.h"
enum class RoadType {
	SimpleRoad,
	SimpleSafeRoad,
	Last //Last element for the purpose of randomization
};
class Road {
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual vector<AnimatingObject*> getRoadObj() = 0;
};


class SimpleRoad:public Road {
private:
	LTexture* roadTexture;
	int nVehicle;
	int speed;
	int startY;
	int endY;
	vector<AnimatingObject*> roadObj;
public:
	SimpleRoad( int nVehicle, int speed,int startY,int endY);
	void Update() override;
	void Draw() override;
	vector<AnimatingObject*> getRoadObj() override;
};

class SimpleSafeRoad :public Road {
private:
	LTexture* roadTexture;
	int startY;
	int endY;
public:
	SimpleSafeRoad(int startY, int endY);
	void Update() override;
	void Draw() override;
	vector<AnimatingObject*> getRoadObj() override;
};