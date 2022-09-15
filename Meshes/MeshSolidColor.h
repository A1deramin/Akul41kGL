#pragma once
#include "Mesh.h"

template <class Vertex>
class MeshSolidColor : public Mesh<Vertex> {
public:
    using Mesh<Vertex>::Mesh;

    MeshSolidColor(Mesh<Vertex>&& baseMesh, const glm::vec4& color);

    void ApplyUniforms() override;
    void Update() override;
    void SetColor(const glm::vec4& color) { m_Color = color; }

    static EMeshType GetMeshType();
    static EDefaultShader GetDefaultShader();

    glm::vec4& GetColorRef();

private:
    glm::vec4 m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

template <class Vertex>
EDefaultShader MeshSolidColor<Vertex>::GetDefaultShader() {
    return EDefaultShader::SOLID_COLOR;
}

template <class Vertex>
EMeshType MeshSolidColor<Vertex>::GetMeshType() {
    return EMeshType::MESH_SOLID_COLOR;
}

template <class Vertex>
MeshSolidColor<Vertex>::MeshSolidColor(Mesh<Vertex>&& baseMesh, const glm::vec4& color)
    : Mesh<Vertex>(baseMesh) {
    m_Color = color;
}

template <class Vertex>
void MeshSolidColor<Vertex>::Update() {
    Mesh<Vertex>::Update();
}

template <class Vertex>
glm::vec4& MeshSolidColor<Vertex>::GetColorRef() {
    return m_Color;
}

template <class Vertex>
void MeshSolidColor<Vertex>::ApplyUniforms() {
    Mesh<Vertex>::ApplyUniforms();
    this->GetShader()->SetUniform4fv("u_Color", m_Color);
}

template <class Vertex>
using MeshSolidColorPtr = std::shared_ptr<MeshSolidColor<Vertex>>;
