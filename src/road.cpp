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
	int offsetY1 = (endY - startY) / 12;//the upper lane
	int offsetY2 = (endY - startY) / 3 + (endY - startY) / 6;//The lower lane
	//Get the resourceManager instance
	
	vector<SDL_Rect> occupiedPixels;

	const float scalingFactor = 0.4;	// Default scaling factor of vehicle resources
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
		AnimatingObject* newVehicle = new NormalVehicle(gRenderer, vehicleTexture, vehicleTexture.size(), 10, vehicleOccupyPixels.x, startY + offsetY1, -1, -1, speed, scalingFactor);
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
		roadObj.push_back(make_pair(new NormalVehicle(gRenderer, vehicleTexture, vehicleTexture.size(), 10, vehicleOccupyPixels.x, startY + offsetY2, -1, -1, -speed, scalingFactor),1));
		occupiedPixels.push_back(vehicleOccupyPixels);
	}
	
}

void SimpleRoad::Update() {
	for (pair<AnimatingObject*,int> obj : roadObj) {
		obj.first->Update();
	}
}
void SimpleRoad::Draw() {
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
	for (auto obj : roadObj) {
		obj.first->Draw();
	}
}
int SimpleRoad::getRoadID() {
	//the id in enum class
	return static_cast<int>(CityRoadType::SimpleRoad);
}

void SimpleRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	//In case there are changes in startY and endY, due to the level moving in endless mode,then update each vehicle's Y coordinate
	int offsetY1 = (endY - startY) / 12;//the upper lane
	int offsetY2 = (endY - startY) / 3 + (endY - startY) / 6;//The lower lane
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

SimpleSafeRoad::SimpleSafeRoad(int startY, int endY) {
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::SimpleSafeRoad)[0];
	this->startY = startY;
	this->endY = endY;
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();

	// Seed the random number generator with nanoseconds
	std::mt19937_64 generator(nanoseconds);

	// Define the range for the random number
	int minNumber = 0;
	int maxNumber = INT_MAX;
	// Define the distribution and generate the random number
	std::uniform_int_distribution<int> distribution(minNumber, maxNumber);
	std::uniform_int_distribution<int> randomVendor(2, 3);
	int randomValue = randomVendor(generator);
	double scalingFactor = 0.5;
	LTexture* vendor;
	vector<SDL_Rect> occupiedPixels;
	for (int i = 0; i < randomValue; i++) {
		int randomInt = distribution(generator);
		ResourceType randomVendor = vendorResources[(randomInt % vendorResources.size())];
		vendor = resourceManager.GetTexture(randomVendor)[0];
		SDL_Rect vendorOccupyPixels;
		while (true) {
			int randomX;
			if (distribution(generator) % 2 == 0) {
				// Random x-coordinate in the range [0, 200]
				randomX = distribution(generator) % 201;
			}
			else {
				// Random x-coordinate in the range [400, 1200]
				randomX = 400 + distribution(generator) % 801;
			}
			vendorOccupyPixels = { randomX ,startY ,static_cast<int>(vendor->getWidth() * scalingFactor),static_cast<int>(vendor->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &vendorOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		StaticAnimatingObject* newVendor = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(randomVendor), 4, 10, vendorOccupyPixels.x, vendorOccupyPixels.y, -1, -1, scalingFactor);
		vendorObj.push_back(newVendor);
		occupiedPixels.push_back(vendorOccupyPixels);
	}
}


void SimpleSafeRoad::Update() {
	for (int i = 0; i < vendorObj.size(); i++) {
		vendorObj[i]->Update();

	}
}

void SimpleSafeRoad::Draw() {
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
	for (auto p : vendorObj) {
		p->Draw();
	}
}

int SimpleSafeRoad::getRoadID() {
	return static_cast<int>(CityRoadType::SimpleSafeRoad);
}

void SimpleSafeRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	for (auto obj : vendorObj) 
		obj->setYCoordinate(newStartY);
}

vector<SDL_Rect> SimpleSafeRoad::getDangerousRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

