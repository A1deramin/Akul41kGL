#pragma once
#include "MeshUtils.h"
#include "Shape.h"

template <class Vertex>
class Parallelepiped : public Shape<Vertex> {
public:
    Parallelepiped(const glm::vec3& location,
        EMeshType meshType = EMeshType::MESH_SOLID_COLOR_WIREFRAME);
    Parallelepiped(const glm::vec3& location,
        float length,
        float width,
        float height,
        EMeshType meshType = EMeshType::MESH_SOLID_COLOR_WIREFRAME);

    float GetLength() const { return m_Length; }
    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }

    void SetLength(float newLength);
    void SetWidth(float newWidth);
    void SetHeight(float newHeight);

private:
    float m_Length = 1.0f;
    float m_Width = 1.0f;
    float m_Height = 1.0f;
};

template <class Vertex>
void Parallelepiped<Vertex>::SetHeight(float newHeight) {
    m_Height = newHeight;
    auto currentScale = this->GetScale();
    currentScale.z = newHeight;
    this->SetScale(currentScale);
}

template <class Vertex>
void Parallelepiped<Vertex>::SetWidth(float newWidth) {
    m_Width = newWidth;
    auto currentScale = this->GetScale();
    currentScale.y = newWidth;
    this->SetScale(currentScale);
}

template <class Vertex>
void Parallelepiped<Vertex>::SetLength(float newLength) {
    m_Length = newLength;
    auto currentScale = this->GetScale();
    currentScale.x = newLength;
    this->SetScale(currentScale);
}

template <class Vertex>
Parallelepiped<Vertex>::Parallelepiped(const glm::vec3& location,
    float length,
    float width,
    float height,
    EMeshType meshType)
    : Parallelepiped(location, meshType) {
    m_Length = length;
    m_Width = width;
    m_Height = height;
    this->SetScale({length, width, height});
}

template <class Vertex>
Parallelepiped<Vertex>::Parallelepiped(const glm::vec3& location,
    EMeshType meshType)
    : Shape<Vertex>(MeshUtils::GetMeshByType<Vertex>(meshType, EBasicGeometry::CUBE), location) {
}

template <class Vertex>
using ParallelepipedPtr = std::shared_ptr<Parallelepiped<Vertex>>;
