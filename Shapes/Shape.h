#pragma once
#include <glm/glm.hpp>

#include "Interfaces.h"
#include "Mesh.h"
#include "MeshUtils.h"
#include "glm/gtx/string_cast.hpp"

struct Transform {
    glm::vec3 location;
    glm::vec3 rotation;
    glm::vec3 scale;
};

template <class Vertex>
class Shape : public Drawable {
public:
    Shape(MeshPtr<Vertex> mesh, const glm::vec3& location = glm::vec3(0.0f));
    Shape(MeshPtr<Vertex> mesh, const Transform& transform);
    Shape(EMeshType meshType = EMeshType::MESH_SOLID_COLOR, const glm::vec3& location = glm::vec3(0.0f),
        EBasicGeometry geometry = EBasicGeometry::CUBE, EDefaultShader shader = EDefaultShader::NONE);
    virtual void Draw(CameraPtr Camera) override;
    virtual void Update() override;
    void SetLocation(const glm::vec3& newLocation);
    void AddLocation(const glm::vec3& deltaLocation);
    void AddScale(const glm::vec3& scale);
    void SetScale(const glm::vec3& scale);
    void AddRotation(float degree, glm::vec3 rotationAxis);
    void SetRotation(float degree, glm::vec3 rotationAxis);

    glm::vec3 GetLocation() const { return m_TranslationMatrix[3]; }
    glm::mat4 GetRotationMatrix() const { return m_RotationMatrix; }
    glm::vec3 GetScale() const { return {m_ScaleMatrix[0][0], m_ScaleMatrix[1][1], m_ScaleMatrix[2][2]}; }

    void SetUpdateMethod(const std::function<void()>& updateMethod);

    void SetMesh(MeshPtr<Vertex> mesh);

    MeshPtr<Vertex> GetBaseMesh() const { return m_Mesh; }

    template <template <class> class MeshClass>
    std::shared_ptr<MeshClass<Vertex>> GetMesh() const {
        return std::dynamic_pointer_cast<MeshClass<Vertex>>(m_Mesh);
    }

private:
    MeshPtr<Vertex> m_Mesh;

    glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
    glm::mat4 m_RotationMatrix = glm::mat4(1.0f);
    glm::mat4 m_TranslationMatrix = glm::mat4(1.0f);
    glm::mat4 m_ScaleMatrix = glm::mat4(1.0f);

    std::function<void()> m_UpdateMethod;

    void ApplyModelMatrix();
};


template <class Vertex>
void Shape<Vertex>::SetMesh(MeshPtr<Vertex> mesh) {
    m_Mesh = mesh;
}

template <class Vertex>
Shape<Vertex>::Shape(EMeshType meshType /*= EMeshType::MESH_SOLID_COLOR*/, const glm::vec3& location /*= glm::vec3(0.0f)*/,
    EBasicGeometry geometry /*= EBasicGeometry::CUBE*/, EDefaultShader shader /*= EDefaultShader::NONE*/)
    : Shape<Vertex>(MeshUtils::GetMeshByType<Vertex>(meshType, geometry, shader), location) {
}

template <class Vertex>
Shape<Vertex>::Shape(MeshPtr<Vertex> mesh, const Transform& transform)
    : Shape(mesh, transform.location) {
    SetRotation(transform.rotation);
    SetScale(transform.scale);
}

template <class Vertex>
void Shape<Vertex>::SetUpdateMethod(const std::function<void()>& updateMethod) {
    m_UpdateMethod = updateMethod;
}

template <class Vertex>
void Shape<Vertex>::SetRotation(float degree, glm::vec3 rotationAxis) {
    m_RotationMatrix = glm::mat4(1.0f);
    AddRotation(degree, rotationAxis);
}

template <class Vertex>
void Shape<Vertex>::AddRotation(float degree, glm::vec3 rotationAxis) {
    m_RotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(degree), rotationAxis);
}

template <class Vertex>
void Shape<Vertex>::SetScale(const glm::vec3& scale) {

    m_ScaleMatrix = glm::mat4(1.0f);
    AddScale(scale);
}

template <class Vertex>
void Shape<Vertex>::AddScale(const glm::vec3& scale) {
    m_ScaleMatrix = glm::scale(m_ScaleMatrix, scale);
}

template <class Vertex>

void Shape<Vertex>::Update() {
    m_Mesh->Update();

    if (m_UpdateMethod) {
        m_UpdateMethod();
    }
}

template <class Vertex>
Shape<Vertex>::Shape(MeshPtr<Vertex> mesh, const glm::vec3& location /*= glm::vec3(0.0f)*/)
    : m_Mesh(mesh) {
    SetLocation(location);
}

template <class Vertex>
void Shape<Vertex>::Draw(CameraPtr Camera) {
    ApplyModelMatrix();
    m_Mesh->Draw(Camera);
}

template <class Vertex>
void Shape<Vertex>::SetLocation(const glm::vec3& newLocation) {
    m_TranslationMatrix = glm::mat4(1.0f);
    AddLocation(newLocation);
}

template <class Vertex>
void Shape<Vertex>::AddLocation(const glm::vec3& deltaLocation) {
    m_TranslationMatrix = glm::translate(m_TranslationMatrix, deltaLocation);
}

template <class Vertex>
void Shape<Vertex>::ApplyModelMatrix() {
    m_ModelMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
    m_Mesh->GetShader()->SetUniformMat4f("u_Model", m_ModelMatrix);
}

template <class Vertex>
using ShapePtr = std::shared_ptr<class Shape<Vertex>>;
