#include "road.h"
#include "ResourceManager.h"

Road::~Road() {

}

SimpleRoad::SimpleRoad(int nVehicle, int speed, int startY, int endY) {
	// Get the current time in nanoseconds
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();

	// Seed the random number generator with nanoseconds
	std::mt19937_64 generator(nanoseconds);

	// Define the range for the random number
	int minNumber = 0;
	int maxNumber = INT_MAX;

	// Define the distribution and generate the random number
	std::uniform_int_distribution<int> distribution(minNumber, maxNumber);


	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::SimpleRoad)[0];
	this->nVehicle = nVehicle;
	this->speed = speed;
	this->startY = startY;
	this->endY = endY;
	//Two offset variable for rendering to the correct lane
	int offsetY1 = (endY-startY)/5;//the upper lane
	int offsetY2 = (endY-startY)/2+ (endY - startY) / 12;//The lower lane
	//Get the resourceManager instance
	
	vector<SDL_Rect> occupiedPixels;

	const float scalingFactor = 0.25;	// Default scaling factor of vehicle resources
	//The two loop below randomize the vehicle for each lane and check for overlapping vehicle
	for (int i = 0; i < nVehicle/2; i++) {
		int randomInt = distribution(generator);
		ResourceType randomVehicle = vehicleResources[(randomInt % vehicleResources.size())];
		vector<LTexture*> vehicleTexture = resourceManager.GetTexture(randomVehicle);
		SDL_Rect vehicleOccupyPixels;
		while (true) {
			vehicleOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY1,static_cast<int>(vehicleTexture[0]->getWidth() * scalingFactor),static_cast<int>(vehicleTexture[0]->getHeight() * scalingFactor)};
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &vehicleOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
			
		}
		AnimatingObject* newVehicle = new NormalVehicle(gRenderer, vehicleTexture, vehicleTexture.size(), 10, vehicleOccupyPixels.x, startY + offsetY1, -1, -1, 1, scalingFactor);
		roadObj.push_back(make_pair(newVehicle,0));
		occupiedPixels.push_back(vehicleOccupyPixels);
	}
	for (int i = 0; i < nVehicle / 2; i++) {
		int randomInt = distribution(generator);
		ResourceType randomVehicle = vehicleResources[(randomInt % vehicleResources.size())];
		vector<LTexture*> vehicleTexture = resourceManager.GetTexture(randomVehicle);
		SDL_Rect vehicleOccupyPixels;
		while (true) {
			vehicleOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY2,static_cast<int>(vehicleTexture[0]->getWidth() * scalingFactor),static_cast<int>(vehicleTexture[0]->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &vehicleOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		roadObj.push_back(make_pair(new NormalVehicle(gRenderer, vehicleTexture, vehicleTexture.size(), 10, vehicleOccupyPixels.x, startY + offsetY2, -1, -1, -1, 0.25),1));
		occupiedPixels.push_back(vehicleOccupyPixels);
	}
	
}

void SimpleRoad::Update() {
	for (pair<AnimatingObject*,int> obj : roadObj) {
		obj.first->Update();
	}
}
void SimpleRoad::Draw() {
	roadTexture->render(0, startY, NULL, SCREEN_WIDTH, endY - startY);
	for (auto obj : roadObj) {
		obj.first->Draw();
	}
}
int SimpleRoad::getRoadID() {
	//the id in enum class
	return static_cast<int>(RoadType::SimpleRoad);
}

void SimpleRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	//In case there are changes in startY and endY, due to the level moving in endless mode,then update each vehicle's Y coordinate
	int offsetY1 = (endY - startY) / 5;//the upper lane
	int offsetY2 = (endY - startY) / 2 + (endY - startY) / 12;//The lower lane
	for (pair<AnimatingObject*, int> obj : roadObj) {
		if (obj.second == 0) {
			obj.first->setYCoordinate(startY + offsetY1);
		}
		else {
			obj.first->setYCoordinate(startY + offsetY2);
		}
	}
}

vector<SDL_Rect> SimpleRoad::getDangerousRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	for (pair<AnimatingObject*,int> obj : roadObj) {
		ans.push_back(obj.first->boundingRect());
	}
	return ans;
}

vector<SDL_Rect> SimpleRoad::getSafeRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

