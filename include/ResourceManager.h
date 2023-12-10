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
    Character,
    SimpleRoad,
    SimpleSafeRoad,
    Railway
};

const vector<ResourceType> vehicleResources = {
    ResourceType::Spacecraft,
    ResourceType::Ambulance,
    ResourceType::BlackViper,
    ResourceType::Taxi,
    ResourceType::PoliceCar
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

