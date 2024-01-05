#include "Monster.h"
NormalArrow::NormalArrow(SDL_Renderer* renderer, float x, float y, int width, int height, int xDest, int yDest, int arrowSpeed, float timeFadeOut,ArrowType arrowType) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->xDest = xDest;
    this->yDest = yDest;
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
    this->renderer = renderer;
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
    //SDL_SetRenderDrawColor(renderer, 200, 0, 0, 100);
    //SDL_Rect dRect = { xDest - 275,yDest , 125,50 };
    //SDL_RenderFillRect(renderer, &dRect);
    if (arrowState != ArrowState::ANIMATING) {
        //SDL_Rect aRect = { static_cast<int>(x)+10,static_cast<int>(y)+10,30,5 };
        //SDL_RenderFillRect(renderer, &aRect);
        arrow->render(x-100, y-50, NULL, width, height);
        //cout << x << endl;
    }
    else {
        animatingArrow[currentFrame_]->render(x-100, y-50, NULL, width, height);
    }
}

SDL_Rect NormalArrow::boundingRect() {
    if (arrowState == ArrowState::ANIMATING) {
        SDL_Rect dRect = { xDest - 250,yDest , 100,50 };
        return dRect;
    }
    else {
        return { static_cast<int>(x) + 10,static_cast<int>(y) + 10,30,5 };
    }
}

int NormalArrow::getYCoordinate() {
    return y;
}

void NormalArrow::setYCoordinate(float y) {
    this->yDest += (y - this->y);
    this->y = y;
}

ShowerArrow::ShowerArrow(SDL_Renderer* renderer, float x, float y, int width, int height, int xDest, int yDest, int arrowSpeed, float timeFadeOut) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->xDest = xDest;
    this->yDest = yDest;
    this->arrowSpeed = arrowSpeed;
    this->timeFadeOut = timeFadeOut;
    this->arrowState = ArrowState::START;
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    animatingArrow = resourceManager.GetTexture(ResourceType::Archer_ARROW_EFFECT_SHOWER);
    this->frameCounter_ = 0;
    this->currentFrame_ = 0;
    this->frameDuration_ = 8;
    this->numFrames_ = animatingArrow.size();
    this->renderer = renderer;
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
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 100);
    SDL_Rect dRect = { xDest - 275,yDest + 25, 100,50 };
    SDL_RenderFillRect(renderer, &dRect);
    if (arrowState != ArrowState::ANIMATING) {
        //arrow->render(x - 100, y - 50, NULL, width, height);
        //cout << x << endl;
    }
    else {
        
        animatingArrow[currentFrame_]->render(x - 100, y - 80, NULL, width, height);

    }
}

SDL_Rect ShowerArrow::boundingRect() {
    if (arrowState != ArrowState::ANIMATING) {
        return { -1,-1,0,0 };
    }
    else {
        SDL_Rect dRect = { xDest - 275,yDest + 25, 100,50 };
        return dRect;
    }
}

int ShowerArrow::getYCoordinate() {
    return y;
}

void ShowerArrow::setYCoordinate(float y) {
    this->yDest += (y-this->y);
    this->y = y;
}

Beam::Beam(SDL_Renderer* renderer, float x, float y, int width, int height,int frameDurations) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->arrowState = ArrowState::START;
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    animatingArrow = resourceManager.GetTexture(ResourceType::Archer_ARROW_EFFECT_SPECIAL);
    this->frameCounter_ = 0;
    this->currentFrame_ = 0;
    this->numFrames_ = animatingArrow.size();
    this->renderer = renderer;
    this->frameDuration_ = frameDurations;
    this->done = false;
}

void Beam::Draw() {
    if (arrowState == ArrowState::ANIMATING) {
        animatingArrow[currentFrame_]->render(x, y-25, NULL, width, height);
    }
}

int Beam::getYCoordinate() {
    return y;
}

void Beam::setYCoordinate(float y) {
    this->y = y;
}

void Beam::startMoving() {
    cout << "toggle beam moving" << endl;
    this->arrowState = ArrowState::ANIMATING;
}

