#pragma once
#include "Texture.h"
#include <vector>
class AnimatingObject {
public:
    AnimatingObject(SDL_Renderer* renderer, vector<LTexture*>& textureFrames, int numFrames, int frameDuration,int x,int y,int width,int height,int speed);

    void Update();
    void Draw();

private:
    int x;
    int y;
    int width;
    int height;
    int speed;
    vector<LTexture*> frames_;
    int numFrames_;
    int frameDuration_;
    int currentFrame_;
    int frameCounter_;
    SDL_Renderer* renderer;
};