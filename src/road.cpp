#include "road.h"
#include "ResourceManager.h"
SimpleRoad::SimpleRoad(int nVehicle, int speed, int startY, int endY) {
	this->roadTexture = new LTexture(gRenderer, "../../../resources/pack/Levels/summer_road.png");
	this->nVehicle = nVehicle;
	this->speed = speed;
	this->startY = startY;
	this->endY = endY;
	int offsetY1 = (endY-startY)/5;
	int offsetY2 = (endY-startY)/2+ (endY - startY) / 12;
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	vector<SDL_Rect> occupiedPixels;
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

	for (int i = 0; i < nVehicle/2; i++) {
		int randomInt = distribution(generator);
		ResourceType randomVehicle = static_cast<ResourceType>(randomInt % static_cast<int>(ResourceType::Last));
		vector<LTexture*> vehicleTexture = resourceManager.GetTexture(randomVehicle);
		SDL_Rect vehicleOccupyPixels;
		while (true) {
			vehicleOccupyPixels = { rand() % SCREEN_WIDTH ,startY + offsetY1,vehicleTexture[0]->getWidth(),vehicleTexture[0]->getHeight() };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &vehicleOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		roadObj.push_back(new AnimatingObject(gRenderer, vehicleTexture, vehicleTexture.size(), 10, rand()%SCREEN_WIDTH,startY+offsetY1, -1, -1, speed,0.25));
	}
	for (int i = 0; i < nVehicle / 2; i++) {
		int randomInt = distribution(generator);
		ResourceType randomVehicle = static_cast<ResourceType>(randomInt % static_cast<int>(ResourceType::Last));
		vector<LTexture*> vehicleTexture = resourceManager.GetTexture(randomVehicle);
		SDL_Rect vehicleOccupyPixels;
		while (true) {
			vehicleOccupyPixels = { rand() % SCREEN_WIDTH ,startY + offsetY2,vehicleTexture[0]->getWidth(),vehicleTexture[0]->getHeight() };
			bool isOccupied = false;
			for (SDL_Rect occupied : occupiedPixels) {
				if (SDL_HasIntersection(&occupied, &vehicleOccupyPixels))
					isOccupied = true;
			}
			if (!isOccupied)
				break;
		}
		roadObj.push_back(new AnimatingObject(gRenderer, vehicleTexture, vehicleTexture.size(), 10, rand() % SCREEN_WIDTH, startY + offsetY2, -1, -1, -speed, 0.25));
	}
	
}

void SimpleRoad::Update() {
	for (AnimatingObject* obj : roadObj) {
		obj->Update();
	}
}
void SimpleRoad::Draw() {
	roadTexture->render(0, startY, NULL, SCREEN_WIDTH, endY - startY);
	for (auto obj : roadObj) {
		obj->Draw();
	}
}

vector<AnimatingObject*> SimpleRoad::getRoadObj() {
	return roadObj;
}


SimpleSafeRoad::SimpleSafeRoad(int startY, int endY) {
	this->roadTexture = new LTexture(gRenderer, "../../../resources/Road/simpleSafeRoad.png");
	this->startY = startY;
	this->endY = endY;
}

void SimpleSafeRoad::Update() {
	return;
}

void SimpleSafeRoad::Draw() {
	roadTexture->render(0, startY, NULL, SCREEN_WIDTH, endY - startY);
}

vector<AnimatingObject*> SimpleSafeRoad::getRoadObj() {
	return vector<AnimatingObject*>();
}