MonsterRoad::MonsterRoad(int nMonster, int speed, int startY, int endY) : SimpleRoad(nMonster, speed, startY, endY) {
	// Two offset variables for rendering to the correct lane
	int offsetY1 = (endY - startY) / 5;      // The upper lane
	int offsetY2 = (endY - startY) / 2 + (endY - startY) / 12;  // The lower lane

	// Get the resource manager instance
	ResourceManager& resourceManager = ResourceManager::GetInstance();

	const float scalingFactor = 0.25;  // Default scaling factor of vehicle resources

	for (int i = 0; i < nMonster / 2; i++) {
		// Use vehicle resources for testing
		ResourceType randomVehicle = vehicleResources[rand() % vehicleResources.size()];
		vector<LTexture*> vehicleTexture = resourceManager.GetTexture(randomVehicle);
		SDL_Rect monsterOccupyPixels;

		// Create Monster object and add it to the road
		AnimatingObject* newMonster = new Monster(gRenderer, vehicleTexture, vehicleTexture.size(), 10, rand() % SCREEN_WIDTH, startY + offsetY1, -1, -1, 1, scalingFactor);
		roadObj.push_back(make_pair(newMonster, 0));
	}

}

EnhancedRoad::EnhancedRoad(int nVehicle, int speed, int startY, int endY)
	: SimpleRoad(nVehicle, speed, startY, endY) {
	arrowSpawnTimer.start(); // Start the arrow spawn timer
}

void EnhancedRoad::Update() {
	// Call the base class update to handle vehicle updates
	SimpleRoad::Update();

	// Update the arrow spawn timer
	if (arrowSpawnTimer.getTicks() >= 2000) { // 2000 milliseconds = 2 seconds
		// Get the resource manager instance
		ResourceManager& resourceManager = ResourceManager::GetInstance();
 
		// Spawn a new arrow using vehicle resources for testing
		ResourceType randomVehicle = vehicleResources[rand() % vehicleResources.size()];
		vector<LTexture*> vehicleTexture = resourceManager.GetTexture(randomVehicle);

		// Calculate the middle of the road
		int middleX = (SCREEN_WIDTH - vehicleTexture[0]->getWidth()) / 2;

		SDL_Rect arrowOccupyPixels;
		arrowOccupyPixels = { middleX, startY, static_cast<int>(vehicleTexture[0]->getWidth() * 0.25), static_cast<int>(vehicleTexture[0]->getHeight() * 0.25) };

		AnimatingObject* newArrow = new Arrow(gRenderer, vehicleTexture, vehicleTexture.size(), 10, arrowOccupyPixels.x, arrowOccupyPixels.y, -1, -1, 1, 0.25);
		roadObj.push_back(make_pair(newArrow, 0));

		// Reset the arrow spawn timer
		arrowSpawnTimer.start();
	}
}

SimpleSafeRoad::SimpleSafeRoad(int startY, int endY) {
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::SimpleSafeRoad)[0];
	this->startY = startY;
	this->endY = endY;
}

void SimpleSafeRoad::Update() {
	return;
}

void SimpleSafeRoad::Draw() {
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
}

int SimpleSafeRoad::getRoadID() {
	return static_cast<int>(RoadType::SimpleSafeRoad);
}

void SimpleSafeRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
}

vector<SDL_Rect> SimpleSafeRoad::getDangerousRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

vector<SDL_Rect> SimpleSafeRoad::getSafeRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

Railway::Railway(int speed, float timeTrafficLightRed, float timeTrafficLightGreen, int startY, int endY) {
	this->speed = speed;
	this->timeTrafficLightGreen = timeTrafficLightGreen;
	this->timeTrafficLightRed = timeTrafficLightRed;
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::Railway)[0];
	this->trafficLightRed = resourceManager.GetTexture(ResourceType::TrafficLight)[0];
	this->trafficLightGreen = resourceManager.GetTexture(ResourceType::TrafficLight)[1];
	this->startY = startY;
	this->endY = endY;
	this->train = new Train(gRenderer,startY,endY-startY, speed);
	this->isRedLight = true;
	train->setRedLight(isRedLight);
	// Get the current time in nanoseconds
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();

	// Seed the random number generator with nanoseconds
	std::mt19937_64 generator(nanoseconds);

	// Define the range for the random number
	int minNumber = 0;
	int maxNumber = INT_MAX;

	// Define the distribution and generate the random number
	std::uniform_int_distribution<int> distribution(minNumber, maxNumber);
	this->timeSinceLastLightChange = (static_cast<float>(distribution(generator))/static_cast<float>(maxNumber))*4.f;
}
int Railway::getRoadID() {
	return static_cast<int>(RoadType::Railway);
}

vector<SDL_Rect> Railway::getDangerousRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	ans.push_back(train->boundingRect());
	return ans;
}