vector<SDL_Rect> SimpleSafeRoad::getSafeRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	for (auto obj : vendorObj) {
		SDL_Rect rect;
		rect.x = obj->boundingRect().x + obj->boundingRect().w / 4;
		rect.y = obj->boundingRect().y + obj->boundingRect().h / 4;
		rect.w = obj->boundingRect().w - obj->boundingRect().w / 2;
		rect.h = obj->boundingRect().h - obj->boundingRect().h / 3;
		ans.push_back(rect);
	}
	return ans;
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
	return static_cast<int>(CityRoadType::Railway);
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
	return static_cast<int>(CityRoadType::River);
}


ConstructionSite::ConstructionSite(int startY, int endY) {
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::ConstructionSite)[0];
	this->startY = startY;
	this->endY = endY;

	int offset = (endY - startY) / 8;
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
	std::uniform_int_distribution<int> random(3, 4);
	int randomValue = random(generator);
	double scalingFactor = 1;
	vector<SDL_Rect> occupiedPixels;
	for (int i = 0; i < randomValue; i++) {
		int randomInt = distribution(generator);
		ResourceType randomDanger = dangerResources[(randomInt % dangerResources.size())];
		this->danger = resourceManager.GetTexture(randomDanger)[0];
		SDL_Rect dangerOccupyPixels;
		while (true) {
			dangerOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offset ,static_cast<int>(this->danger->getWidth() * scalingFactor),static_cast<int>(this->danger->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &dangerOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		StaticAnimatingObject* newDanger = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(randomDanger), 1, 10, dangerOccupyPixels.x, dangerOccupyPixels.y, -1, -1, scalingFactor);
		dangerObj.push_back(newDanger);
		occupiedPixels.push_back(dangerOccupyPixels);
	};
}

void ConstructionSite::Update() {
	return;
}

vector<SDL_Rect> ConstructionSite::getDangerousRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

vector<SDL_Rect> ConstructionSite::getSafeRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	for (auto obj : dangerObj) {
		SDL_Rect rect;
		rect.x = obj->boundingRect().x + obj->boundingRect().w / 4;
		rect.y = obj->boundingRect().y + obj->boundingRect().h / 4;
		rect.w = obj->boundingRect().w - obj->boundingRect().w / 2;
		rect.h = obj->boundingRect().h - obj->boundingRect().h / 3;
		ans.push_back(rect);
	}
	return ans;
}

void ConstructionSite::Draw() {
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
	for (auto p : dangerObj) {
		p->Draw();
	}
}

void ConstructionSite::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	for (StaticAnimatingObject* obj : dangerObj) {
		obj->setYCoordinate(newStartY);
	}
}

int ConstructionSite::getRoadID() {
	return static_cast<int>(CityRoadType::ConstructionSite);
}


//==============================================SafeForestRoad================================
SafeForestRoad::SafeForestRoad(int startY, int endY) {
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::SafeForestRoad)[0];
	this->startY = startY;
	this->endY = endY;
	this->flower = resourceManager.GetTexture(ResourceType::Flower)[0];
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
	std::uniform_int_distribution<int> randomFlower(8, 12);
	int randomValue = randomFlower(generator);
	double scalingFactor = 2;
	vector<SDL_Rect> occupiedPixels;
	for (int i = 0; i < randomValue; i++) {
		int randomInt = distribution(generator);
		SDL_Rect flowerOccupyPixels;
		while (true) {
			flowerOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY,static_cast<int>(this->flower->getWidth() * scalingFactor),static_cast<int>(this->flower->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &flowerOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;

		}

		StaticAnimatingObject* newFlower = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::Flower), 14, 10, flowerOccupyPixels.x, flowerOccupyPixels.y, -1, -1, scalingFactor);
		flowerObj.push_back(newFlower);
		occupiedPixels.push_back(flowerOccupyPixels);
	}
}

void SafeForestRoad::Update() {
	for (int i = 0; i < flowerObj.size(); i++) {
		flowerObj[i]->Update();

	}
	return;
}

void SafeForestRoad::Draw() {
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
	for (auto p : flowerObj) {
		p->Draw();
	}
}

int SafeForestRoad::getRoadID() {
	return static_cast<int>(ForestRoadType::SafeForestRoad);
}

void SafeForestRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	for (auto obj : flowerObj) obj->setYCoordinate(newStartY);
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
	this->roadTexture = resourceManager.GetTexture(ResourceType::AnimalRoad)[0];
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
	this->roadTexture = resourceManager.GetTexture(ResourceType::SafeForestRoad)[0];
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
	std::uniform_int_distribution<int> randomTree(3, 7);
	int randomValue = randomTree(generator);
	double scalingFactor = 1;
	vector<SDL_Rect> occupiedPixels;
	for (int i = 0; i < randomValue; i++) {
		int randomInt = distribution(generator);
		SDL_Rect treeOccupyPixels;
		while (true) {
			treeOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY,72,96 };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &treeOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;

		}

		StaticAnimatingObject* newTree = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::Tree), 14, 4, treeOccupyPixels.x, treeOccupyPixels.y, -1, -1, scalingFactor);
		treeObj.push_back(newTree);
		occupiedPixels.push_back(treeOccupyPixels);
	}
	for (auto obj : treeObj) {
		SDL_Rect objRect = obj->boundingRect();
		cout << "Constructor Object : " << objRect.x << " " << objRect.y << " " << objRect.w << " " << objRect.h << endl;
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
	int nRoadRender = ceil(SCREEN_WIDTH / static_cast<float>(roadTexture->getWidth()));
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadTexture->getWidth() * i, startY, NULL, roadTexture->getWidth(), endY - startY);
	}
	for (auto p : treeObj) {
		p->Draw();
	}
}

void TreeRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	for (auto obj : treeObj) obj->setYCoordinate(newStartY);
}

int TreeRoad::getRoadID() {
	return static_cast<int>(ForestRoadType::TreeRoad);
}

MonsterRoad::MonsterRoad(float timeIdle, float timeArrow, int arrowSpeed, int startY, int endY) {
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::MonsterRoad)[0];
	this->timeIdle = timeIdle;
	this->timeArrow = timeArrow;
	this->arrowSpeed = arrowSpeed;
	this->startY = startY;
	this->endY = endY;
	this->archer = new Monster(gRenderer, 0, startY, 288, 128, 3, 1, 2000);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();

	// Seed the random number generator with nanoseconds
	std::mt19937_64 generator(nanoseconds);

	// Define the range for the random number
	int minNumber = 0;
	int maxNumber = INT_MAX;

	// Define the distribution and generate the random number
	std::uniform_int_distribution<int> distribution(minNumber, maxNumber);
	int nDecorator = distribution(generator)%50;
	for (int i = 0; i < nDecorator; i++) {
		int decoratorType = distribution(generator) % 11;
		int xRand = distribution(generator) % SCREEN_WIDTH;
		int yRand = startY+(distribution(generator) % (endY-startY-32));
		if (decoratorType < 9) {
			vector<LTexture*> temp;
			temp.push_back(resourceManager.GetTexture(ResourceType::Grass)[decoratorType]);
			decoratorObj.push_back(new StaticAnimatingObject(gRenderer,temp , 1, 4, xRand, yRand, -1, -1));
		}
		else if (decoratorType == 9) {
			decoratorObj.push_back(new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::Bush), 14, 8, xRand, yRand, -1, -1));
		}
		else {
			decoratorObj.push_back(new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::Flower), 14, 8, xRand, yRand, -1, -1));
		}
	}

}

int MonsterRoad::getRoadID() {
	return static_cast<int>(ForestRoadType::MonsterRoad);
}

void MonsterRoad::Draw() {
	const float roadWidth = 50;
	const float roadHeight = 50;
	int nRoadRender = ceil(SCREEN_WIDTH / roadWidth);
	for (int i = 0; i < nRoadRender; i++) {
		roadTexture->render(roadWidth * i, startY, NULL, roadWidth, (endY-startY)/2);
		roadTexture->render(roadWidth * i, startY+ (endY - startY) / 2, NULL, roadWidth, (endY - startY) / 2);
	}
	for (StaticAnimatingObject* obj : decoratorObj)
		obj->Draw();
	archer->Draw();
	vector<SDL_Rect> debugDangerRect = getDangerousRoadObjBoundRect();
	//SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	/*
	SDL_SetRenderDrawColor(gRenderer, 200, 0, 0, 100);
	for (SDL_Rect dangerRect : debugDangerRect) {
		SDL_RenderFillRect(gRenderer, &dangerRect);
	}
	*/
}

