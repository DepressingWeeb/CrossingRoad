#include "Button.h"
//Note: this Button can only handle void function with int parameters when clicked
Button::Button(const string& pathToNormalButton, const string& pathToButtonHover, SDL_Rect destRect, void* func,vector<int>args) {
	this->destRect = destRect;
	this->normal = new LTexture(gRenderer, pathToNormalButton);
	this->onHover = new LTexture(gRenderer, pathToButtonHover);
	this->isOnHover = false;
	this->funcIfClicked = func;
	this->args = args;
	this->alwaysActiveState = -1;
}
Button::~Button() {
	delete normal;
	delete onHover;
	funcIfClicked = nullptr;
}
void Button::Update(bool* quit) {
	SDL_Point mousePos;
	bool clicked = SDL_GetMouseState(&mousePos.x, &mousePos.y) & SDL_BUTTON_LMASK;
	SDL_Rect collisionRect;
	if (destRect.w == -1) {
		collisionRect = { destRect.x,destRect.y,normal->getWidth(),normal->getHeight() };
	}
	else {
		collisionRect = destRect;
	}
	if (SDL_PointInRect(&mousePos,&collisionRect)) {
		if (clicked && funcIfClicked != nullptr) {
			if(quit)
				*quit = true;
			functionStack.push(VoidFunction(funcIfClicked,args));
		}
		else {
			isOnHover = true;
		}
	}
	else {
		isOnHover = false;
	}
}

void Button::Draw() {
	if (alwaysActiveState == -1) {
		if (isOnHover) {
			onHover->render(destRect.x, destRect.y, nullptr, destRect.w, destRect.h);
		}
		else {
			normal->render(destRect.x, destRect.y, nullptr, destRect.w, destRect.h);
		}
	}
	else {
		if (alwaysActiveState == 0)
			normal->render(destRect.x, destRect.y, nullptr, destRect.w, destRect.h);
		else
			onHover->render(destRect.x, destRect.y, nullptr, destRect.w, destRect.h);
	}
}

void Button::setHoverState(bool newState) {
	isOnHover = newState;
}

void Button::setAlwaysActiveState(int state) {
	alwaysActiveState = state;
}