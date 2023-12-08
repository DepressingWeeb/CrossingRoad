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

int ResourceManager::getSize() {
    return textureMap.size();
}

void ResourceManager::LoadTexture(SDL_Renderer* gRenderer,ResourceType type,int numTexture, const string& filePath) {
    vector<LTexture*> v;
    for (int i = 0; i < numTexture; i++) {
        string path = filePath + (i < 10 ? "tile00" + to_string(i) + ".png" : "tile0" + to_string(i) + ".png");
        v.push_back(new LTexture(gRenderer, path));
    }
    if (textureMap.find(type) != textureMap.end()) {
        for (LTexture* texture : textureMap[type]) {
            delete texture;
        }
    }
    textureMap[type] = v;
}

vector<LTexture*> ResourceManager::GetTexture(ResourceType type) {
    auto it = textureMap.find(type);
    return it->second;
}