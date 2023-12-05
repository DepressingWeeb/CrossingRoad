#pragma once
#include "Texture.h"
#include "Timer.h"
#include <vector>
class AnimatingObject {
public:
    virtual void Update()=0;
    virtual void Draw()=0;
    virtual SDL_Rect boundingRect()=0;
    virtual ~AnimatingObject() = 0;
};