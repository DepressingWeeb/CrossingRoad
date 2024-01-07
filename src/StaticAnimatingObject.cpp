#include "StaticAnimatingObject.h"

StaticAnimatingObject::StaticAnimatingObject(SDL_Renderer* renderer, vector<LTexture*> textureFrames, int numFrames, int frameDuration, int x, int y, int width, int height, double scale) {
    this->renderer = renderer;
    frames_ = textureFrames;
    numFrames_ = numFrames;
    frameDuration_ = frameDuration;
    currentFrame_ = 0;
    frameCounter_ = 0;
    this->x = x;
    this->y = y;
    this->width = static_cast<double>(width) * scale;
    this->height = static_cast<double>(height) * scale;
    if (width == -1) {
        this->width = static_cast<double>(textureFrames[0]->getWidth()) * scale;
        this->height = static_cast<double>(textureFrames[0]->getHeight()) * scale;
    }
}
SDL_Rect StaticAnimatingObject::boundingRect() {
    return { static_cast<int>(x),static_cast<int>(y),width,height };
}
void StaticAnimatingObject::Update() {
    //update animation frames
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % numFrames_;
        frameCounter_ = 0;
    }
}

void StaticAnimatingObject::Draw() {
    frames_[currentFrame_]->render(x, y, NULL, width, height);
}

void StaticAnimatingObject::setYCoordinate(float y) {
    this->y = y;
}

int StaticAnimatingObject::getYCoordinate() {
    return y;
}
