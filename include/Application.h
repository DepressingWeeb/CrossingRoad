#pragma once
#include "CrossingRoad.h"
#include "road.h"
#include "RandomLevelGenerator.h"
#include "EndlessLevelGenerator.h"
#include "Button.h"
#include <SDL_mixer.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer.h"
#include "imgui/imgui-knobs.h"
#include <fstream>
#include <tuple>
#include "ParallaxBackground.h"
#include "utils.h"
class Application {
private:
	static void game(vector<int>args);
	static void leaderboardScreen();
	static void settingScreen();
	static void creditScreen();
	static void levelScreen();
	static void mainScreen();
public:
	Application();
	void run();
};