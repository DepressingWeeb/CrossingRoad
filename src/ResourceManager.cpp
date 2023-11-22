#include "ResourceManager.h"

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
    // Clean up resources
    for (auto& pair : textureMap) {
        pair.second.clear();
    }
}

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::LoadTexture(SDL_Renderer* gRenderer,ResourceType type,int numTexture, const string& filePath) {
    vector<LTexture*> v;
    for (int i = 0; i < numTexture; i++) {
        string path = filePath + (i < 10 ? "tile00" + to_string(i) + ".png" : "tile0" + to_string(i) + ".png");
        v.push_back(new LTexture(gRenderer, path));
    }
    textureMap[type] = v;
}

vector<LTexture*> ResourceManager::GetTexture(ResourceType type) {
    auto it = textureMap.find(type);
    return it->second;
}