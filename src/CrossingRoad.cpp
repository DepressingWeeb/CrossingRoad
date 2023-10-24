// CrossingRoad.cpp : Defines the entry point for the application.
//

#include "CrossingRoad.h"
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
	vector<LTexture*> frames;
	vector<vector<LTexture*>> framesCharacter(8,vector<LTexture*>(4,nullptr));
	frames.push_back(new LTexture(gRenderer, "../../../resources/vehicle/spacecraft/tile000.png"));
	frames.push_back(new LTexture(gRenderer, "../../../resources/vehicle/spacecraft/tile001.png"));
	frames.push_back(new LTexture(gRenderer, "../../../resources/vehicle/spacecraft/tile002.png"));
	AnimatingObject spacescraft1(gRenderer, frames, frames.size(), 10, 0, 300, -1, -1, 2);
	AnimatingObject spacescraft2(gRenderer, frames, frames.size(), 10, 100, 500, -1, -1, 3);
	AnimatingObject spacescraft3(gRenderer, frames, frames.size(), 10, 500, 100, -1, -1, -2);
	for (int i = 0; i < 32; i++) {
		string path = "../../../resources/character/"+(i<10? "tile00" + to_string(i) + ".png": "tile0" + to_string(i) + ".png");
		framesCharacter[i / 4][i % 4] = new LTexture(gRenderer, path);
	}
	Character player(gRenderer, framesCharacter, frames.size(),10, 300, 610, 32, 32, 1);
	SDL_Texture* lane = IMG_LoadTexture(gRenderer, "../../../resources/pack/Levels/summer_road.png");
	LTexture* newTexture = new LTexture(gRenderer, "../../../resources/vehicle/spacecraft/tile000.png");
	bool quit = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		for (int i = 0; i < 11; i++) {
			rect = { i*100,500,100,100 };
			SDL_RenderCopy(gRenderer, lane, NULL, &rect);
		}
		//newTexture->render(0, 0, NULL, -1, -1);
		spacescraft1.Update();
		spacescraft1.Draw();
		spacescraft2.Update();
		spacescraft2.Draw();
		spacescraft3.Update();
		spacescraft3.Draw();
		vector<AnimatingObject> obj;
		obj.push_back(spacescraft1);
		obj.push_back(spacescraft2);
		obj.push_back(spacescraft3);
		player.checkCollision(obj);
		player.updateAll();
		player.Draw();
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
