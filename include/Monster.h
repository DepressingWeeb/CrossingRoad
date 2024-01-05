#pragma once

#include "Texture.h"
#include "Timer.h"
#include <vector>
#include "ResourceManager.h"
#include "Character.h"
#include "global.h"
#include <random>
#include <chrono>
enum class ArrowType {
    ENTANGLE=0,
    POISON
};
class Arrow {
public:
    virtual bool Update() = 0;
    virtual void Draw() = 0;
    virtual int getYCoordinate() = 0;
    virtual void setYCoordinate(float y) = 0;
    virtual void startMoving() = 0;
    virtual SDL_Rect boundingRect() = 0;
};
class NormalArrow: public Arrow{
private:
    float x;
    float y;
    int xDest;
    int yDest;
    int width;
    int height;
    int arrowSpeed;
    enum class ArrowState {
        START=0,
        MOVING,
        ANIMATING
    } arrowState;
    LTexture* arrow;
    vector<LTexture*> animatingArrow;
    int numFrames_;
    int frameDuration_;
    int currentFrame_;
    int frameCounter_;
    float timeFadeOut;
    LTimer stepTimer;
    SDL_Renderer* renderer;
public:
    NormalArrow(SDL_Renderer* renderer,float x, float y, int width, int height, int xDest,int yDest,int arrowSpeed = 2000,float timeFadeOut=1.5f,ArrowType arrowType=ArrowType::ENTANGLE);
    bool Update();
    void Draw() ;
    int getYCoordinate();
    void setYCoordinate(float y);
    void startMoving();
    SDL_Rect boundingRect();
};

class ShowerArrow:public Arrow {
private:
    float x;
    float y;
    int xDest;
    int yDest;
    int width;
    int height;
    int arrowSpeed;
    enum class ArrowState {
        START = 0,
        MOVING,
        ANIMATING
    } arrowState;
    vector<LTexture*> animatingArrow;
    int numFrames_;
    int frameDuration_;
    int currentFrame_;
    int frameCounter_;
    float timeFadeOut;
    LTimer stepTimer;
    SDL_Renderer* renderer;
public:
    ShowerArrow(SDL_Renderer* renderer, float x, float y, int width, int height, int xDest, int yDest, int arrowSpeed = 2000, float timeFadeOut = 1.5f);
    bool Update();
    void Draw();
    int getYCoordinate();
    void setYCoordinate(float y);
    void startMoving();
    SDL_Rect boundingRect();
};

class Beam :public Arrow {
private:
    float x;
    float y;
    int width;
    int height;
    bool done;
    enum class ArrowState {
        START = 0,
        ANIMATING
    } arrowState;
    vector<LTexture*> animatingArrow;
    int numFrames_;
    int frameDuration_;
    int currentFrame_;
    int frameCounter_;
    float timeFadeOut;
    LTimer stepTimer;
    SDL_Renderer* renderer;
public:
    Beam(SDL_Renderer* renderer, float x, float y, int width, int height,int frameDurations);
    bool Update();
    void Draw();
    int getYCoordinate();
    void setYCoordinate(float y);
    void startMoving();
    SDL_Rect boundingRect();
};

class Monster {
private:
    float x;
    float y;
    int width;
    int height;
    enum class MonsterState {
        IDLE=0,
        ATTACK_1,
        ATTACK_2,
        ATTACK_3,
        LAST
    } monsterState;
    Character* player;
    LTimer stepTimer;
    vector<vector<LTexture*>> frames_;
    int numFrames_;
    int frameDuration_;
    int currentFrame_;
    int frameCounter_;
    float timeIdle;
    float timeArrowAppear;
    float timeSinceLastChange;
    bool startedAttack;
    int arrowSpeed;
    vector<Arrow*> arrowVector;
    SDL_Renderer* renderer;
    std::mt19937_64 generator;//random engine
    std::uniform_int_distribution<int> distribution;
public:
    Monster(SDL_Renderer* renderer, int x, int y, int width, int height,float timeIdle,float timeArrow,int arrowSpeed);
    void Update();
    void Draw();
    int getYCoordinate();
    void setYCoordinate(float y);
    SDL_Rect boundingRect();
    vector<SDL_Rect> arrowBoundingRect();
};