vector<SDL_Rect> Railway::getSafeRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	ans.push_back({ 10, endY - trafficLightRed->getHeight(), trafficLightRed->getWidth(), trafficLightRed->getHeight() });
	return ans;
}

void Railway::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	train->setYCoordinate(startY);
}

void Railway::Update() {
	float timeStep = stepTimer.getTicks() / 1000.f;
	stepTimer.start();
	timeSinceLastLightChange += timeStep;
	if (isRedLight && timeSinceLastLightChange>timeTrafficLightRed) {
		timeSinceLastLightChange =0;
		isRedLight = false;
		train->setRedLight(isRedLight);
	}
	else if(!isRedLight && timeSinceLastLightChange > timeTrafficLightGreen) {
		timeSinceLastLightChange =0;
		isRedLight = true;
		train->setRedLight(isRedLight);
	}
	train->Update();
}

void Railway::Draw() {
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
	train->Draw();
	if (isRedLight)
		trafficLightRed->render(10, endY-trafficLightRed->getHeight(), NULL, trafficLightRed->getWidth(), trafficLightRed->getHeight());
	else
		trafficLightGreen->render(10, endY-trafficLightGreen->getHeight(), NULL, trafficLightGreen->getWidth(), trafficLightGreen->getHeight());

}


River::River(int startY, int endY) {
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->bg = resourceManager.GetTexture(ResourceType::SimpleSafeRoad)[0];
	this->bridge = resourceManager.GetTexture(ResourceType::Bridge)[0];
	this->startY = startY;
	this->endY = endY;
	// Get the current time in nanoseconds
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();

	// Seed the random number generator with nanoseconds
	std::mt19937_64 generator(nanoseconds);

	// Define the range for the random number
	int minNumber = 0;
	int maxNumber = INT_MAX;

	// Define the distribution and generate the random number
	std::uniform_int_distribution<int> distribution(minNumber, maxNumber);
	this->bridgeW = (endY - startY) * bridgeWidthHeightRatio;
	this->bridgeX = distribution(generator) % (SCREEN_WIDTH-bridgeW);
	int tileHeight = (endY - startY) / 2;
	int tileWidth = tileHeight;
	int nPairTile = ceil(SCREEN_WIDTH / static_cast<float>(tileWidth));
	for (int i = 0; i < nPairTile; i++) {
		StaticAnimatingObject* upperWaterTile = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::UpperWaterLane), 14, 4, i * tileWidth, startY, tileWidth, tileHeight);
		StaticAnimatingObject* lowerWaterTile = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::LowerWaterLane), 14, 4, i * tileWidth, startY+tileHeight, tileWidth, tileHeight);
		this->waters.push_back(make_pair(upperWaterTile, lowerWaterTile));
	}
}

void River::Update() {
	for (int i = 0; i < waters.size(); i++) {
		waters[i].first->Update();
		waters[i].second->Update();
	}
}

vector<SDL_Rect> River::getSafeRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

vector<SDL_Rect> River::getDangerousRoadObjBoundRect() {
	SDL_Rect rect1 = { 0,startY,bridgeX,endY - startY };
	SDL_Rect rect2 = { bridgeX + bridgeW,startY,SCREEN_WIDTH,endY - startY };
	vector<SDL_Rect> ans = { rect1,rect2 };
	return ans;
}

void River::Draw() {
	bg->render(0, startY, NULL,SCREEN_WIDTH, endY - startY);
	for (auto p : waters) {
		p.first->Draw();
		p.second->Draw();
	}
	bridge->render(bridgeX, startY, NULL, bridgeW, endY - startY);
}

void River::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	for (auto p : waters) {
		p.first->setYCoordinate(newStartY);
		p.second->setYCoordinate(newStartY+ (endY - startY) / 2);
	}
}

int River::getRoadID() {
	return static_cast<int>(RoadType::River);
}

