#include "Character.h"
#include "global.h"

Character::Character(SDL_Renderer* renderer, vector<vector<LTexture*>>& textureFrames, int numFrames, int frameDuration, int x, int y, int width, int height, int speed) {
	direction = 0;
	frames = textureFrames;
	isDeath = false;
    isMoving = false;
	this->x = x;
	this->y = y;
    if (width == -1 && height == -1) {
        this->width = textureFrames[0][0]->getWidth();
        this->height = textureFrames[0][0]->getHeight();
    }
    else {
        this->width = width;
        this->height = height;
    }
	this->speed = speed;
    this->xChange=new int[8]{0,-speed,-speed,-speed,0,speed,speed,speed};
    this->yChange = new int[8] {speed,speed,0,-speed,-speed,-speed,0,speed};
	this->numFrames_ = numFrames;
	this->frameDuration_ = frameDuration;
	this->currentFrame_ = 0;
	this->frameCounter_ = 0;
	this->renderer = renderer;
}
void Character::checkCollision(const vector<AnimatingObject>& v) {
    SDL_Rect characterRect = { x,y,width,height };
    for (auto obj : v) {
        SDL_Rect objRect = obj.boundingRect();
        if (SDL_HasIntersection(&characterRect, &objRect)) {
            isDeath = true;
            break;
        }
    }
}
void Character::updateDirection() {
    int currDir = -1;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_UP]&& currentKeyStates[SDL_SCANCODE_LEFT])
    {
        currDir = 3;
    }
    else if (currentKeyStates[SDL_SCANCODE_UP] && currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        currDir = 5;
    }
    else if (currentKeyStates[SDL_SCANCODE_DOWN] && currentKeyStates[SDL_SCANCODE_LEFT])
    {
        currDir = 1;
    }
    else if (currentKeyStates[SDL_SCANCODE_DOWN] && currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        currDir = 7;
    }
    else if (currentKeyStates[SDL_SCANCODE_UP])
    {
        currDir = 4;
    }
    else if (currentKeyStates[SDL_SCANCODE_DOWN])
    {
        currDir = 0;
    }
    else if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        currDir = 2;
    }
    else if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        currDir = 6;
    }
    if (currDir == -1) {
        isMoving = false;
    }
    else {
        isMoving = true;
        direction = currDir;
    }
}

void Character::updateCoordinate() {
    
    if (!isMoving) { 
        currentFrame_ = 0;
        return; 
    }
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % numFrames_;
        frameCounter_ = 0;
    }
    x += xChange[direction];
    y += yChange[direction];
    
}

void Character::updateIfDeath() {
    if (isDeath) {
        isDeath = false;
        x = 500;
        y = 610;
    }
}
void Character::updateAll() {
    updateIfDeath();
    updateDirection();//check for keyboard event and update direction accordingly
    updateCoordinate();//change the frame and update coordinate of character
}
void Character::Draw() {
    frames[direction][currentFrame_]->render(x, y, NULL, width, height);
}