#include "Character.h"
#include "global.h"

Character::Character(SDL_Renderer* renderer, const vector<LTexture*>& textureFrames, int numFrames, int frameDuration, int x, int y, int width, int height, float speed,float levelSpeed) {
	direction = 0;
    frames = vector<vector<LTexture*>>(8, vector<LTexture*>(4));
    for (int i = 0; i < textureFrames.size(); i++) {
        frames[i / 4][i % 4] = textureFrames[i];
    }
	isDeath = false;
    isMoving = false;
	this->x = static_cast<float>(x);
	this->y = static_cast<float>(y);
    if (width == -1 && height == -1) {
        this->width = frames[0][0]->getWidth();
        this->height = frames[0][0]->getHeight();
    }
    else {
        this->width = width;
        this->height = height;
    }
	this->speed = speed;
    this->levelSpeed = levelSpeed;
    this->xChange=new float[8]{0,-speed,-speed,-speed,0,speed,speed,speed};
    this->yChange = new float[8] {speed,speed,0,-speed,-speed,-speed,0,speed};
	this->numFrames_ = numFrames;
	this->frameDuration_ = frameDuration;
	this->currentFrame_ = 0;
	this->frameCounter_ = 0;
	this->renderer = renderer;
}

Character::~Character() {
    delete[] xChange;
    delete[] yChange;
}

bool Character::checkDangerousCollision(const vector<SDL_Rect>& dangerousObjBoundRectVector) {
    SDL_Rect characterRect = { x,y+20,width,height-20 };
    for (SDL_Rect objRect : dangerousObjBoundRectVector) {
        if (SDL_HasIntersection(&characterRect, &objRect)) {
            cout << "Danger Object : " << objRect.x << " " << objRect.y << " " << objRect.w << " " << objRect.h << endl;
            isDeath = true;
            return true;
        }
    }
    return false;
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

void Character::updateAnimation() {
    if (!isMoving) {
        currentFrame_ = 0;
        return;
    }
    //update animation
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % numFrames_;
        frameCounter_ = 0;
    }
}

void Character::updateCoordinate(const vector<SDL_Rect>& safeObjBoundRectVector) {
    float timeStep = stepTimer.getTicks() / 1000.f;//Get the time from last frame to current frame
    y += levelSpeed * timeStep;//Always minus the levelspeed
    stepTimer.start();//reset timer
    if (!isMoving) 
        return;
    //update coordinate
    x += xChange[direction] * timeStep;
    y += yChange[direction] * timeStep;
    
    //If the updated coordinate intersect with any of the safeObj (Obstacles),undo the change
    bool isIntersectSafeObj = false;
    SDL_Rect characterRect = { x,y + 20,width,height-20 };
    for (SDL_Rect objRect : safeObjBoundRectVector) {
        if (SDL_HasIntersection(&characterRect, &objRect)) {
            isIntersectSafeObj = true;
            //cout << "Character : " << characterRect.x << " " << characterRect.y << " " << characterRect.w << " " << characterRect.h << endl;
            //cout << "Object : " << objRect.x << " " << objRect.y << " " << objRect.w << " " << objRect.h << endl;
            break;
        }
    }
    if (isIntersectSafeObj) {
        x -= xChange[direction] * timeStep;
        y -= yChange[direction] * timeStep;
    }
}

void Character::updateIfDeath() {
    if (isDeath) {
        //TODO:
        //Need to update death event in the future
    }
}
void Character::Draw() {
    frames[direction][currentFrame_]->render(x, y, NULL, width, height);
}

SDL_Rect Character::getBoundingRect() {
    return { static_cast<int>(x),static_cast<int>(y),width,height };
}

pair<float, float> Character::getCoordinate() {
    return make_pair(x, y);
}

void Character::setCoordinate(float x, float y) {
    this->x = x;
    this->y = y;
}

void Character::setXCoordinate(float x) {
    this->x = x;
}

void Character::setLevelSpeed(float newSpeed) {
    this->levelSpeed = newSpeed;
}