#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <stack>
#include <vector>
#define RED {255,0,0}
#define BLUE {0,0,255}
#define GREEN {0,255,0}
#define BLACK {0,0,0}
#define WHITE {255,255,255}
#define YELLOW {255,255,0}
//typedef void (*FunctionPtr)();
using namespace std;
struct VoidFunction {
	void* func;
	vector<int> args;
	VoidFunction(void* func, const vector<int>& args = vector<int>()) {
		this->args = args;
		this->func = func;
	}
};
struct GlobalSetting {
	int volume;
	SDL_KeyCode up;
	SDL_KeyCode down;
	SDL_KeyCode left;
	SDL_KeyCode right;
	GlobalSetting() {
		volume = 50;
		up = SDLK_w;
		down = SDLK_s;
		left = SDLK_a;
		right = SDLK_d;
	}
	GlobalSetting &operator=(const GlobalSetting& other) {
		if (this != &other) {
			volume = other.volume;
			up = other.up;
			down = other.down;
			left = other.left;
			right = other.right;
		}
		return *this;
	}
	bool operator!=(const GlobalSetting& other) {
		return volume != other.volume || up != other.up || down != other.down || left != other.left || right != other.right;
	}
};
extern GlobalSetting globalSetting;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const SDL_Rect SCREEN;
extern SDL_Window* window;
extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;
extern stack<VoidFunction> functionStack;

