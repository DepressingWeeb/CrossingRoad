#pragma once
#include "SDL.h"
#include "Texture.h"
#include <string>
#include <unordered_map>
enum class ResourceType {
    Spacecraft = 0,
    Ambulance,
    BlackViper,
    Taxi,
    PoliceCar
};

const vector<ResourceType> vehicleResources = {
    ResourceType::Spacecraft,
    ResourceType::Ambulance,
    ResourceType::BlackViper,
    ResourceType::Taxi,
    ResourceType::PoliceCar
};

class ResourceManager {
public:
    static ResourceManager& GetInstance();

    void LoadTexture(SDL_Renderer* gRenderer,ResourceType type, int numTexture,const string& filePath);
    vector<LTexture*> GetTexture(ResourceType type);
    int getSize();
private:
    unordered_map<ResourceType, vector<LTexture*>> textureMap;

    ResourceManager();
    ~ResourceManager();

    // Disable copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
};

