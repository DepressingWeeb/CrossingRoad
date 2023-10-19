#include "AnimatingObject.h"
#include "global.h"
AnimatingObject::AnimatingObject(SDL_Renderer* renderer, vector<LTexture*>& textureFrames, int numFrames, int frameDuration,int x,int y,int width,int height,int speed) {
    this->renderer = renderer;
    frames_ = textureFrames;
    numFrames_ = numFrames;
    frameDuration_ = frameDuration;
    currentFrame_ = 0;
    frameCounter_ = 0;
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->width = width;
    this->height = height;
    if (width == -1) {
        this->width = textureFrames[0]->getWidth();
        this->height = textureFrames[0]->getHeight();
    }
}

void AnimatingObject::Update() {
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % numFrames_;
        frameCounter_ = 0;
    }
    x += speed;
    if (speed > 0 && x > SCREEN_WIDTH)
        x = -frames_[currentFrame_]->getWidth();
    if (speed <= 0 && x < -width)
        x = SCREEN_WIDTH;
}

void AnimatingObject::Draw() {
    if(speed>0)
        frames_[currentFrame_]->render(x, y,NULL,width,height);
    else
        frames_[currentFrame_]->render(x, y, NULL, width, height,SDL_FLIP_HORIZONTAL);
}