#include "Vehicle.h"
#include "global.h"
NormalVehicle::NormalVehicle(SDL_Renderer* renderer, vector<LTexture*> textureFrames, int numFrames, int frameDuration, int x, int y, int width, int height, int speed, double scale) {
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
SDL_Rect NormalVehicle::boundingRect() {
    return { static_cast<int>(x) + 10,static_cast<int>(y) + 10,width - 10,height - 10 };//May need to edit to set hit box manually in the future
}
void NormalVehicle::Update() {
    //update animation frames
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % numFrames_;
        frameCounter_ = 0;
    }
    //update coordinate of vehicle
    float timeStep = stepTimer.getTicks() / 1000.f;//the delta time from last frame to current frame in seconds
    x += speed * timeStep;
    //if the vehicle is moving to the right and it passed the right boundary, then reset it to the left
    if (speed > 0 && x > SCREEN_WIDTH)
        x = -frames_[currentFrame_]->getWidth();
    //if the vehicle is moving to the left and it passed the left boundary, then reset it to the right
    if (speed <= 0 && x < -width)
        x = SCREEN_WIDTH;
    stepTimer.start();//reset the timer
}

void NormalVehicle::Draw() {
    if (speed > 0)
        frames_[currentFrame_]->render(x, y, NULL, width, height);
    else
        frames_[currentFrame_]->render(x, y, NULL, width, height, SDL_FLIP_HORIZONTAL);
}

void NormalVehicle::setYCoordinate(float y) {
    this->y = y;
}

int NormalVehicle::getYCoordinate() {
    return y;
}

Monster::Monster(SDL_Renderer* renderer, vector<LTexture*> textureFrames, int numFrames, int frameDuration, int x, int y, int width, int height, int speed, double scale)
    : NormalVehicle(renderer, textureFrames, numFrames, frameDuration, x, y, width, height, speed, scale), targetX(0), targetY(0) {
    // Additional initialization for Monster if needed
}

void Monster::Update(/*Insert Player X and y coordinate here!*/) {
    // Update animation frames
    frameCounter_++;
    if (frameCounter_ >= frameDuration_) {
        currentFrame_ = (currentFrame_ + 1) % numFrames_;
        frameCounter_ = 0;
    }

    // Calculate the direction towards the target position
    float deltaX = targetX - x;
    float deltaY = targetY - y;
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    // Normalize the direction vector
    if (distance > 0) {
        deltaX /= distance;
        deltaY /= distance;
    }

    // Update the vehicle's position based on the direction towards the target
    x += speed * deltaX;
    y += speed * deltaY;

    // Generate random values for playerX and playerY for testing
    int playerX = rand() % SCREEN_WIDTH;  // Assuming SCREEN_WIDTH is the maximum X-coordinate
    int playerY = rand() % SCREEN_HEIGHT; // Assuming SCREEN_HEIGHT is the maximum Y-coordinate

    //The Monster move to a new target!

    SetTargetPosition(playerX, playerY);


    // Additional logic if needed

    // Call the parent class's Update function
    NormalVehicle::Update();
}

void Monster::SetTargetPosition(int targetX, int targetY) {
    this->targetX = targetX;
    this->targetY = targetY;
}

Train::Train(SDL_Renderer* renderer,float y,int height, float speed, double scale) {
    this->renderer = renderer;
    this->speed = speed;
    this->y = y;
    this->height = height;
    ResourceManager& resourceManager = ResourceManager::GetInstance();
    this->trainHeadTexture = resourceManager.GetTexture(ResourceType::Train)[0];
    this->trainPartTexture = resourceManager.GetTexture(ResourceType::Train)[1];
    this->isRedLight = true;
}

int Train::getYCoordinate() {
    return y;
}

void Train::Draw() {
    //render 
    for (int i = 0; i < trainCurrCoord.size(); i++) {
        SDL_Rect partRect = trainCurrCoord[i].first;
        //cout << "After " << partRect.y <<" "<<partRect.y+partRect.h<< endl;
        if(trainCurrCoord[i].second==TrainTextureType::TRAIN_HEAD)
            trainHeadTexture->render(partRect.x, partRect.y, NULL, partRect.w, partRect.h);
        else
            trainPartTexture->render(partRect.x, partRect.y, NULL, partRect.w, partRect.h);
    }
}

void Train::setYCoordinate(float y) {
    this->y = y;
    for (int i = 0; i < trainCurrCoord.size(); i++) {
        trainCurrCoord[i].first.y = this->y;
        //cout<<trainCurrCoord[i].first.y << " " << this->y << endl;
    }
}

void Train::setRedLight(bool redLight) {
    this->isRedLight = redLight;
}

SDL_Rect Train::boundingRect() {
    if (trainCurrCoord.size() == 0)
        return { -9,-9,9,9 };
    SDL_Rect first = trainCurrCoord[0].first;
    SDL_Rect last = trainCurrCoord[trainCurrCoord.size() - 1].first;
    //cout << "Before: " << last.y <<" "<< last.y + first.h << endl;
    return { last.x,last.y+10,first.x + first.w - last.x-10,first.h-10 };
}

void Train::Update() {
    
    float timeStep = stepTimer.getTicks() / 1000.f;
    stepTimer.start();
    for (int i = 0; i < trainCurrCoord.size(); i++) {
        trainCurrCoord[i].first.x += floor(speed * timeStep);
    }
    if (trainCurrCoord.size()>0 && trainCurrCoord[0].first.x > SCREEN_WIDTH) {
        trainCurrCoord.erase(trainCurrCoord.begin());
    }
    if (!isRedLight) {
        if (trainCurrCoord.size() == 0) {
            trainCurrCoord.push_back({{ -trainHeadTexture->getWidth(),static_cast<int>(y),trainHeadTexture->getWidth(),height },TrainTextureType::TRAIN_HEAD});
        }
        else {
            SDL_Rect last = trainCurrCoord[trainCurrCoord.size() - 1].first;
            if (last.x > 0) {
                trainCurrCoord.push_back({ { last.x - trainPartTexture->getWidth(),static_cast<int>(y),trainPartTexture->getWidth(),height },TrainTextureType::TRAIN_PART });
            }
        }
    }
}