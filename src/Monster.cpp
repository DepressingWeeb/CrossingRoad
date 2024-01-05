#include "Monster.h"
NormalArrow::NormalArrow(float x, float y, int width, int height, int xDest, int yDest, int arrowSpeed, float timeFadeOut,ArrowType arrowType) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->xDest = xDest;
    this->YDest = yDest;
    this->arrowSpeed = arrowSpeed;
    this->timeFadeOut = timeFadeOut;
    this->arrowState = ArrowState::START;
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    arrow = resourceManager.GetTexture(ResourceType::Archer_ARROW)[0];
    if(arrowType==ArrowType::ENTANGLE)
        animatingArrow = resourceManager.GetTexture(ResourceType::Archer_ARROW_EFFECT_1);
    else
        animatingArrow = resourceManager.GetTexture(ResourceType::Archer_ARROW_EFFECT_2);
    this->frameCounter_ = 0;
    this->currentFrame_ = 0;
    this->frameDuration_ = 4;
    this->numFrames_ = animatingArrow.size();
}

bool NormalArrow::Update() {
    float timeStep = stepTimer.getTicks() / 1000.f;
    stepTimer.start();

    if (arrowState == ArrowState::MOVING) {
        x += static_cast<float>(arrowSpeed) * timeStep;
        cout << x << endl;
        if (x + width >= xDest) {
            currentFrame_ = 0;
            arrowState = ArrowState::ANIMATING;
        }
    }
    else if (arrowState == ArrowState::ANIMATING) {
        frameCounter_++;
        timeFadeOut -= timeStep;
        if (frameCounter_ >= frameDuration_) {
            currentFrame_ = min(currentFrame_ + 1, numFrames_-1);
            frameCounter_ = 0;

        }
    }
    if (timeFadeOut <= 0) {
        return false;
    }
    return true;
}

void NormalArrow::startMoving() {
    arrowState = ArrowState::MOVING;
}

void NormalArrow::Draw() {
    if (arrowState != ArrowState::ANIMATING) {
        arrow->render(x-100, y-50, NULL, width, height);
        //cout << x << endl;
    }
    else {
        animatingArrow[currentFrame_]->render(x-100, y-50, NULL, width, height);
    }
}

SDL_Rect NormalArrow::boundingRect() {
    return { static_cast<int>(x),static_cast<int>(y),width,height };
}

int NormalArrow::getYCoordinate() {
    return y;
}

void NormalArrow::setYCoordinate(float y) {
    this->y = y;
}

ShowerArrow::ShowerArrow(float x, float y, int width, int height, int xDest, int yDest, int arrowSpeed, float timeFadeOut, ArrowType arrowType) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->xDest = xDest;
    this->YDest = yDest;
    this->arrowSpeed = arrowSpeed;
    this->timeFadeOut = timeFadeOut;
    this->arrowState = ArrowState::START;
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    animatingArrow = resourceManager.GetTexture(ResourceType::Archer_ARROW_EFFECT_SHOWER);
    this->frameCounter_ = 0;
    this->currentFrame_ = 0;
    this->frameDuration_ = 8;
    this->numFrames_ = animatingArrow.size();
}

bool ShowerArrow::Update() {
    float timeStep = stepTimer.getTicks() / 1000.f;
    stepTimer.start();

    if (arrowState == ArrowState::MOVING) {
        x += static_cast<float>(arrowSpeed) * timeStep;
        cout << x << endl;
        if (x + width >= xDest) {
            currentFrame_ = 0;
            arrowState = ArrowState::ANIMATING;
        }
    }
    else if (arrowState == ArrowState::ANIMATING) {
        frameCounter_++;
        timeFadeOut -= timeStep;
        if (frameCounter_ >= frameDuration_) {
            currentFrame_ = min(currentFrame_ + 1, numFrames_ - 1);
            frameCounter_ = 0;

        }
    }
    if (timeFadeOut <= 0) {
        return false;
    }
    return true;
}

void ShowerArrow::startMoving() {
    arrowState = ArrowState::MOVING;
}

void ShowerArrow::Draw() {
    if (arrowState != ArrowState::ANIMATING) {
        //arrow->render(x - 100, y - 50, NULL, width, height);
        //cout << x << endl;
    }
    else {
        animatingArrow[currentFrame_]->render(x - 100, y - 80, NULL, width, height);
    }
}

