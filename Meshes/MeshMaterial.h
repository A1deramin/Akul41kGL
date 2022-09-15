#pragma once
#include "Mesh.h"

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    void ApplyUniforms(Shader& shader) {
        shader.SetUniform3fv("Material.ambient", ambient);
        shader.SetUniform3fv("Material.diffuse", diffuse);
        shader.SetUniform3fv("Material.specular", specular);
        shader.SetUniform1f("Material.shininess", shininess);
    }

};

using MaterialPtr = std::shared_ptr<Material>;

template <class Vertex>
class MeshMaterial : public Mesh<Vertex> {
public:
    using Mesh<Vertex>::Mesh;
    MeshMaterial(Mesh<Vertex>&& baseMesh, MaterialPtr material);

    void ApplyUniforms() override;
    void Update() override;

    static EMeshType GetMeshType() { return EMeshType::MESH_MATERIAL; }
    static EDefaultShader GetDefaultShader() { return EDefaultShader::LIGHTING; }
private:
    MaterialPtr m_Material;
};

template <class Vertex>
MeshMaterial<Vertex>::MeshMaterial(Mesh<Vertex>&& baseMesh, MaterialPtr material)
    : Mesh<Vertex>(baseMesh),
      m_Material(material) {
}

template <class Vertex>
void MeshMaterial<Vertex>::ApplyUniforms() {
    Mesh<Vertex>::ApplyUniforms();
    m_Material->ApplyUniforms(*this->GetShader());
}

template <class Vertex>
void MeshMaterial<Vertex>::Update() {
    Mesh<Vertex>::Update();
}
