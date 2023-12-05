#include "Character.h"
#include "global.h"

Character::Character(SDL_Renderer* renderer, const vector<vector<LTexture*>>& textureFrames, int numFrames, int frameDuration, int x, int y, int width, int height, float speed) {
	direction = 0;
	frames = textureFrames;
	isDeath = false;
    isMoving = false;
	this->x = static_cast<float>(x);
	this->y = static_cast<float>(y);
    if (width == -1 && height == -1) {
        this->width = textureFrames[0][0]->getWidth();
        this->height = textureFrames[0][0]->getHeight();
    }
    else {
        this->width = width;
        this->height = height;
    }
	this->speed = static_cast<float>(speed);
    this->xChange=new float[8]{0,-speed,-speed,-speed,0,speed,speed,speed};
    this->yChange = new float[8] {speed,speed,0,-speed,-speed,-speed,0,speed};
	this->numFrames_ = numFrames;
	this->frameDuration_ = frameDuration;
	this->currentFrame_ = 0;
	this->frameCounter_ = 0;
	this->renderer = renderer;
}
void Character::checkCollision(const vector<AnimatingObject*> v) {
    SDL_Rect characterRect = { x,y+20,width,height };
    for (auto obj : v) {
        SDL_Rect objRect = obj->boundingRect();
        if (SDL_HasIntersection(&characterRect, &objRect)) {
            isDeath = true;
            break;
        }
    }
}
void Character::updateDirection() {
    int currDir = -1;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    SDL_Scancode up = SDL_GetScancodeFromKey(globalSetting.up);
    SDL_Scancode down = SDL_GetScancodeFromKey(globalSetting.down);
    SDL_Scancode left = SDL_GetScancodeFromKey(globalSetting.left);
    SDL_Scancode right = SDL_GetScancodeFromKey(globalSetting.right);
    if (currentKeyStates[up]&& currentKeyStates[left])
    {
        currDir = 3;
    }
    else if (currentKeyStates[up] && currentKeyStates[right])
    {
        currDir = 5;
    }
    else if (currentKeyStates[down] && currentKeyStates[left])
    {
        currDir = 1;
    }
    else if (currentKeyStates[down] && currentKeyStates[right])
    {
        currDir = 7;
    }
    else if (currentKeyStates[up])
    {
        currDir = 4;
    }
    else if (currentKeyStates[down])
    {
        currDir = 0;
    }
    else if (currentKeyStates[left])
    {
        currDir = 2;
    }
    else if (currentKeyStates[right])
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
        stepTimer.start();
        return; 
    }
    
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % numFrames_;
        frameCounter_ = 0;
    }
    float timeStep = stepTimer.getTicks() / 1000.f;
    x += xChange[direction] * timeStep;
    y += yChange[direction] * timeStep;
    stepTimer.start();
    
}

void Character::updateIfDeath() {
    if (isDeath) {
        isDeath = false;
        x = SCREEN_WIDTH/2;
        y = SCREEN_HEIGHT-height-10;
        //TODO:
        //Need to update death event in the future
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

SDL_Rect Character::getBoundingRect() {
    return { static_cast<int>(x),static_cast<int>(y),width,height };
}

void Character::setCoordinate(int x, int y) {
    this->x = x;
    this->y = y;
}