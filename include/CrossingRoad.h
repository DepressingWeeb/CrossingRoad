﻿// CrossingRoad.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "global.h"
#include "Texture.h"
#include "AnimatingObject.h"
#include "Character.h"
#include "ResourceManager.h"

void mainScreen();
void levelScreen();
void settingScreen();
void creditScreen();
void leaderboardScreen();
void play(int level,int mode);
// TODO: Reference additional headers your program requires here.