void MonsterRoad::Update() {
	archer->Update();
	for (StaticAnimatingObject* obj : decoratorObj)
		obj->Update();
}

void MonsterRoad::setStartEndPosRoad(int newStartY,int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	archer->setYCoordinate(newStartY);
	for (auto obj : decoratorObj) 
		obj->setYCoordinate(newStartY);
}

vector<SDL_Rect> MonsterRoad::getSafeRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

vector<SDL_Rect> MonsterRoad::getDangerousRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	ans.push_back(archer->boundingRect());
	vector<SDL_Rect> arrowBoundRects = archer->arrowBoundingRect();
	for (SDL_Rect rect : arrowBoundRects)
		ans.push_back(rect);
	return ans;
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
	int offsetY2 = (endY - startY) / 3 ;//The lower lane

	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();

	std::mt19937_64 generator(nanoseconds);

	int minNumber = 0;
	int maxNumber = INT_MAX;

	std::uniform_int_distribution<int> distribution(minNumber, maxNumber);

	const float scalingFactor = 0.25;
	vector<SDL_Rect> occupiedPixels;

	for (int i = 0; i < nStone / 2; i++) {
		vector<LTexture*> stoneTexture = resourceManager.GetTexture(ResourceType::Stone);
		SDL_Rect stoneOccupyPixels;
		while (true) {
			cout << "loop 1" << endl;
			stoneOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY1,static_cast<int>(stoneTexture[0]->getWidth() * scalingFactor),static_cast<int>(stoneTexture[0]->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &stoneOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;

		}
		AnimatingObject* newStone = new NormalVehicle(gRenderer, stoneTexture, stoneTexture.size(), 10, stoneOccupyPixels.x, startY + offsetY1, -1, -1, speed, scalingFactor);
		roadObj.push_back(make_pair(newStone, 0));
		occupiedPixels.push_back(stoneOccupyPixels);
	}

	for (int i = 0; i < nStone / 2; i++) {
		vector<LTexture*> stoneTexture = resourceManager.GetTexture(ResourceType::Stone);
		SDL_Rect stoneOccupyPixels;
		while (true) {
			cout << "loop 1" << endl;
			stoneOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY2,static_cast<int>(stoneTexture[0]->getWidth() * scalingFactor),static_cast<int>(stoneTexture[0]->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &stoneOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		roadObj.push_back(make_pair(new NormalVehicle(gRenderer, stoneTexture, stoneTexture.size(), 10, stoneOccupyPixels.x, startY + offsetY2, -1, -1, -speed, 0.25), 1));
		occupiedPixels.push_back(stoneOccupyPixels);
	}
}

void RollingStoneRoad::Update() {
	for (pair<AnimatingObject*, int> obj : roadObj) {
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
	return static_cast<int>(ForestRoadType::RollingStoneRoad);
}

void RollingStoneRoad::setStartEndPosRoad(int newStartY, int newEndY) {
	this->startY = newStartY;
	this->endY = newEndY;
	//In case there are changes in startY and endY, due to the level moving in endless mode,then update each vehicle's Y coordinate
	int offsetY1 = 0;//the upper lane
	int offsetY2 = (endY - startY) / 3;//The lower lane
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
	for (pair<AnimatingObject*, int> obj : roadObj) {
		ans.push_back(obj.first->boundingRect());
	}
	return ans;
}

vector<SDL_Rect> RollingStoneRoad::getSafeRoadObjBoundRect() {
	return vector<SDL_Rect>();
}

ForestRiver::ForestRiver(int n, int speed, int startY, int endY,Character* player) {
	//Get resource manager instance
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	this->roadTexture = resourceManager.GetTexture(ResourceType::SimpleSafeRoad)[0];
	this->n = n;
	this->speed = speed;
	this->startY = startY;
	this->endY = endY;
	this->player = player;
	int offsetY = (endY - startY) / 2;//The lower lane

	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();

	std::mt19937_64 generator(nanoseconds);

	int minNumber = 0;
	int maxNumber = INT_MAX;

	std::uniform_int_distribution<int> distribution(minNumber, maxNumber);

	const float scalingFactor = 0.5;
	vector<SDL_Rect> occupiedPixels;
	for (int i = 0; i < n / 2; i++) {
		vector<LTexture*> timberTexture = resourceManager.GetTexture(ResourceType::Timber);
		SDL_Rect timberOccupyPixels;
		while (true) {
			cout << "loop 1" << endl;
			timberOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY,static_cast<int>(timberTexture[0]->getWidth() * scalingFactor),static_cast<int>(timberTexture[0]->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &timberOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;

		}
		AnimatingObject* newTimber = new NormalVehicle(gRenderer, timberTexture, timberTexture.size(), 10, timberOccupyPixels.x, startY, -1, -1, speed, scalingFactor);
		roadObj.push_back(make_pair(newTimber, 0));
		occupiedPixels.push_back(timberOccupyPixels);
	}
	for (int i = 0; i < n / 2; i++) {
		vector<LTexture*> timberTexture = resourceManager.GetTexture(ResourceType::Timber);
		SDL_Rect timberOccupyPixels;
		while (true) {
			cout << "loop 2 : " << n/2 << endl;
			timberOccupyPixels = { distribution(generator) % SCREEN_WIDTH ,startY + offsetY,static_cast<int>(timberTexture[0]->getWidth() * scalingFactor),static_cast<int>(timberTexture[0]->getHeight() * scalingFactor) };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &timberOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		roadObj.push_back(make_pair(new NormalVehicle(gRenderer, timberTexture, timberTexture.size(), 10, timberOccupyPixels.x, startY + offsetY, -1, -1, -speed, scalingFactor), 1));
		occupiedPixels.push_back(timberOccupyPixels);
	}

	int tileHeight = (endY - startY) / 2;
	int tileWidth = tileHeight;
	int nPairTile = ceil(SCREEN_WIDTH / static_cast<float>(tileWidth));
	for (int i = 0; i < nPairTile; i++) {
		StaticAnimatingObject* upperWaterTile = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::UpperWaterLane), 14, 4, i * tileWidth, startY, tileWidth, tileHeight);
		StaticAnimatingObject* lowerWaterTile = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::LowerWaterLane), 14, 4, i * tileWidth, startY + tileHeight, tileWidth, tileHeight);
		this->waters.push_back(make_pair(upperWaterTile, lowerWaterTile));
	}
}

int ForestRiver::getRoadID() {
	return static_cast<int>(ForestRoadType::ForestRiver);
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
	for (auto p : waters) {
		p.first->setYCoordinate(newStartY);
		p.second->setYCoordinate(newStartY + (endY - startY) / 2);
	}

}

vector<SDL_Rect> ForestRiver::getDangerousRoadObjBoundRect() {
	vector<SDL_Rect> ans;
	const int offset = 10;
	vector<SDL_Rect> timberBoundRectsUpperLane;
	vector<SDL_Rect> timberBoundRectsLowerLane;
	//Get the timber bound rect for 2 lane
	for (auto timber : roadObj) {
		SDL_Rect boundRect = timber.first->boundingRect();
		//undo the offset
		boundRect.x -= 20;
		boundRect.y -= 20;
		boundRect.w += 40;
		boundRect.h += 40;
		if (timber.second == 0)
			timberBoundRectsUpperLane.push_back(boundRect);
		else
			timberBoundRectsLowerLane.push_back(boundRect);
	}
	//must ensure that at least 1 timber for each lane exists
	assert(timberBoundRectsLowerLane.size() > 0,"Must ensure that at least 1 timber for each lane exists");
	assert(timberBoundRectsUpperLane.size() > 0, "Must ensure that at least 1 timber for each lane exists");
	//sort the bound rect ascending by x coordinate
	std::sort(timberBoundRectsUpperLane.begin(), timberBoundRectsUpperLane.end(), [](SDL_Rect rect1, SDL_Rect rect2) {return rect1.x < rect2.x; });
	std::sort(timberBoundRectsLowerLane.begin(), timberBoundRectsLowerLane.end(), [](SDL_Rect rect1, SDL_Rect rect2) {return rect1.x < rect2.x; });
	for (int i = 1; i < timberBoundRectsUpperLane.size(); i++) {
		SDL_Rect prev = timberBoundRectsUpperLane[i - 1];
		SDL_Rect curr = timberBoundRectsUpperLane[i];
		//danger rect is between the gap of the 2 rect
		SDL_Rect dangerRect = { prev.x + prev.w,startY+offset*2,curr.x - (prev.x + prev.w),(endY-startY)/2-offset*2 };
		ans.push_back(dangerRect);
	}
	//add the gap between the first and last timber if exist
	SDL_Rect first = timberBoundRectsUpperLane[0];
	SDL_Rect last = timberBoundRectsUpperLane[timberBoundRectsUpperLane.size() - 1];
	if (first.x > 0) {
		ans.push_back({ 0,startY+offset * 2,first.x,(endY - startY) / 2 - offset * 2 });
	}
	if (last.x + last.w < SCREEN_WIDTH) {
		ans.push_back({ last.x + last.w ,startY+offset * 2,SCREEN_WIDTH - (last.x + last.w),(endY - startY) / 2 - offset * 2 });
	}

	for (int i = 1; i < timberBoundRectsLowerLane.size(); i++) {
		SDL_Rect prev = timberBoundRectsLowerLane[i - 1];
		SDL_Rect curr = timberBoundRectsLowerLane[i];
		//danger rect is between the gap of the 2 rect
		SDL_Rect dangerRect = { prev.x + prev.w,startY+ (endY - startY) / 2+offset,curr.x - (prev.x + prev.w),(endY - startY) / 2-offset*2 };
		ans.push_back(dangerRect);
	}
	//add the gap between the first and last timber if exist
	first = timberBoundRectsLowerLane[0];
	last = timberBoundRectsLowerLane[timberBoundRectsLowerLane.size() - 1];
	if (first.x > 0) {
		ans.push_back({ 0,startY+ (endY - startY) / 2 + offset,first.x,(endY - startY) / 2 - offset * 2 });
	}
	if (last.x + last.w < SCREEN_WIDTH) {
		ans.push_back({ last.x + last.w ,startY+ (endY - startY) / 2 + offset,SCREEN_WIDTH - (last.x + last.w),(endY - startY) / 2 - offset * 2 });
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
	
	for (pair<AnimatingObject*, int> obj : roadObj) {
		obj.first->Update();
	}
	SDL_Rect playerRect = player->getBoundingRect();
	pair<float, float> coord = player->getCoordinate();
	float timeStep = stepTimer.getTicks() / 1000.f;
	stepTimer.start();

	if (playerRect.y + playerRect.h > startY && playerRect.y + playerRect.h < startY + (endY - startY) / 2) {

		float deltaX = speed * timeStep;
		player->setXCoordinate(coord.first + deltaX);
	}
	else if (playerRect.y + playerRect.h > startY + (endY - startY) / 2 && playerRect.y + playerRect.h < endY) {

		float deltaX = -speed * timeStep;
		player->setXCoordinate(coord.first + deltaX);
	}
	for (int i = 0; i < waters.size(); i++) {
		waters[i].first->Update();
		waters[i].second->Update();
	}
}
void ForestRiver::Draw() {
	roadTexture->render(0, startY, NULL, SCREEN_WIDTH, endY - startY);
	for (auto p : waters) {
		p.first->Draw();
		p.second->Draw();
	}
	vector<SDL_Rect> debugDangerRect = getDangerousRoadObjBoundRect();
	
	for (auto obj : roadObj) {
		obj.first->Draw();
	}
	/*
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(gRenderer, 200, 0, 0, 100);
	for (SDL_Rect dangerRect : debugDangerRect) {
		SDL_RenderFillRect(gRenderer, &dangerRect);
	}
	*/
	
}