SDL_Rect ShowerArrow::boundingRect() {
    return { static_cast<int>(x),static_cast<int>(y),width,height };
}

int ShowerArrow::getYCoordinate() {
    return y;
}

void ShowerArrow::setYCoordinate(float y) {
    this->y = y;
}

Monster::Monster(SDL_Renderer* renderer, int x, int y, int width, int height, float timeIdle, float timeArrow, int arrowSpeed) {
    this->renderer = renderer;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->startedAttack = false;
    this->arrowSpeed = arrowSpeed;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();
    generator = std::mt19937_64(nanoseconds);
    distribution = std::uniform_int_distribution<int>(0, INT_MAX);
    ResourceManager& resourceManager = resourceManager.GetInstance();
    frames_.push_back(resourceManager.GetTexture(ResourceType::Archer_IDLE));
    frames_.push_back(resourceManager.GetTexture(ResourceType::Archer_ATTACK_1));
    frames_.push_back(resourceManager.GetTexture(ResourceType::Archer_ATTACK_2));
    frames_.push_back(resourceManager.GetTexture(ResourceType::Archer_ATTACK_2));
    this->frameDuration_ = 4;
    this->frameCounter_ = 0;
    this->currentFrame_ = 0;
    this->timeIdle = timeIdle;
    this->timeArrowAppear = timeArrow;
    this->timeSinceLastChange = 0;
    monsterState = MonsterState::IDLE;
}

int Monster::getYCoordinate() {
    return y;
}

void Monster::setYCoordinate(float y) {
    float deltaY = y - this->y;
    this->y = y;
    for (Arrow* arrow : arrowVector) {
        float currY = arrow->getYCoordinate();
        arrow->setYCoordinate(currY + deltaY);
    }
}

SDL_Rect Monster::boundingRect() {
    return { static_cast<int>(x),static_cast<int>(y),width,height };
}

void Monster::Update() {
    float timeStep = stepTimer.getTicks() / 1000.f;
    stepTimer.start();
    timeSinceLastChange += timeStep;
    if (monsterState == MonsterState::IDLE) {
        if (timeSinceLastChange >= timeIdle) {
            timeSinceLastChange -= timeIdle;
            int rnd = (distribution(generator)%3)+1;
            monsterState = static_cast<MonsterState>(rnd);
            currentFrame_ = 0;
        }
    }
    else {
        if (!startedAttack) {
            //append arrow to arrowVector
            if (monsterState == MonsterState::ATTACK_1) {
                arrowVector.push_back(new NormalArrow(x + 50, y, 256, 128, 700, y, arrowSpeed, timeArrowAppear, ArrowType::POISON));
                arrowVector.push_back(new NormalArrow(x + 50, y + 30, 256, 128, 900, y + 30, arrowSpeed, timeArrowAppear));
                arrowVector.push_back(new NormalArrow(x + 50, y + 60, 256, 128, 1100, y + 60, arrowSpeed, timeArrowAppear));
            }
            else {
                arrowVector.push_back(new ShowerArrow(x + 50, y, 256, 128, 700, y, arrowSpeed, timeArrowAppear, ArrowType::POISON));
                arrowVector.push_back(new ShowerArrow(x + 50, y + 30, 256, 128, 900, y + 30, arrowSpeed, timeArrowAppear));
                arrowVector.push_back(new ShowerArrow(x + 50, y + 60, 256, 128, 1100, y + 60, arrowSpeed, timeArrowAppear));
            }
            startedAttack = true;
        }
        else {
            
        }

    }
    if (arrowVector.size() != 0) {
        bool updated = false;
        for (auto& arrow : arrowVector) {
            updated |= arrow->Update();
        }
        if (!updated) {
            startedAttack = false;
            timeSinceLastChange = 0;
            arrowVector.clear();
        }
    }
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % frames_[static_cast<int>(monsterState)].size();
        frameCounter_ = 0;
        //if done animation attack
        if (currentFrame_ == 0 && monsterState != MonsterState::IDLE) {
            monsterState = MonsterState::IDLE;
            for (int i = 0; i < arrowVector.size(); i++) {
                arrowVector[i]->startMoving();
            }
        }
    }
}

void Monster::Draw() {
    frames_[static_cast<int>(monsterState)][currentFrame_]->render(x-100, y-60, NULL, width, height);
    for (int i = 0; i < arrowVector.size(); i++) {
        arrowVector[i]->Draw();
    }
}