#pragma once
#include "Shape.h"

template <class Vertex>
class Prism : public Shape<Vertex> {
public:
    Prism(unsigned int n = 3, const glm::vec3& position = glm::vec3(0.0f), EMeshType meshType = EMeshType::MESH_SOLID_COLOR_WIREFRAME);

    void Regenerate(unsigned int n, float height = 1.0f, float baseRadius = 1.0f);

    unsigned int GetLateralFaces() const { return m_LateralFaces; }
    float GetBaseRadius() const { return m_BaseRadius; }
    float GetHeight() const { return m_Height; }

    void SetHeight(float height);
    void SetBaseRadius(float baseRadius);

private:
    unsigned int m_LateralFaces = 3;
    float m_Height = 1.0f;
    float m_BaseRadius = 1.0f;
};

template <class Vertex>
void Prism<Vertex>::SetBaseRadius(float baseRadius) {
    m_BaseRadius = baseRadius;
    auto currentScale = this->GetScale();
    currentScale.x = baseRadius;
    currentScale.y = baseRadius;
    this->SetScale(currentScale);
}

template <class Vertex>
void Prism<Vertex>::SetHeight(float height) {
    m_Height = height;
    auto currentScale = this->GetScale();
    currentScale.y = height;
    this->SetScale(currentScale);
}

template <class Vertex>
void Prism<Vertex>::Regenerate(unsigned int n, float height /*= 1.0f*/, float baseRadius /*= 1.0f*/) {
    if (n <= 3) {
        n = 3;
    }
    this->GetMesh()->SetGeometry(Geometry<Vertex>::GeneratePrism(n, height, baseRadius));
    m_LateralFaces = n;
}

template <class Vertex>
Prism<Vertex>::Prism(
    unsigned int n /*= 3*/, const glm::vec3& position /*= glm::vec3(0.0f)*/, EMeshType meshType /*= EMeshType::MESH_SOLID_COLOR_WIREFRAME*/)
    : Shape<Vertex>(
        MeshUtils::GetMeshByType<Vertex>(meshType, EBasicGeometry::NONE, EDefaultShader::NONE, Geometry<Vertex>::GeneratePrism(n)),
        position) {
    m_LateralFaces = n;
}

template <class Vertex>
using PrismPtr = std::shared_ptr<Prism<Vertex>>;
