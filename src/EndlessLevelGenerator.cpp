#include "EndlessLevelGenerator.h"
EndlessLevelGenerator::EndlessLevelGenerator(int terrainID ,int difficulty, int roadHeight, Character* player,float baseSpeed,float levelSpeed) {

	this->roadHeight = roadHeight;
	this->player = player;
	this->difficulty = difficulty;
	this->baseSpeed = baseSpeed;
	this->terrainID = terrainID;
	this->levelSpeed = levelSpeed;
	this->levelUpperPosY = 0;
	this->totalScore = 0;
	this->player->setLevelSpeed(levelSpeed);
	font = TTF_OpenFont("../../../resources/Font/ARCADECLASSIC.ttf", 28);
	this->scoreTexture = new LTexture(gRenderer);
	scoreTexture->loadFromRenderedText(to_string(totalScore), WHITE, font);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count(); 
	generator= std::mt19937_64(nanoseconds);
	distribution = std::uniform_int_distribution<int>(0, INT_MAX);
	generateFirstLevel();
}

EndlessLevelGenerator::~EndlessLevelGenerator() {
	delete scoreTexture;
	TTF_CloseFont(font);
}

void EndlessLevelGenerator::generateFirstLevel() {
	int nRoad = SCREEN_HEIGHT / roadHeight; //total number of roads
	bool isLastRoadSafe = false;
	for (int i = 0; i < nRoad; i++) {

		int randomInt = distribution(generator);
		int roadType = randomInt % static_cast<int>(RoadType::Last);
		if (isLastRoadSafe && roadType == 0) {
			i--;
			continue;
		}
		//cout << roadType << endl;
		int numVehicle, newSpeed;
		float timeRedLight, timeGreenLight;
		switch (roadType)
		{
		case 0:
			roadVector.push_back(new SimpleSafeRoad(i * roadHeight, i * roadHeight + roadHeight));
			roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
			isLastRoadSafe = true;
			break;
		case 1:
			numVehicle = sqrt(difficulty + 1) * 2;
			newSpeed = baseSpeed * (1.0);
			roadVector.push_back(new SimpleRoad(numVehicle, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
			roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
			isLastRoadSafe = false;
			break;
		case 2:
			newSpeed = baseSpeed * 24 + (baseSpeed * difficulty) / 4;
			timeRedLight = 4.f - 0.015f * static_cast<float>(difficulty);
			timeGreenLight = 2.f + 0.015f * static_cast<float>(difficulty);
			roadVector.push_back(new Railway(newSpeed, timeRedLight, timeGreenLight, i * roadHeight, i * roadHeight + roadHeight));
			roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
			isLastRoadSafe = false;
			break;
		default:
			break;
		}
	}
	//Some last pixels are saved for safe road for the player to respawn safely
	roadVector.push_back(new SimpleSafeRoad(roadVector.size() * roadHeight, SCREEN_HEIGHT));

}

void EndlessLevelGenerator::generateNewRoad() {
	float levelLowerPosY = levelUpperPosY + SCREEN_HEIGHT;
	bool isLastRoadSafe = (roadVector[0]->getRoadID() == 0);
	pair<int,int> topRoadPos = roadPosVector[0];
	int nRoad = 1; //number of roads to add each time
	if (terrainID == 0)
	{
		bool isLastRoadSafe = false;
		for (int i = 0; i < nRoad; i++) {

			int randomInt = distribution(generator);
			int roadType = randomInt % static_cast<int>(RoadType::Last);
			if (isLastRoadSafe && roadType == 0) {
				i--;
				continue;
			}

			int numVehicle, newSpeed;
			float timeRedLight, timeGreenLight;
			switch (roadType)
			{
			case 0:
				roadVector.insert(roadVector.begin(), new SimpleSafeRoad(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = true;
				break;
			case 1:
				numVehicle = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.insert(roadVector.begin(), new SimpleRoad(numVehicle, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = false;
				break;
			case 2:
				newSpeed = baseSpeed * 24 + (baseSpeed * difficulty) / 4;
				timeRedLight = 4.f - 0.04f * static_cast<float>(difficulty);
				timeGreenLight = 2.f + 0.04f * static_cast<float>(difficulty);
				roadVector.insert(roadVector.begin(), new Railway(newSpeed, timeRedLight, timeGreenLight, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = false;
				break;
			default:
				break;
			}

			//Some last pixels are saved for safe road for the player to respawn safely
		}
	}
	else
	{
		bool isLastRoadSafe = false;
		int roadType;
		for (int i = 0; i < nRoad; i++)
		{
			int randomInt = distribution(generator);
			roadType = randomInt % static_cast<int>(ForestRoadType::Last);
			if ((isLastRoadSafe && (roadType == 0 || roadType == 2))) {
				i--;
				continue;
			}
			int numAnimal, newSpeed;
			switch (roadType)
			{
			case 0:
				roadVector.insert(roadVector.begin(), new SafeForestRoad(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = true;
				break;
			case 1:
				numAnimal = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.insert(roadVector.begin(), new AnimalRoad(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = false;
				break;
			case 2:
				roadVector.insert(roadVector.begin(), new TreeRoad(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = true;
				break;
			default:
				break;
			}
		}
	}

}

int EndlessLevelGenerator::getScore() {
	return totalScore;
}

bool EndlessLevelGenerator::Update() {
	float timeStep = stepTimer.getTicks() / 1000.f;
	stepTimer.start();
	levelUpperPosY -= levelSpeed * timeStep;
	float levelLowerPosY = levelUpperPosY + SCREEN_HEIGHT;
	pair<int, int> topRoadPos = roadPosVector[0];
	pair<int, int> lastRoadPos = roadPosVector[roadPosVector.size() - 1];
	if (levelLowerPosY < lastRoadPos.first) {
		roadPosVector.pop_back();
		roadVector.pop_back();
	}
	if (levelUpperPosY <= topRoadPos.first) {
		difficulty++;
		generateNewRoad();
	}
	topRoadPos = roadPosVector[0];
	float topRoadStartY = topRoadPos.first - levelUpperPosY;
	float topRoadEndY = topRoadStartY + roadHeight;
	//cout << topRoadStartY << endl;
	bool isCollided = false;
	vector<SDL_Rect> safeObjBoundRectVector;
	for (Road* road : roadVector) {
		road->setStartEndPosRoad(topRoadStartY, topRoadEndY);
		road->Update();
		isCollided |= player->checkDangerousCollision(road->getDangerousRoadObjBoundRect());
		vector<SDL_Rect> roadSafeObjBoundRects = road->getSafeRoadObjBoundRect();
		for (SDL_Rect rect : roadSafeObjBoundRects)
			safeObjBoundRectVector.push_back(rect);
		topRoadStartY += roadHeight;
		topRoadEndY += roadHeight;
	}
	SDL_Rect screenLowerBoundRect = { 0,SCREEN_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT + 10 };
	vector<SDL_Rect> v = { screenLowerBoundRect };//only a wrapper vector for the variable
	isCollided |= player->checkDangerousCollision(v);
	player->updateIfDeath();
	player->updateDirection();
	player->updateAnimation();
	player->updateCoordinate(safeObjBoundRectVector);
	return !isCollided;
}

void EndlessLevelGenerator::Draw() {
	
	for (Road* road : roadVector) {
		road->Draw();
	}
	player->Draw();
	//render score
	scoreTexture->render(SCREEN_WIDTH / 2, 10, NULL, -1, -1);
}