#pragma once
#include "Texture.h"
#include "Timer.h"
#include <vector>
class AnimatingObject {
public:
    virtual void Update()=0;//update the object for every frame
    virtual void Draw()=0;//render to screen
    virtual void setYCoordinate(float y) = 0;//set the y of the object
    virtual int getYCoordinate() = 0;//get the y of the object
    virtual SDL_Rect boundingRect()=0;//return the bound rect(x,y,width,height) of the object
    virtual void toFile(std::ofstream& outputFile);
    virtual ~AnimatingObject()=0;
};

//TODOFOREST: create classes represent animals or monters, have to inherit from AnimatingObject class. See vehicle.h and .cpp for example