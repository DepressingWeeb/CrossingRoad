#pragma once
#include <vector>
#include <chrono>
#include <random>
#include "SDL.h"
#include "Texture.h"
#include "global.h"
#include "Vehicle.h"
#include "StaticAnimatingObject.h"
#include "Monster.h"
#include "Animal.h"
enum class CityRoadType {
	SimpleSafeRoad=0,
	SimpleRoad,
	Railway,
	River,
	RollingStoneRoad,
	ForestRiver,
	Last //Last element for the purpose of randomization
};

enum class ForestRoadType
{
	SafeForestRoad = 0,
	AnimalRoad,
	TreeRoad,
	MonsterRoad,
	RollingStoneRoad,
	ForestRiver,
	Last,
};
class Road {
public:
	
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void setStartEndPosRoad(int newStartY, int newEndY)=0;//for endless mode because the level is constantly moving
	virtual int getRoadID() = 0;//Get the id from enum
	virtual vector<SDL_Rect> getDangerousRoadObjBoundRect() = 0;//Return the "dangerous" roadObj's current bounding rect,like a vehicle
	virtual vector<SDL_Rect> getSafeRoadObjBoundRect()=0;//Return the "safe" roadObj's current bounding rect,like a tree or some obstacles.
	virtual ~Road()=0;
};


class SimpleRoad:public Road {
protected:
	LTexture* roadTexture;
	int nVehicle;
	int speed;
	int startY;
	int endY;
	vector<pair<AnimatingObject*,int>> roadObj;//A vector contains the pointer to the vehicles and an int indicates which lane the vehicles belong (0:upper lane,1:lower lane)
public:
	SimpleRoad(int nVehicle, int speed,int startY,int endY);
	void Update() override;
	void Draw() override;
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
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
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
};

class River:public Road{
private:
	LTexture* bg;
	LTexture* bridge;
	int startY;
	int endY;
	int bridgeX;
	int bridgeW;
	const float bridgeWidthHeightRatio = 0.81f;
	vector<pair<StaticAnimatingObject*,StaticAnimatingObject*>> waters; //pair.first is texture for upper water lane, pair.second is for lower water lane
public:
	River( int startY, int endY);
	void Update() override;
	void Draw() override;
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
};//TODOCITY

class Railway:public Road{
	LTexture* roadTexture;
	LTexture* trafficLightRed;
	LTexture* trafficLightGreen;
	LTimer stepTimer;
	Train* train;
	int speed;
	bool isRedLight;
	float timeTrafficLightRed;
	float timeTrafficLightGreen;
	float timeSinceLastLightChange;
	int startY;
	int endY;
public:
	Railway(int speed,float timeTrafficLightRed,float timeTrafficLightGreen,int startY, int endY);
	void Update() override;
	void Draw() override;
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
	
};

//TODOFOREST: create new road type for forest terrain, have to inherit from class Road
class SafeForestRoad :public Road {
private:
	LTexture* roadTexture;
	LTexture* flower;
	int startY;
	int endY;
	vector<StaticAnimatingObject*>flowerObj;
public:
	SafeForestRoad(int startY, int endY);
	void Update() override;
	void Draw() override;
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
};
//=======================AnimalRoad=======================
class AnimalRoad :public Road {
protected:
	LTexture* roadTexture;
	int nAnimal;
	int speed;
	int startY;
	int endY;
	vector<pair<AnimatingObject*, int>> roadObj;//A vector contains the pointer to the animals and an int indicates which lane the animals belong (0:upper lane,1:lower lane)
public:
	AnimalRoad(int nAnimal, int speed, int startY, int endY);
	void Update() override;
	void Draw() override;
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
};
//=============================TreeRoad==========================
class TreeRoad :public Road {
private:

	LTexture* bg;
	int startY;
	int endY;
	vector<StaticAnimatingObject*>treeObj;
public:
	TreeRoad(int startY, int endY);
	void Update() override;
	void Draw() override;
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
};
class MonsterRoad : public Road {
private:
	LTexture* roadTexture;
	int startY;
	int endY;
	float timeIdle;
	float timeArrow;
	int arrowSpeed;
	Monster* archer;
	vector<StaticAnimatingObject*> decoratorObj;
public:
	MonsterRoad(float timeIdle, float timeArrow, int arrowSpeed, int startY, int endY);
	void Update() override;
	void Draw() override;
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
};

class ForestRiver : public Road {
protected:
	LTexture* roadTexture;
	int n;
	int speed;
	int startY;
	int endY;
	vector<pair<AnimatingObject*, int>> roadObj;
	vector<pair<StaticAnimatingObject*, StaticAnimatingObject*>> waters;
public:
	ForestRiver(int n, int speed, int startY, int endY);
	void Update() override;
	void Draw() override;
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
};

class RollingStoneRoad : public Road {
protected:
	LTexture* roadTexture;
	int nStone;
	int speed;
	int startY;
	int endY;
	vector<pair<AnimatingObject*, int>> roadObj;
public:
	RollingStoneRoad(int nStone, int speed, int startY, int endY);
	void Update() override;
	void Draw() override;
	void setStartEndPosRoad(int newStartY, int newEndY) override;
	int getRoadID() override;
	vector<SDL_Rect> getDangerousRoadObjBoundRect() override;
	vector<SDL_Rect> getSafeRoadObjBoundRect() override;
};

