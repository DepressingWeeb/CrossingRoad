// CrossingRoad.cpp : Defines the entry point for the application.
//

#include "CrossingRoad.h"
#include "road.h"
#include "RandomLevelGenerator.h"
using namespace std;
void quitGame() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(gRenderer);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(0);
}
void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	IMG_Init(IMG_INIT_JPG);
	TTF_Init();
}
int main(int argn, char** argv)
{
	init();
	SDL_Event e;
	SDL_Rect rect;
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	vector<vector<LTexture*>> framesCharacter(8,vector<LTexture*>(4,nullptr));
	resourceManager.LoadTexture(gRenderer,ResourceType::Spacecraft, 3, "../../../resources/vehicle/spacecraft/");
	resourceManager.LoadTexture(gRenderer,ResourceType::Taxi, 1, "../../../resources/vehicle/taxi/");
	resourceManager.LoadTexture(gRenderer,ResourceType::BlackViper, 1, "../../../resources/vehicle/blackviper/");
	resourceManager.LoadTexture(gRenderer,ResourceType::Ambulance, 3, "../../../resources/vehicle/ambulance/");
	resourceManager.LoadTexture(gRenderer, ResourceType::PoliceCar, 3, "../../../resources/vehicle/police/");
	for (int i = 0; i < 32; i++) {
		string path = "../../../resources/character/"+(i<10? "tile00" + to_string(i) + ".png": "tile0" + to_string(i) + ".png");
		framesCharacter[i / 4][i % 4] = new LTexture(gRenderer, path);
	}
	Character player(gRenderer, framesCharacter, 4,10, 300, 610, 32, 32, 1);
	RandomLevelGenerator levelGenerator(2, 100, &player);
	bool quit = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		levelGenerator.Update();
		levelGenerator.Draw();
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			}
		}
		
		SDL_RenderPresent(gRenderer);
	}
	return 0;
}
