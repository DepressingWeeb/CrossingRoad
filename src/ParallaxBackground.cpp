#include "ParallaxBackground.h"


ParallaxBackground::ParallaxBackground(SDL_Renderer* renderer, const std::vector<LTexture*>& textures)
    : renderer(renderer), textures(textures) {
    int textureCount = textures.size();
    offsets.resize(textureCount, 0.f);
}

void ParallaxBackground::update(float speed) {
    float timeStep = timer.getTicks() / 1000.f;
    timer.start();
    int textureCount = textures.size();
    for (int i = 0; i < textureCount; ++i) {
        offsets[i] += speed *timeStep* (i + 1);
        if (offsets[i] >SCREEN_WIDTH) {
            offsets[i] = 0.f;
        }
    }
}

void ParallaxBackground::render() {
    int textureCount = textures.size();
    for (int i = 0; i < textureCount; ++i) {
        renderTexture(textures[i], offsets[i], 0);
        renderTexture(textures[i], offsets[i] - SCREEN_WIDTH, 0);
    }
}

void ParallaxBackground::renderTexture(LTexture* texture, float x, float y) {
    texture->render(x, y, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);
}