RollingStoneRoad::RollingStoneRoad(int nStone, int speed, int startY, int endY) {
	//Get resource manager instance
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::RollingStoneRoad)[0];
	this->nStone = nStone;
	this->speed = speed;
	this->startY = startY;
	this->endY = endY;

	//Two offset variable 
	int offsetY1 = 0;//the upper lane
	int offsetY2 = (endY-startY)/3+ (endY - startY) / 12;//The lower lane

	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();

	std::mt19937_64 generator(nanoseconds);

	int minNumber = 0;
	int maxNumber = INT_MAX;

	std::uniform_int_distribution<int> distribution(minNumber, maxNumber);

	const float scalingFactor = 0.25;
	vector<SDL_Rect> occupiedPixels;

	for (int i = 0; i < nStone/2; i++) {
		vector<LTexture*> stoneTexture = resourceManager.GetTexture(ResourceType::Stone);
		SDL_Rect stoneOccupyPixels;
		while (true) {
			stoneOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY1,static_cast<int>(stoneTexture[0]->getWidth() * scalingFactor),static_cast<int>(stoneTexture[0]->getHeight() * scalingFactor)};
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &stoneOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
			
		}
		AnimatingObject* newStone = new NormalVehicle(gRenderer, stoneTexture, stoneTexture.size(), 10, stoneOccupyPixels.x, startY + offsetY1, -1, -1, 1, scalingFactor);
		roadObj.push_back(make_pair(newStone,0));
		occupiedPixels.push_back(stoneOccupyPixels);
	}

	for (int i = 0; i < nStone / 2; i++) {
		vector<LTexture*> stoneTexture = resourceManager.GetTexture(ResourceType::Stone);
		SDL_Rect stoneOccupyPixels;
		while (true) {
			stoneOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY2,static_cast<int>(stoneTexture[0]->getWidth() * scalingFactor),static_cast<int>(stoneTexture[0]->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &stoneOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		roadObj.push_back(make_pair(new NormalVehicle(gRenderer, stoneTexture, stoneTexture.size(), 10, stoneOccupyPixels.x, startY + offsetY2, -1, -1, -1, 0.25),1));
		occupiedPixels.push_back(stoneOccupyPixels);
	}
}

void RollingStoneRoad::Update() {
	for (pair<AnimatingObject*,int> obj : roadObj) {
		obj.first->Update();
	}
}
void RollingStoneRoad::Draw() {
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
	for (auto obj : roadObj) {
		obj.first->Draw();
	}
}
int RollingStoneRoad::getRoadID() {
	return static_cast<int>(RoadType::RollingStoneRoad);
}

void RollingStoneRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	//In case there are changes in startY and endY, due to the level moving in endless mode,then update each vehicle's Y coordinate
	int offsetY1 = (endY - startY) / 5;//the upper lane
	int offsetY2 = (endY - startY) / 2 + (endY - startY) / 12;//The lower lane
	for (pair<AnimatingObject*, int> obj : roadObj) {
		if (obj.second == 0) {
			obj.first->setYCoordinate(startY + offsetY1);
		}
		else {
			obj.first->setYCoordinate(startY + offsetY2);
		}
	}
}

vector<SDL_Rect> RollingStoneRoad::getDangerousRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	for (pair<AnimatingObject*,int> obj : roadObj) {
		ans.push_back(obj.first->boundingRect());
	}
	return ans;
}

vector<SDL_Rect> RollingStoneRoad::getSafeRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

ForestRiver::ForestRiver(int n, int speed, int startY, int endY) {
	//Get resource manager instance
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::SimpleSafeRoad)[0];
	this->n = n;
	this->speed = speed;
	this->startY = startY;
	this->endY = endY;

	int offsetY = (endY-startY)/2;//The lower lane
	
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();

	std::mt19937_64 generator(nanoseconds);

	int minNumber = 0;
	int maxNumber = INT_MAX;

	std::uniform_int_distribution<int> distribution(minNumber, maxNumber);

	const float scalingFactor = 0.5;
	vector<SDL_Rect> occupiedPixels;

	for (int i = 0; i < n/2; i++) {
		vector<LTexture*> timberTexture = resourceManager.GetTexture(ResourceType::Timber);
		SDL_Rect timberOccupyPixels;
		while (true) {
			timberOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY,static_cast<int>(timberTexture[0]->getWidth() * scalingFactor),static_cast<int>(timberTexture[0]->getHeight() * scalingFactor)};
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &timberOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
			
		}
		AnimatingObject* newTimber = new NormalVehicle(gRenderer, timberTexture, timberTexture.size(), 10, timberOccupyPixels.x, startY, -1, -1, 1, scalingFactor);
		roadObj.push_back(make_pair(newTimber,0));
		occupiedPixels.push_back(timberOccupyPixels);
	}

	for (int i = 0; i < n/2; i++) {
		vector<LTexture*> timberTexture = resourceManager.GetTexture(ResourceType::Timber);
		SDL_Rect timberOccupyPixels;
		while (true) {
			timberOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY,static_cast<int>(timberTexture[0]->getWidth() * scalingFactor),static_cast<int>(timberTexture[0]->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &timberOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		roadObj.push_back(make_pair(new NormalVehicle(gRenderer, timberTexture, timberTexture.size(), 10, timberOccupyPixels.x, startY + offsetY, -1, -1, -1, scalingFactor),1));
		occupiedPixels.push_back(timberOccupyPixels);
	}

	int tileHeight = (endY - startY) / 2;
	int tileWidth = tileHeight;
	int nPairTile = ceil(SCREEN_WIDTH / static_cast<float>(tileWidth));
	for (int i = 0; i < nPairTile; i++) {
		StaticAnimatingObject* upperWaterTile = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::UpperWaterLane), 14, 4, i * tileWidth, startY, tileWidth, tileHeight);
		StaticAnimatingObject* lowerWaterTile = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::LowerWaterLane), 14, 4, i * tileWidth, startY+tileHeight, tileWidth, tileHeight);
		this->waters.push_back(make_pair(upperWaterTile, lowerWaterTile));
	}
}

