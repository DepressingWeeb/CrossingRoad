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
		AnimatingObject* newVehicle = new NormalVehicle(gRenderer, vehicleTexture, vehicleTexture.size(), 10, vehicleOccupyPixels.x, startY + offsetY1, -1, -1, -speed, scalingFactor);
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
		roadObj.push_back(make_pair(new NormalVehicle(gRenderer, vehicleTexture, vehicleTexture.size(), 10, vehicleOccupyPixels.x, startY + offsetY2, -1, -1, speed, 0.25),1));
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
//==============================================SafeForestRoad================================
SafeForestRoad::SafeForestRoad(int startY, int endY) {
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::SafeForestRoad)[1];
	this->startY = startY;
	this->endY = endY;
}

void SafeForestRoad::Update() {
	return;
}

void SafeForestRoad::Draw() {
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
}

int SafeForestRoad::getRoadID() {
	return static_cast<int>(ForestRoadType::SafeForestRoad);
}

void SafeForestRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
}

vector<SDL_Rect> SafeForestRoad::getDangerousRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

vector<SDL_Rect> SafeForestRoad::getSafeRoadObjBoundRect() {
	return vector<SDL_Rect>();
}
//=================================AnimalRoad==========================================
AnimalRoad::AnimalRoad(int nAnimal, int speed, int startY, int endY) {
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
	this->roadTexture = resourceManager.GetTexture(ResourceType::AnimalRoad)[4];
	this->nAnimal = nAnimal;
	this->speed = speed;
	this->startY = startY;
	this->endY = endY;
	//Two offset variable for rendering to the correct lane
	int offsetY1 = (endY - startY) / 8;//the upper lane
	int offsetY2 = (endY - startY) / 2 - (endY - startY) / 12;//The lower lane
	//Get the resourceManager instance
	vector<SDL_Rect> occupiedPixels;

	const float scalingFactor = 2;	// Default scaling factor of animal resources
	//The two loop below randomize the animal for each lane and check for overlapping animal
	for (int i = 0; i < nAnimal / 2; i++) {
		int randomInt = distribution(generator);
		ResourceType randomAnimal = animalResources[(randomInt % animalResources.size())];
		vector<LTexture*> animalTexture = resourceManager.GetTexture(randomAnimal);
		SDL_Rect animalOccupyPixels;
		while (true) {
			animalOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY1,static_cast<int>(animalTexture[0]->getWidth() * scalingFactor),static_cast<int>(animalTexture[0]->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &animalOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;

		}
		AnimatingObject* newAnimal = new NormalAnimal(gRenderer, animalTexture, animalTexture.size(), 10, animalOccupyPixels.x, startY + offsetY1, -1, -1, speed, scalingFactor);
		roadObj.push_back(make_pair(newAnimal, 0));
		occupiedPixels.push_back(animalOccupyPixels);
	}
	for (int i = 0; i < nAnimal / 2; i++) {
		int randomInt = distribution(generator);
		ResourceType randomAnimal = animalResources[(randomInt % animalResources.size())];
		vector<LTexture*> animalTexture = resourceManager.GetTexture(randomAnimal);
		SDL_Rect animalOccupyPixels;
		while (true) {
			animalOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY2,static_cast<int>(animalTexture[0]->getWidth() * scalingFactor),static_cast<int>(animalTexture[0]->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &animalOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		roadObj.push_back(make_pair(new NormalAnimal(gRenderer, animalTexture, animalTexture.size(), 10, animalOccupyPixels.x, startY + offsetY2, -1, -1, -speed, 2), 1));
		occupiedPixels.push_back(animalOccupyPixels);
	}

}

void AnimalRoad::Update() {
	for (pair<AnimatingObject*, int> obj : roadObj) {
		obj.first->Update();
	}
}
void AnimalRoad::Draw() {
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
	for (auto obj : roadObj) {
		obj.first->Draw();
	}
}
int AnimalRoad::getRoadID() {
	//the id in enum class
	return static_cast<int>(ForestRoadType::AnimalRoad);
}

void AnimalRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	//In case there are changes in startY and endY, due to the level moving in endless mode,then update each animal's Y coordinate
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

vector<SDL_Rect> AnimalRoad::getDangerousRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	for (pair<AnimatingObject*, int> obj : roadObj) {
		ans.push_back(obj.first->boundingRect());
	}
	return ans;
}

vector<SDL_Rect> AnimalRoad::getSafeRoadObjBoundRect() {
	return vector<SDL_Rect>();
}
//==============================Tree Road==============================
TreeRoad::TreeRoad(int startY, int endY) {
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->bg = resourceManager.GetTexture(ResourceType::SafeForestRoad)[1];
	this->tree = resourceManager.GetTexture(ResourceType::Tree)[0];
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
	std::uniform_int_distribution<int> distribution(minNumber,maxNumber);
	std::uniform_int_distribution<int> randomTree(3, 7);
	int randomValue = randomTree(generator);
	double scalingFactor = 1;
	vector<SDL_Rect> occupiedPixels;
	for (int i = 0; i < randomValue; i++) {
		int randomInt = distribution(generator);
		SDL_Rect treeOccupyPixels;
		while (true) {
			treeOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY,static_cast<int>(this->tree->getWidth() * scalingFactor),static_cast<int>(this->tree->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &treeOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;

		}

		StaticAnimatingObject*newTree = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::Tree), 14,4, treeOccupyPixels.x, treeOccupyPixels.y, 72,96, scalingFactor);
		treeObj.push_back(newTree);
		occupiedPixels.push_back(treeOccupyPixels);
	}
}

void TreeRoad::Update() {
	for (int i = 0; i < treeObj.size(); i++) {
		treeObj[i]->Update();

	}
	return;
}

vector<SDL_Rect> TreeRoad::getDangerousRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

vector<SDL_Rect> TreeRoad::getSafeRoadObjBoundRect() {
	
	vector<SDL_Rect> ans;
	for (auto obj : treeObj) {
		ans.push_back(obj->boundingRect());
	}
	return ans;
}

void TreeRoad::Draw() {
	bg->render(0, startY, NULL, SCREEN_WIDTH, 100);
	for (auto p : treeObj) {
		p->Draw();
	}
}

void TreeRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	for(auto obj:treeObj) obj->setYCoordinate(newStartY);
}

int TreeRoad::getRoadID() {
	return static_cast<int>(ForestRoadType::TreeRoad);
}