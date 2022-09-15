#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"
using CameraPtr = std::shared_ptr<class Camera>;

class Camera {
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_UpVector = glm::vec3(0.0f, 1.0f, 0.0f);

    int m_Width = 800;
    int m_Height = 800;

    float m_FOV = 45.0f;
    float m_NearPlane = 0.1f;
    float m_FarPlane = 30000.f;

    float m_SpeedBase = 0.005f;
    float m_Speed = m_SpeedBase;
    float m_Sensitivity = 30.f;

    bool m_RightMouseFirstClick = false;

public:
    Camera() = default;
    Camera(int width, int height, const glm::vec3& pos);

    void Update(Shader& shader);
    void Inputs(GLFWwindow* window);

    glm::vec3 GetPosition() const {
        return m_Position;
    }

    void SetPosition(const glm::vec3& position) {
        m_Position = position;
    }

    glm::vec3 GetOrientation() const {
        return m_Orientation;
    }

    void SetOrientation(const glm::vec3& orientation) {
        m_Orientation = orientation;
    }
};
