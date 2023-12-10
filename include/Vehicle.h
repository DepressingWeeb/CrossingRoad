#pragma once
#include "Texture.h"
#include "Timer.h"
#include <vector>
#include "AnimatingObject.h"
#include "ResourceManager.h"
class NormalVehicle:public AnimatingObject {
public:
    NormalVehicle(SDL_Renderer* renderer, vector<LTexture*> textureFrames, int numFrames, int frameDuration, int x, int y, int width, int height, int speed, double scale = 1.0);
    void Update() override;
    void Draw() override;
    int getYCoordinate() override;
    void setYCoordinate(float y) override;
    SDL_Rect boundingRect();
protected:
    float x;
    float y;
    int width;
    int height;
    float speed;//pixel/s
    LTimer stepTimer;
    vector<LTexture*> frames_;
    int numFrames_;
    int frameDuration_;
    int currentFrame_;
    int frameCounter_;
    SDL_Renderer* renderer;
};

class Train:public AnimatingObject{
public:
    Train(SDL_Renderer* renderer,float y,int height, float speed, double scale = 1.0);
    void Update() override;
    void Draw() override;
    int getYCoordinate() override;
    void setYCoordinate(float y) override;
    void setRedLight(bool redLight);
    SDL_Rect boundingRect();
protected:
    enum class TrainTextureType {
        TRAIN_HEAD,
        TRAIN_PART
    };
    float y;
    int height;
    float speed;//pixel/s
    bool isRedLight;
    vector<pair<SDL_Rect,TrainTextureType>> trainCurrCoord;
    LTimer stepTimer;
    LTexture* trainHeadTexture;
    LTexture* trainPartTexture;
    SDL_Renderer* renderer;
};//TODO
