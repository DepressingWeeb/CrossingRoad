#pragma once
#include <vector>
#include <chrono>
#include <random>
#include "SDL.h"
#include "Texture.h"
#include "global.h"
#include "Vehicle.h"
enum class RoadType {
	SimpleSafeRoad=0,
	SimpleRoad,
	
	Last //Last element for the purpose of randomization
};
class Road {
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual int getRoadID() = 0;//Get the id from enum
	virtual vector<AnimatingObject*> getRoadObj() = 0;
	virtual ~Road()=0;
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
	int getRoadID() override;
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
	int getRoadID() override;
	vector<AnimatingObject*> getRoadObj() override;
};

class RoadWithTrafficLight:public Road{};//TODO

class Railway:public Road{};//TODO