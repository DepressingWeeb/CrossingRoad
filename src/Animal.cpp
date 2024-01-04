#include "Animal.h"
NormalAnimal::NormalAnimal(SDL_Renderer* renderer, vector<LTexture*> textureFrames, int numFrames, int frameDuration, int x, int y, int width, int height, int speed, double scale) {
    this->renderer = renderer;
    frames_ = textureFrames;
    numFrames_ = numFrames;
    frameDuration_ = frameDuration;
    currentFrame_ = 0;
    frameCounter_ = 0;
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->width = static_cast<double>(width) * scale;
    this->height = static_cast<double>(height) * scale;
    if (width == -1) {
        this->width = static_cast<double>(textureFrames[0]->getWidth()) * scale;
        this->height = static_cast<double>(textureFrames[0]->getHeight()) * scale;
    }
}
SDL_Rect NormalAnimal::boundingRect() {
    return { static_cast<int>(x) + 10,static_cast<int>(y) + 10,width - 10,height - 10 };//May need to edit to set hit box manually in the future
}
void NormalAnimal::Update() {
    //update animation frames
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % numFrames_;
        frameCounter_ = 0;
    }
    //update coordinate of animal
    float timeStep = stepTimer.getTicks() / 1000.f;//the delta time from last frame to current frame in seconds
    x += speed * timeStep;
    //if the animal is moving to the right and it passed the right boundary, then reset it to the left
    if (speed > 0 && x > SCREEN_WIDTH)
        x = -frames_[currentFrame_]->getWidth();
    //if the animal is moving to the left and it passed the left boundary, then reset it to the right
    if (speed <= 0 && x < -width)
        x = SCREEN_WIDTH;
    stepTimer.start();//reset the timer
}

void NormalAnimal::Draw() {
    if (speed > 0)
        frames_[currentFrame_]->render(x, y, NULL, width, height);
    else
        frames_[currentFrame_]->render(x, y, NULL, width, height, SDL_FLIP_HORIZONTAL);
}

void NormalAnimal::setYCoordinate(float y) {
    this->y = y;
}

int NormalAnimal::getYCoordinate() {
    return y;
}