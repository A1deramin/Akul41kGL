#pragma once
#include <memory>
#include <unordered_set>

#include "Interfaces.h"
using DrawablePtr = std::shared_ptr<Drawable>;

class Scene {
public:
    virtual void AddObject(DrawablePtr object);
    virtual void Draw(CameraPtr camera);

private:
    std::vector<DrawablePtr> m_Objects;
};