int ForestRiver::getRoadID() {
	return static_cast<int>(RoadType::ForestRiver);
}

void ForestRiver::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	//In case there are changes in startY and endY, due to the level moving in endless mode,then update each vehicle's Y coordinate
	int offsetY1 = 0;//the upper lane
	int offsetY2 = (endY - startY) / 2;//The lower lane
	for (pair<AnimatingObject*, int> obj : roadObj) {
		if (obj.second == 0) {
			obj.first->setYCoordinate(startY + offsetY1);
		}
		else {
			obj.first->setYCoordinate(startY + offsetY2);
		}
	}
}

vector<SDL_Rect> ForestRiver::getDangerousRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	int index = 0;
	for (auto it = roadObj.begin(); it != roadObj.end(); ++it) { 
		SDL_Rect rect;
		pair<AnimatingObject*,int> obj = *it;
		if (index < n/2) {
			if (index == 0) {
				rect = {0, startY, obj.first->boundingRect().x, (endY - startY)/2};
				ans.push_back(rect);
			}
			else {
				--it;
				pair<AnimatingObject*, int>& prevObj = *it;
				it++;
				rect = {prevObj.first->boundingRect().x + prevObj.first->boundingRect().w, startY, obj.first->boundingRect().x - prevObj.first->boundingRect().x - prevObj.first->boundingRect().w, (endY - startY)/2};
				ans.push_back(rect);
			}
			if (index == n/2 - 1) {
				rect = {obj.first->boundingRect().x + obj.first->boundingRect().w, startY, SCREEN_WIDTH - obj.first->boundingRect().x - obj.first->boundingRect().w, (endY - startY)/2};
				ans.push_back(rect);
			}
			index += 1;
		}
		else {
			if (index == n/2) {
				rect = {0, startY + (endY - startY)/2, obj.first->boundingRect().x, (endY - startY)/2};
				ans.push_back(rect);
			}
			else {
				--it;
				pair<AnimatingObject*, int>& prevObj = *it;
				it++;
				rect = {prevObj.first->boundingRect().x + prevObj.first->boundingRect().w, startY + (endY - startY)/2, obj.first->boundingRect().x - prevObj.first->boundingRect().x - prevObj.first->boundingRect().w, (endY - startY)/2};
				ans.push_back(rect);
			}
			if (index == n - 1) {
				rect = {obj.first->boundingRect().x + obj.first->boundingRect().w, startY+(endY - startY)/2, SCREEN_WIDTH - obj.first->boundingRect().x - obj.first->boundingRect().w, (endY - startY)/2};
				ans.push_back(rect);
			}
			index += 1;
		}

	}
	return ans;
}

vector<SDL_Rect> ForestRiver::getSafeRoadObjBoundRect() {
	return vector<SDL_Rect>();
	/*
	vector<SDL_Rect> ans;
	for (pair<AnimatingObject*,int> obj : roadObj) {
		ans.push_back(obj.first->boundingRect());
	}
	return ans;
	*/
}

void ForestRiver::Update() {
	for (pair<AnimatingObject*,int> obj : roadObj) {
		obj.first->Update();
	}
	for (int i = 0; i < waters.size(); i++) {
		waters[i].first->Update();
		waters[i].second->Update();
	}
}
void ForestRiver::Draw() {
	roadTexture->render(0, startY, NULL,SCREEN_WIDTH, endY - startY);
	for (auto p : waters) {
		p.first->Draw();
		p.second->Draw();
	}
	for (auto obj : roadObj) {
		obj.first->Draw();
	}
}
