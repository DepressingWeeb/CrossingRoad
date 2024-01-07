#include "EndlessLevelGenerator.h"
EndlessLevelGenerator::EndlessLevelGenerator(int difficulty, int roadHeight, Character* player,int terrainID,float baseSpeed,float levelSpeed) {
	this->roadHeight = roadHeight;
	this->player = player;
	this->difficulty = difficulty;
	this->baseSpeed = baseSpeed;
	this->levelSpeed = levelSpeed;
	this->levelUpperPosY = 0;
	this->totalScore = 0;
	this->currScore = 0;
	this->lastScore = 0;

	this->player->setLevelSpeed(levelSpeed);
	font = TTF_OpenFont("../../../resources/Font/ARCADECLASSIC.ttf", 28);
	this->scoreTexture = new LTexture(gRenderer);
	scoreTexture->loadFromRenderedText(to_string(totalScore), WHITE, font);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count(); 
	generator= std::mt19937_64(nanoseconds);
	distribution = std::uniform_int_distribution<int>(0, INT_MAX);
	this->terrainID = terrainID;
	generateFirstLevel();
}

EndlessLevelGenerator::~EndlessLevelGenerator() {
	delete scoreTexture;
	TTF_CloseFont(font);
}

void EndlessLevelGenerator::generateFirstLevel() {
	int nRoad = SCREEN_HEIGHT / roadHeight; //total number of roads
	bool isLastRoadSafe = false;
	bool isLastRoadRiver = false;
	for (int i = 0; i < nRoad; i++) {

		int randomInt = distribution(generator);
		if (terrainID == 0) {
			int roadType = randomInt % static_cast<int>(CityRoadType::Last);
			if ((isLastRoadSafe && roadType == 0) || (isLastRoadRiver && roadType == 3)) {
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
				isLastRoadRiver = false;
				break;
			case 1:
				numVehicle = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.push_back(new SimpleRoad(numVehicle, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
			case 2:

				newSpeed = baseSpeed * 24 + (baseSpeed * difficulty) / 4;
				timeRedLight = 4.f - 0.04f * static_cast<float>(difficulty);
				timeGreenLight = 2.f + 0.04f * static_cast<float>(difficulty);
				roadVector.push_back(new Railway(1200, 4, 4, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				break;

			case 3:
				roadVector.push_back(new River(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = true;
				break;
			case 4:
				roadVector.push_back(new ConstructionSite(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = true;
				isLastRoadRiver = false;
				break;
			default:
				break;
			}
		}
		else {
			int roadType = randomInt % static_cast<int>(ForestRoadType::Last);
			if ((isLastRoadSafe && roadType == 0) || (isLastRoadRiver && roadType == 3)) {
				i--;
				continue;
			}
			int numAnimal, newSpeed;
			switch (roadType)
			{
			case 0:
				roadVector.push_back(new SafeForestRoad(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = true;
				break;
			case 1:
				numAnimal = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.push_back(new AnimalRoad(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				break;
			case 2:
				roadVector.push_back(new TreeRoad(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = true;
				break;
			case 3:
				roadVector.push_back(new MonsterRoad(5.0f, 1.5f, 500, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
			case 4:
				numAnimal = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.push_back(new RollingStoneRoad(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
			case 5:
				numAnimal = 4;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.push_back(new ForestRiver(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight,player));
				roadPosVector.push_back(make_pair(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = true;
				break;
			default:
				break;
			}
		}
	}
	//Some last pixels are saved for safe road for the player to respawn safely
	if (terrainID == 0)
		roadVector.push_back(new SimpleSafeRoad(roadVector.size() * roadHeight, SCREEN_HEIGHT));
	else
		roadVector.push_back(new SafeForestRoad(roadVector.size() * roadHeight, SCREEN_HEIGHT));
	roadPosVector.push_back(make_pair((roadVector.size()-1)* roadHeight, SCREEN_HEIGHT));
	cout << roadVector.size() << " " << roadPosVector.size() << endl;
}

void EndlessLevelGenerator::generateNewRoad() {
	float levelLowerPosY = levelUpperPosY + SCREEN_HEIGHT;
	bool isLastRoadSafe = (roadVector[0]->getRoadID() == 0);
	pair<int,int> topRoadPos = roadPosVector[0];
	int nRoad = 1; //number of roads to add each time
	bool isLastRoadRiver = false;
	if(terrainID==0)
		isLastRoadRiver= (roadVector[0]->getRoadID() == static_cast<int>(CityRoadType::River));
	else
		isLastRoadRiver = (roadVector[0]->getRoadID() == static_cast<int>(ForestRoadType::ForestRiver));
	for (int i = 0; i < nRoad; i++) {

		int randomInt = distribution(generator);
		if (terrainID == 0) {
			int roadType = randomInt % static_cast<int>(CityRoadType::Last);
			if ((isLastRoadSafe && roadType == 0) || (isLastRoadRiver && roadType == 3)) {
				i--;
				continue;
			}
			//cout << roadType << endl;
			int numVehicle, newSpeed;
			float timeRedLight, timeGreenLight;
			switch (roadType)
			{
			case 0:
				roadVector.insert(roadVector.begin(), new SimpleSafeRoad(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = true;
				isLastRoadRiver = false;
				break;
			case 1:
				numVehicle = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.insert(roadVector.begin(), new SimpleRoad(numVehicle, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
			case 2:

				newSpeed = baseSpeed * 24 + (baseSpeed * difficulty) / 4;
				timeRedLight = 4.f - 0.04f * static_cast<float>(difficulty);
				timeGreenLight = 2.f + 0.04f * static_cast<float>(difficulty);
				roadVector.insert(roadVector.begin(), new Railway(1200, 4, 4, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
				/*
				roadVector.push_back(new MonsterRoad(5.0f, 1.5f, 500, i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;*/
			case 3:
				roadVector.insert(roadVector.begin(), new River(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = false;
				isLastRoadRiver = true;
				break;
			case 4:
				roadVector.insert(roadVector.begin(), new ConstructionSite(i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = true;
				isLastRoadRiver = false;
				break;
			default:
				break;
			}
		}
		else {
			int roadType = randomInt % static_cast<int>(ForestRoadType::Last);
			if ((isLastRoadSafe && roadType == 0) || (isLastRoadRiver && roadType == 3)) {
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
			case 3:
				roadVector.insert(roadVector.begin(), new MonsterRoad(5.0f, 1.5f, 500, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
			case 4:
				numAnimal = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.insert(roadVector.begin(), new RollingStoneRoad(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
			case 5:
				numAnimal = 4;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.insert(roadVector.begin(), new ForestRiver(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight,player));
				roadPosVector.insert(roadPosVector.begin(), { topRoadPos.first - roadHeight,topRoadPos.second - roadHeight });
				isLastRoadSafe = false;
				isLastRoadRiver = true;
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
		lastScore++;
	}
	if (levelUpperPosY <= topRoadPos.first) {
		difficulty++;
		generateNewRoad();
	}
	topRoadPos = roadPosVector[0];
	float topRoadStartY = topRoadPos.first - levelUpperPosY;
	float topRoadEndY = topRoadStartY + roadHeight;
	SDL_Rect playerRect = player->getBoundingRect();
	int playerFootYCoordinate = playerRect.y + playerRect.h;
	int currRoad = 0;
	bool isCollided = false;
	vector<SDL_Rect> safeObjBoundRectVector;
	for (Road* road : roadVector) {
		road->setStartEndPosRoad(topRoadStartY, topRoadEndY);
		road->Update();
		isCollided |= player->checkDangerousCollision(road->getDangerousRoadObjBoundRect());
		vector<SDL_Rect> roadSafeObjBoundRects = road->getSafeRoadObjBoundRect();
		for (SDL_Rect rect : roadSafeObjBoundRects)
			safeObjBoundRectVector.push_back(rect);
		if (playerFootYCoordinate >= 0) {
			currRoad++;
			playerFootYCoordinate -= (topRoadEndY - std::max(static_cast<int>(topRoadStartY), 0));
		}
		
		topRoadStartY += roadHeight;
		topRoadEndY += roadHeight;
	}
	currScore = roadVector.size() - currRoad;
	totalScore = currScore + lastScore;
	scoreTexture->loadFromRenderedText(to_string(totalScore), WHITE, font);

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

void EndlessLevelGenerator::ToFile(std::ostream& out) {
	out << roadHeight << endl;
	out << difficulty << endl;
	out << currScore << endl;
	out << lastScore << endl;
	out << totalScore << endl;
	out << baseSpeed << endl;
	out << levelSpeed << endl;
	out << levelUpperPosY << endl;
	out << terrainID << endl;
	out << roadVector.size() << endl;
	for (int i = 0; i < roadVector.size();i++) {
		out << roadVector[i]->getRoadID() << " "<<roadPosVector[i].first<<" "<<roadPosVector[i].second<<endl;
	}
}

EndlessLevelGenerator::EndlessLevelGenerator(Character* player,std::ifstream& in) {
	int roadSize;
	in >> roadHeight >> difficulty >> currScore >> lastScore >> totalScore >> baseSpeed >> levelSpeed >> levelUpperPosY >> terrainID >> roadSize;
	roadPosVector = vector<pair<int, int>>(roadSize, make_pair(0, 0));
	for (int i = 0; i < roadSize; i++) {
		int roadType;
		in >> roadType>>roadPosVector[i].first>>roadPosVector[i].second;
		if (terrainID == 0) {
			//cout << roadType << endl;
			int numVehicle, newSpeed;
			float timeRedLight, timeGreenLight;
			switch (roadType)
			{
			case 0:
				roadVector.push_back(new SimpleSafeRoad(i * roadHeight, i * roadHeight + roadHeight));
				break;
			case 1:
				numVehicle = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.push_back(new SimpleRoad(numVehicle, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				break;
			case 2:

				newSpeed = baseSpeed * 24 + (baseSpeed * difficulty) / 4;
				timeRedLight = 4.f - 0.04f * static_cast<float>(difficulty);
				timeGreenLight = 2.f + 0.04f * static_cast<float>(difficulty);
				roadVector.push_back(new Railway(1200, 4, 4, i * roadHeight, i * roadHeight + roadHeight));
				break;

			case 3:
				roadVector.push_back(new River(i * roadHeight, i * roadHeight + roadHeight));
				break;
			case 4:
				roadVector.push_back(new ConstructionSite(i * roadHeight, i * roadHeight + roadHeight));
				break;
			default:
				break;
			}
		}
		else {
			int numAnimal, newSpeed;
			switch (roadType)
			{
			case 0:
				roadVector.push_back(new SafeForestRoad(i * roadHeight, i * roadHeight + roadHeight));
				break;
			case 1:
				numAnimal = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.push_back(new AnimalRoad(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				break;
			case 2:
				roadVector.push_back(new TreeRoad(i * roadHeight, i * roadHeight + roadHeight));
				break;
			case 3:
				roadVector.push_back(new MonsterRoad(5.0f, 1.5f, 500, i * roadHeight, i * roadHeight + roadHeight));
				break;
			case 4:
				numAnimal = sqrt((difficulty / (SCREEN_WIDTH / roadHeight)) + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.push_back(new RollingStoneRoad(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				break;
			case 5:
				numAnimal = 4;
				newSpeed = baseSpeed * (1.0 + 0.04 * static_cast<float>(difficulty));
				roadVector.push_back(new ForestRiver(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight, player));
				break;
			default:
				break;
			}
		}
	}
	this->player = player;
	this->player->setLevelSpeed(levelSpeed);
	font = TTF_OpenFont("../../../resources/Font/ARCADECLASSIC.ttf", 28);
	this->scoreTexture = new LTexture(gRenderer);
	scoreTexture->loadFromRenderedText(to_string(totalScore), WHITE, font);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();
	generator = std::mt19937_64(nanoseconds);
	distribution = std::uniform_int_distribution<int>(0, INT_MAX);
}