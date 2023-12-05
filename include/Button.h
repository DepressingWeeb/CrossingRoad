#pragma once
#include "Texture.h"
#include "global.h"
class Button {
private:
	bool isOnHover;
	LTexture* normal;
	LTexture* onHover;
	SDL_Rect destRect;
	void* funcIfClicked;
	vector<int> args;
	int alwaysActiveState;
public:
	Button(const string& pathToNormalButton, const string& pathToButtonHover,SDL_Rect destRect,void* func,vector<int> args=vector<int>());
	~Button();
	void Update(bool* quit=nullptr);
	void Draw();
	void setHoverState(bool newState);
	void setAlwaysActiveState(int state);
};