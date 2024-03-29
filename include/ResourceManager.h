#pragma once
#include "SDL.h"
#include "Texture.h"
#include <string>
#include <unordered_map>
//Define the resources for all animating objects
enum class ResourceType {
    Spacecraft = 0,
    Ambulance,
    BlackViper,
    Taxi,
    PoliceCar,
    Train,
    Explosion,
    TrafficLight,
    Bridge,
    Character,
    Archer_IDLE,
    Archer_ATTACK_1,
    Archer_ATTACK_2,
    Archer_ATTACK_3,
    Archer_ARROW,
    Archer_ARROW_EFFECT_1,
    Archer_ARROW_EFFECT_2,
    Archer_ARROW_EFFECT_SPECIAL,
    Archer_ARROW_EFFECT_SHOWER,
    SimpleRoad,
    ConstructionSite,
    SimpleSafeRoad,
    UpperWaterLane,
    LowerWaterLane,
    Railway,
    Cat, Chicken, Fox, Cow, Wolf, Boar,
    AnimalRoad,
    Tree,
    Flower,
    Bush,
    Grass,
    SafeForestRoad,
    MonsterRoad,
    RollingStoneRoad,
    Stone,
    Timber,
    Vendor_1,
    Vendor_2,
    Danger_1,
    Danger_2,
    Cloud
};

const vector<ResourceType> vendorResources = {
    ResourceType::Vendor_1,
    ResourceType::Vendor_2
};

const vector<ResourceType> dangerResources = {
    ResourceType::Danger_1,
    ResourceType::Danger_2
};

const vector<ResourceType> vehicleResources = {
    ResourceType::Spacecraft,
    ResourceType::Ambulance,
    ResourceType::BlackViper,
    ResourceType::Taxi,
    ResourceType::PoliceCar
};

const vector<ResourceType> animalResources = {
     ResourceType::Cat,
     ResourceType::Fox,
     ResourceType::Chicken,
     ResourceType::Cow,
     ResourceType::Boar,
     ResourceType::Wolf
};
//using Singleton pattern for ResourceManager
class ResourceManager {
public:
    static ResourceManager& GetInstance();

    void LoadTexture(SDL_Renderer* gRenderer,ResourceType type, int numTexture,const string& filePath);
    vector<LTexture*> GetTexture(ResourceType type);//Return a vector of LTexture represents all frames of the animation,vector of size 1 for texture with only 1 image
    int getSize();
private:
    unordered_map<ResourceType, vector<LTexture*>> textureMap;

    ResourceManager();
    ~ResourceManager();

    // Disable copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
};

