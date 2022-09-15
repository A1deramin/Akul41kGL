#pragma once
#include "Shape.h"

template <class Vertex>
class Sphere : public Shape<Vertex> {
public:
    Sphere(float radius = 1.0f, const glm::vec3& position = glm::vec3(0.0f), EMeshType meshType = EMeshType::MESH_SOLID_COLOR_WIREFRAME);

    void Regenerate(float radius, unsigned int sectorCount, unsigned int stackCount);

    float GetRadius() const { return m_Radius; }

    void SetRadius(float radius);

    unsigned int GetSectorCount() const { return m_SectorCount; }
    unsigned int GetStackCount() const { return m_StackCount; }

private:
    float m_Radius = 1.0f;
    unsigned int m_SectorCount = 50;
    unsigned int m_StackCount = 50;
};

template <class Vertex>
void Sphere<Vertex>::SetRadius(float radius) {
    m_Radius = radius;
    this->SetScale(glm::vec3(radius, radius, radius));
}

template <class Vertex>
void Sphere<Vertex>::Regenerate(float radius, unsigned int sectorCount, unsigned int stackCount) {
    if (sectorCount < 3) {
        sectorCount = 3;
    }
    if (stackCount < 2) {
        stackCount = 2;
    }
    this->GetBaseMesh()->SetGeometry(Geometry<Vertex>::GenerateSphere(radius, sectorCount, stackCount));
    m_Radius = radius;
    m_SectorCount = stackCount;
    m_StackCount = stackCount;
}

template <class Vertex>
Sphere<Vertex>::Sphere(float radius, const glm::vec3& position, EMeshType meshType)
    : Shape<Vertex>(
        MeshUtils::GetMeshByType<Vertex>(meshType, EBasicGeometry::NONE, EDefaultShader::NONE,
            Geometry<Vertex>::GenerateSphere(radius)),
        position) {
    m_Radius = radius;
}

template <class Vertex>
using SpherePtr = std::shared_ptr<Sphere<Vertex>>;
