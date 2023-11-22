#pragma once
#include "SDL.h"
#include "Texture.h"
#include <string>
#include <unordered_map>
enum class ResourceType {
    Spacecraft=0,
    Ambulance,
    BlackViper,
    Taxi,
    PoliceCar,
    Last //Just the last element for the purpose of randomization,not belongs to any type of resources
};
class ResourceManager {
public:
    static ResourceManager& GetInstance();

    void LoadTexture(SDL_Renderer* gRenderer,ResourceType type, int numTexture,const string& filePath);
    vector<LTexture*> GetTexture(ResourceType type);

private:
    unordered_map<ResourceType, vector<LTexture*>> textureMap;

    ResourceManager();
    ~ResourceManager();

    // Disable copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
};

