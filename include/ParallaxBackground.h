#pragma once
#include "global.h"
#include "Texture.h"
#include "Timer.h"
#include <vector>

class ParallaxBackground {
public:
    ParallaxBackground(SDL_Renderer* renderer, const std::vector<LTexture*>& textures);

    void update(float speed);
    void render();

private:
    void renderTexture(LTexture* texture, float x, float y);
    LTimer timer;
    SDL_Renderer* renderer;
    std::vector<LTexture*> textures;
    std::vector<float> offsets;
};