SDL_Rect Beam::boundingRect() {
    if (arrowState == ArrowState::ANIMATING) {
        switch (currentFrame_) {
        case 0:
            return { static_cast<int>(x),static_cast<int>(y) + 30,width,19 };
        case 1:
            return { static_cast<int>(x),static_cast<int>(y) + 35,width,19-10 };
        case 2:
            return { static_cast<int>(x),static_cast<int>(y) + 38,width,19 - 16 };
        case 3:
            return { static_cast<int>(x),static_cast<int>(y) + 39,width,19 - 18 };
        default:
            return { -1,-1,0,0 };
        }
    }
    return { -1,-1,0,0 };
}

bool Beam::Update() {
    if (arrowState == ArrowState::ANIMATING) {
        frameCounter_++;
        if (frameCounter_ >= frameDuration_) {
            currentFrame_ = min(currentFrame_ + 1, numFrames_ - 1);
            frameCounter_ = 0;

        }
    }
    if (currentFrame_ == numFrames_ - 1)
        done = true;
    return !done;
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
    frames_.push_back(resourceManager.GetTexture(ResourceType::Archer_ATTACK_3));
    this->frameDuration_ = 8;
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
    SDL_Rect archerRect = { static_cast<int>(x) + 25,static_cast<int>(y) + 25,35,35 };
    return archerRect;
}

vector<SDL_Rect> Monster::arrowBoundingRect() {
    vector<SDL_Rect> ans;
    for (Arrow* arrow : arrowVector) {
        ans.push_back(arrow->boundingRect());
    }
    return ans;
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
        //x:400 to 1500
        if (!startedAttack) {
            //append arrow to arrowVector
            if (monsterState == MonsterState::ATTACK_1) {
                arrowVector.push_back(new NormalArrow(renderer,x + 50, y, 256, 128, 400, y, arrowSpeed, timeArrowAppear, ArrowType::POISON));
                arrowVector.push_back(new NormalArrow(renderer,x + 50, y + 30, 256, 128, 900, y + 30, arrowSpeed, timeArrowAppear));
                arrowVector.push_back(new NormalArrow(renderer,x + 50, y + 60, 256, 128, 1300, y + 60, arrowSpeed, timeArrowAppear));
            }
            else if(monsterState==MonsterState::ATTACK_2) {
                arrowVector.push_back(new ShowerArrow(renderer,x + 50, y, 256, 128, 400, y, arrowSpeed, timeArrowAppear));
                arrowVector.push_back(new ShowerArrow(renderer,x + 50, y + 30, 256, 128, 900, y + 30, arrowSpeed, timeArrowAppear));
                arrowVector.push_back(new ShowerArrow(renderer,x + 50, y + 60, 256, 128, 1300, y + 60, arrowSpeed, timeArrowAppear));
                
            }
            else {
                for(int i=0;i<SCREEN_WIDTH;i+=256)
                    arrowVector.push_back(new Beam(renderer, x + 50+i, y, 256, 128, frameDuration_));
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
            cout << "Curr frame : " << currentFrame_ <<" "<<(monsterState==MonsterState::IDLE?"IDLE":"ATK")<<endl;
            monsterState = MonsterState::IDLE;
        }
    }
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % frames_[static_cast<int>(monsterState)].size();
        frameCounter_ = 0;
        //if done animation attack
        if (currentFrame_ == 0 && (monsterState ==MonsterState::ATTACK_1 || monsterState==MonsterState::ATTACK_2)) {
            monsterState = MonsterState::IDLE;
            for (int i = 0; i < arrowVector.size(); i++) {
                arrowVector[i]->startMoving();
            }
        }
        else if (monsterState == MonsterState::ATTACK_3) {
            if (currentFrame_ == 9) {
                for (int i = 0; i < arrowVector.size(); i++) {
                    arrowVector[i]->startMoving();
                }
            }
            if (currentFrame_ == 0) {
                cout << "IDLEEEEEEEEEEEEEEEEEEEEE" << endl;
                monsterState == MonsterState::IDLE;
                
            }
        }
    }
}

void Monster::Draw() {
    frames_[static_cast<int>(monsterState)][currentFrame_]->render(x-100, y-60, NULL, width, height);
    for (int i = 0; i < arrowVector.size(); i++) {
        arrowVector[i]->Draw();
    }
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 100);
    SDL_Rect dRect = { static_cast<int>(x)+25,static_cast<int>(y)+25,35,35 };
    SDL_RenderFillRect(renderer, &dRect);
}