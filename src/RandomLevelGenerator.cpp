#include "RandomLevelGenerator.h"

RandomLevelGenerator::RandomLevelGenerator(int difficulty, int roadHeight, Character* player,int terrainID,float baseSpeed) {
	
	this->roadHeight = roadHeight;
	this->player = player;
	this->difficulty = difficulty;
	this->level = 0;
	this->lastLevelScore = 0;
	this->currScore = 0;
	this->baseSpeed = baseSpeed;
	font = TTF_OpenFont("../../../resources/Font/ARCADECLASSIC.ttf", 28);
	this->scoreTexture = new LTexture(gRenderer);
	scoreTexture->loadFromRenderedText(to_string(totalScore), WHITE, font);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();
	generator = std::mt19937_64(nanoseconds);
	distribution = std::uniform_int_distribution<int>(0, INT_MAX);
	this->terrainID = terrainID;
	generateNewLevel();
}

RandomLevelGenerator::~RandomLevelGenerator() {
	delete scoreTexture;
	TTF_CloseFont(font);
}

void RandomLevelGenerator::generateNewLevel() {
	
	//Currently this handles difficulty in a really crude way due to limited number of road types,need to be dealt with in the future
	//May need biased randomization
	roadVector.clear();
	int nRoad = SCREEN_HEIGHT / roadHeight; //total number of roads
	bool isLastRoadSafe = false;
	bool isLastRoadRiver = false;
	cloudObj.clear();
	if (distribution(generator) % 2 == 0) {
		hasCloud = true;
		ResourceManager& resourceManager = ResourceManager::GetInstance();
		this->cloud = resourceManager.GetTexture(ResourceType::cloud)[0];
		SDL_Rect cloudOccupyPixels;
		int scalingFactor = 2;
		cloudOccupyPixels = { distribution(generator) % SCREEN_WIDTH - this->cloud->getWidth(), distribution(generator) % SCREEN_HEIGHT - this->cloud->getHeight() * scalingFactor, static_cast<int>(this->cloud->getWidth() * scalingFactor),static_cast<int>(this->cloud->getHeight() * scalingFactor) };
		StaticAnimatingObject* newCloud = new StaticAnimatingObject(gRenderer, resourceManager.GetTexture(ResourceType::cloud), 3, 20, cloudOccupyPixels.x, cloudOccupyPixels.y, -1, -1, scalingFactor);
		cloudObj.push_back(newCloud);
	}
	else {
		hasCloud = false;
	}

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
				isLastRoadSafe = true;
				isLastRoadRiver = false;
				break;
			case 1:
				numVehicle = sqrt(difficulty + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.2 * static_cast<float>(difficulty));
				roadVector.push_back(new SimpleRoad(numVehicle, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
			case 2:
				newSpeed = baseSpeed * 24 + (baseSpeed * difficulty*2);
				timeRedLight = 4.f - 0.1f * static_cast<float>(difficulty);
				timeGreenLight = 2.f + 0.1f * static_cast<float>(difficulty);
				roadVector.push_back(new Railway(1200, 4, 4, i * roadHeight, i * roadHeight + roadHeight));
				break;
			case 3:
				roadVector.push_back(new River(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = true;
				break;
			case 4:
				roadVector.push_back(new ConstructionSite(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = true;
				isLastRoadRiver = false;
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
				isLastRoadSafe = true;
				break;
			case 1:
				numAnimal = sqrt(difficulty + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.2 * static_cast<float>(difficulty));
				roadVector.push_back(new AnimalRoad(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				break;
			case 2:
				roadVector.push_back(new TreeRoad(i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = true;
				break;
			case 3:
				roadVector.push_back(new MonsterRoad(5.0f, 1.5f, 500, i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
			case 4:
				numAnimal = sqrt(difficulty + 1) * 2;
				newSpeed = baseSpeed * (1.0 + 0.2 * static_cast<float>(difficulty));
				roadVector.push_back(new RollingStoneRoad(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
				isLastRoadSafe = false;
				isLastRoadRiver = false;
				break;
			case 5:
				numAnimal = 6;
				newSpeed = baseSpeed * (1.0 + 0.2 * static_cast<float>(difficulty));
				roadVector.push_back(new ForestRiver(numAnimal, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
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
	cout <<"Road vector size: " << roadVector.size() << endl;
}

int RandomLevelGenerator::getScore() {
	return totalScore;
}

bool RandomLevelGenerator::Update() {
	SDL_Rect playerRect = player->getBoundingRect();
	
	if (hasCloud) {
		for (int i = 0; i < cloudObj.size(); i++) {
			cloudObj[i]->Update();
		}
	}
	//if the player touch the upper bound of the screen,generate new level and reset the player's coordinate and update scoring
	if (playerRect.y <= 0) {
		player->setCoordinate(SCREEN_WIDTH / 2, SCREEN_HEIGHT - playerRect.h - 10);
		//if the last road(from bottom to top) is not safe
		if(roadVector[0]->getRoadID() > 0)
			currScore++;
		//Reset score
		lastLevelScore += currScore;
		currScore = 0;
		difficulty++;
		generateNewLevel();
	}
	//Check for collision of player
	bool isCollided = false;
	vector<SDL_Rect> safeObjBoundRectVector;
	for (Road* road : roadVector) {
		road->Update();
		isCollided|=player->checkDangerousCollision(road->getDangerousRoadObjBoundRect());
		vector<SDL_Rect> roadSafeObjBoundRects = road->getSafeRoadObjBoundRect();
		for (SDL_Rect rect : roadSafeObjBoundRects)
			safeObjBoundRectVector.push_back(rect);
	}
	
	player->updateIfDeath();
	player->updateDirection();
	player->updateAnimation();
	player->updateCoordinate(safeObjBoundRectVector);
	//Calculate the score
	int tempScore = 0;
	int playerFootPosY = playerRect.y + playerRect.h;
	int playerCurrRoad = playerFootPosY / roadHeight;//top to bottom
	for (int i = roadVector.size() - 1; i > playerCurrRoad; i--) {
		//if the ith road from top to bottom is not safe
		if (roadVector[i]->getRoadID() > 0) {
			tempScore++;
		}
	}
	currScore = tempScore;
	totalScore = lastLevelScore + currScore;
	scoreTexture->loadFromRenderedText(to_string(totalScore), WHITE, font);
	return !isCollided;
}

void RandomLevelGenerator::Draw() {
	for (Road* road : roadVector) {
		road->Draw();
	}
	player->Draw();
	//render score
	scoreTexture->render(SCREEN_WIDTH/2, 10, NULL, -1, -1);

	if (hasCloud) {
		for (auto p : cloudObj) {
			p->Draw();
		}
	}
}