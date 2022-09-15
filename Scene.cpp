#include "Scene.h"

#include "Profile.h"

void Scene::Draw(CameraPtr camera) {
    for (const auto& DrawablePtr : m_Objects) {
        DrawablePtr->Update();
        DrawablePtr->Draw(camera);
    }
}

void Scene::AddObject(DrawablePtr object) { m_Objects.push_back(object); }
