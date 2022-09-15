#pragma once
#include <functional>
#include <utility>
#include <vector>

#include "Camera.h"
#include "Geometry.h"
#include "IndexBuffer.h"
#include "Interfaces.h"
#include "OGLRenderer.h"
#include "Shader.h"
#include "Utils/Profile.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

enum class EMeshType { MESH, MESH_SOLID_COLOR, MESH_SOLID_COLOR_WIREFRAME, MESH_MATERIAL, MESH_VERTEX_LIGHTING };

template <class Vertex>
class Mesh : public Drawable {
public:
    Mesh(EBasicGeometry geometry, EDefaultShader shader);
    Mesh(Geometry<Vertex> geometry, EDefaultShader shader);

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
        const VertexBufferLayout& layout = Vertex::GenerateLayout());
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, EDefaultShader shaderType,
        const VertexBufferLayout& layout = Vertex::GenerateLayout());
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::string& shaderPath,
        const VertexBufferLayout& layout = Vertex::GenerateLayout());
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, ShaderPtr shader,
        const VertexBufferLayout& layout = Vertex::GenerateLayout());

    void Draw(CameraPtr Camera) override;

    ShaderPtr GetShader() const;

    Geometry<Vertex> GetGeometry() const { return {m_Vertices, m_Indices}; }

    void SetGeometry(const Geometry<Vertex>& geometry);

    virtual void Update() override;

    virtual void ApplyUniforms();

    static EMeshType GetMeshType();
    static EDefaultShader GetDefaultShader();


private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

    std::function<void()> m_UpdateMethod;

    VertexArray<Vertex> m_VertexArray;
    VertexBuffer<Vertex> m_VertexBuffer;
    IndexBuffer m_IndexBuffer;

protected:
    ShaderPtr m_Shader;
}; // class Mesh

template <class Vertex>
void Mesh<Vertex>::SetGeometry(const Geometry<Vertex>& geometry) {
    m_Vertices = geometry.GetVertices();
    m_Indices = geometry.GetIndices();
    m_VertexBuffer.SetData(m_Vertices);
    m_IndexBuffer.SetData(m_Indices);
    m_VertexArray.AddBuffer(m_VertexBuffer, Vertex::GenerateLayout());
}

template <class Vertex>
EDefaultShader Mesh<Vertex>::GetDefaultShader() {
    return EDefaultShader::DEFAULT;
}

template <class Vertex>
EMeshType Mesh<Vertex>::GetMeshType() {
    return EMeshType::MESH;
}

template <class Vertex>
Mesh<Vertex>::Mesh(EBasicGeometry geometry, EDefaultShader shader)
    : Mesh(Geometry<Vertex>(EBasicGeometry::CUBE), shader) {
}

template <class Vertex>
Mesh<Vertex>::Mesh(Geometry<Vertex> geometry, EDefaultShader shader)
    : Mesh(geometry.GetVertices(), geometry.GetIndices(), shader) {
}

template <class Vertex>
void Mesh<Vertex>::Update() {
}

template <class Vertex>
Mesh<Vertex>::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const VertexBufferLayout& layout)
    : Mesh(vertices, indices, Shader::GetDefaultShader(EDefaultShader::DEFAULT)) {
}

template <class Vertex>
Mesh<Vertex>::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, EDefaultShader shaderType,
    const VertexBufferLayout& layout)
    : Mesh(vertices, indices, Shader::GetDefaultShader(shaderType)) {
}

template <class Vertex>
Mesh<Vertex>::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::string& shaderPath,
    const VertexBufferLayout& layout)
    : Mesh<Vertex>(vertices, indices, std::make_shared<Shader>(shaderPath)) {
}

template <class Vertex>
Mesh<Vertex>::Mesh(
    const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, ShaderPtr shader, const VertexBufferLayout& layout) {
    m_Vertices = vertices;
    m_Indices = indices;
    m_VertexArray.Bind();

    m_VertexBuffer = VertexBuffer<Vertex>(m_Vertices);
    m_IndexBuffer = IndexBuffer(m_Indices);

    m_VertexArray.AddBuffer(m_VertexBuffer, layout);
    m_Shader = std::move(shader);
}

template <class Vertex>
void Mesh<Vertex>::Draw(CameraPtr camera) {
    m_Shader->Bind();
    m_VertexArray.Bind();

    ApplyUniforms();

    camera->Update(*m_Shader);

    OGLRenderer::Draw(m_VertexArray, m_IndexBuffer, *m_Shader);
}

template <class Vertex>
ShaderPtr Mesh<Vertex>::GetShader() const {
    m_Shader->Bind();
    return m_Shader;
}

template <class Vertex>
void Mesh<Vertex>::ApplyUniforms() {
}

template <class Vertex>
using MeshPtr = std::shared_ptr<Mesh<Vertex>>;
