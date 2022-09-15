#pragma once
#include "MeshSolidColor.h"

template <class Vertex>
class MeshSolidColorWireframe : public MeshSolidColor<Vertex> {
public:
    using MeshSolidColor<Vertex>::MeshSolidColor;

    MeshSolidColorWireframe(
        MeshSolidColor<Vertex>&& baseMesh, const glm::vec4& lineColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), float lineWidth = 1.0f);

    void ApplyUniforms() override;

    void SetLineColor(const glm::vec4& color) { m_LineColor = color; }
    void SetLineWidth(float lineWidth) { m_LineWidth = lineWidth; }

    glm::vec4 GetLineColor() const { return m_LineColor; }
    float GetLineWidth() const { return m_LineWidth; }

    static EMeshType GetMeshType();

    static EDefaultShader GetDefaultShader();

private:
    glm::vec4 m_LineColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    float m_LineWidth = 1.0f;
};

template <class Vertex>
EDefaultShader MeshSolidColorWireframe<Vertex>::GetDefaultShader() {
    return EDefaultShader::SOLID_COLOR_WIREFRAME;
}

template <class Vertex>
EMeshType MeshSolidColorWireframe<Vertex>::GetMeshType() {
    return EMeshType::MESH_SOLID_COLOR_WIREFRAME;
}

template <class Vertex>
void MeshSolidColorWireframe<Vertex>::ApplyUniforms() {
    MeshSolidColor<Vertex>::ApplyUniforms();
    this->GetShader()->SetUniform4fv("u_LineColor", m_LineColor);
    this->GetShader()->SetUniform1f("u_LineWidth", m_LineWidth);
    int width, height;
    OGLRenderer::GetWindowSize(width, height);
    this->GetShader()->SetUniform1i("u_ScreenHeight", height);
    this->GetShader()->SetUniform1i("u_ScreenWidth", width);
}

template <class Vertex>
MeshSolidColorWireframe<Vertex>::MeshSolidColorWireframe(
    MeshSolidColor<Vertex>&& baseMesh, const glm::vec4& lineColor /*= glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)*/, float lineWidth /*= 1.0f*/)
    : MeshSolidColor<Vertex>(baseMesh) {
    m_LineColor = lineColor;
    m_LineWidth = lineWidth;
}

template <class Vertex>
using MeshSolidColorWireframePtr = std::shared_ptr<MeshSolidColorWireframe<Vertex>>;
