#include "RandomLevelGenerator.h"

RandomLevelGenerator::RandomLevelGenerator(int difficulty, int roadHeight, Character* player) {
	
	this->roadHeight = roadHeight;
	this->player = player;
	this->difficulty = difficulty;
	this->level = 0;
	this->lastLevelScore = 0;
	this->currScore = 0;
	font = TTF_OpenFont("../../../resources/Font/ARCADECLASSIC.ttf", 28);
	this->scoreTexture = new LTexture(gRenderer);
	scoreTexture->loadFromRenderedText(to_string(totalScore), WHITE, font);
	generateNewLevel();
}

void RandomLevelGenerator::generateNewLevel() {
	
	//Currently this handles difficulty in a really crude way due to limited number of road types,need to be dealt with in the future
	//May need biased randomization
	roadVector.clear();
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
		switch (roadType)
		{
		case 0:
			roadVector.push_back(new SimpleSafeRoad(i * roadHeight, i * roadHeight + roadHeight));
			isLastRoadSafe = true;
			break;
		case 1:
			numVehicle = sqrt(difficulty + 1)*2;
			newSpeed = baseSpeed * (1.0 + 0.2 * static_cast<float>(difficulty));
			roadVector.push_back(new SimpleRoad(numVehicle, newSpeed, i * roadHeight, i * roadHeight + roadHeight));
			isLastRoadSafe = false;
			break;
		default:
			break;
		}
	}
	//Some last pixels are saved for safe road for the player to respawn safely
	roadVector.push_back(new SimpleSafeRoad(roadVector.size() * roadHeight, SCREEN_HEIGHT));
}

void RandomLevelGenerator::Update() {
	SDL_Rect playerRect = player->getBoundingRect();
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
	for (Road* road : roadVector) {
		road->Update();
		player->checkCollision(road->getRoadObj());
		
	}
	player->updateAll();
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

}

void RandomLevelGenerator::Draw() {
	for (Road* road : roadVector) {
		road->Draw();
	}
	player->Draw();
	//render score
	scoreTexture->render(SCREEN_WIDTH/2, 10, NULL, -1, -1);
}