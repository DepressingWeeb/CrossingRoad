#pragma once
#include "ResourceManager.h"
#include "global.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer.h"
#include <tuple>
#include <fstream>
bool cmp(tuple<string, string, int, string, string> t1, tuple<string, string, int, string, string> t2);
void quitGame();
void init();
void saveScore(string mode, int score, string timeFormatted, string terrainType);
void loadResource();
void highlightRect(SDL_Rect rectHighlight);