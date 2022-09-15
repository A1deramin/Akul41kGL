#pragma once
#include <vector>

#include "Camera.h"
#include "Shader.h"
class Drawable {
   public:
    virtual ~Drawable() = default;
    virtual void Draw(CameraPtr camera) = 0;
    virtual void Update() = 0;
};
