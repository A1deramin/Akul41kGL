#pragma once
#include "Mesh.h"


template <class Vertex>
class MeshVertexLit : public Mesh<Vertex> {
public:
    using Mesh<Vertex>::Mesh;
    MeshVertexLit(Mesh<Vertex>&& baseMesh, const glm::vec3& lightPos);
    void ApplyUniforms() override;
    void SetLightPosition(const glm::vec3& pos);
    static EMeshType GetMeshType() { return EMeshType::MESH_VERTEX_LIGHTING; }
    static EDefaultShader GetDefaultShader() { return EDefaultShader::VERTEX_LIGHTING; }
private:
    glm::vec3 m_LightPos;
};

template <class Vertex>
MeshVertexLit<Vertex>::MeshVertexLit(Mesh<Vertex>&& baseMesh, const glm::vec3& lightPos)
    : Mesh<Vertex>(baseMesh),
      m_LightPos(lightPos) {
}

template <class Vertex>
void MeshVertexLit<Vertex>::ApplyUniforms() {
    Mesh<Vertex>::ApplyUniforms();
    this->GetShader()->SetUniform3fv("u_LightPos", m_LightPos);
}

template <class Vertex>
void MeshVertexLit<Vertex>::SetLightPosition(const glm::vec3& pos) {
    m_LightPos = pos;

}
