#pragma once
#include "MeshUtils.h"
#include "Shape.h"

template <class Vertex>
class Pyramid : public Shape<Vertex> {
public:
    Pyramid(unsigned int n = 3,
        const glm::vec3& position = glm::vec3(0.0f),
        EMeshType meshType = EMeshType::MESH_SOLID_COLOR_WIREFRAME);

    void Regenerate(unsigned int n, float height = 1.0f, float baseRadius = 1.0f);

    unsigned int GetLateralFaces() const { return m_LateralFaces; }
    float GetHeight() const { return m_Height; }
    float GetBaseRadius() const { return m_BaseRadius; }

    void SetHeight(float height);

    void SetBaseRadius(float baseRadius);

private:
    unsigned int m_LateralFaces = 3;
    float m_Height = 1.0f;
    float m_BaseRadius = 1.0f;
};

template <class Vertex>
void Pyramid<Vertex>::SetBaseRadius(float baseRadius) {
    m_BaseRadius = baseRadius;
    auto currentScale = this->GetScale();
    currentScale.x = baseRadius;
    currentScale.z = baseRadius;
    this->SetScale(currentScale);
}

template <class Vertex>
void Pyramid<Vertex>::SetHeight(float height) {
    m_Height = height;
    auto currentScale = this->GetScale();
    currentScale.y = height;
    this->SetScale(currentScale);
}

template <class Vertex>
void Pyramid<Vertex>::Regenerate(unsigned int n,
    float height /*= 1.0f*/,
    float baseRadius /*= 1.0f*/) {
    if (n < 3) {
        n = 3;
    }
    this->GetBaseMesh()->SetGeometry(
        Geometry<Vertex>::GeneratePyramid(n, height, baseRadius));
    m_LateralFaces = n;
}

template <class Vertex>
Pyramid<Vertex>::Pyramid(
    unsigned int n /*= 3*/,
    const glm::vec3& position /*= 0.0f*/,
    EMeshType meshType /*= EMeshType::MESH_SOLID_COLOR_WIREFRAME*/)
    : Shape<Vertex>(MeshUtils::GetMeshByType<Vertex>(
        meshType,
        EBasicGeometry::NONE,
        EDefaultShader::NONE,
        Geometry<Vertex>::GeneratePyramid(n)), position) {
    m_LateralFaces = n;
}

template <class Vertex>
using PyramidPtr = std::shared_ptr<Pyramid<Vertex>>;
