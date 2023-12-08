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
		roadObj.push_back(make_pair(new NormalVehicle(gRenderer, vehicleTexture, vehicleTexture.size(), 10, vehicleOccupyPixels.x, startY + offsetY2, -1, -1, -speed, 0.25),1));
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
	return 1;
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
	roadTexture->render(0, startY, NULL, SCREEN_WIDTH, endY - startY);
}

int SimpleSafeRoad::getRoadID() {
	return 0;
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