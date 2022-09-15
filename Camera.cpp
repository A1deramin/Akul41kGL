#include "Camera.h"

#include "OGLRenderer.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(int width, int height, const glm::vec3& position)
    : m_Position(position),
      m_Width(width),
      m_Height(height) {
}

void Camera::Update(Shader& shader) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);
    view = glm::lookAt(m_Position, m_Position + m_Orientation, m_UpVector);
    proj = glm::perspective(glm::radians(m_FOV), (float)(m_Width / m_Height),
        m_NearPlane, m_FarPlane);
    shader.SetUniformMat4f("u_Proj", proj);
    shader.SetUniformMat4f("u_View", view);
}

void Camera::Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_Position += m_Speed * m_Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_Position +=
            m_Speed * -glm::normalize(glm::cross(m_Orientation, m_UpVector));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_Position += m_Speed * -m_Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_Position +=
            m_Speed * glm::normalize(glm::cross(m_Orientation, m_UpVector));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_Position += m_Speed * m_UpVector;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        m_Position += m_Speed * -m_UpVector;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        m_Speed *= 1.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        m_Speed /= 1.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        m_Speed = m_SpeedBase;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        m_RightMouseFirstClick = !m_RightMouseFirstClick;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        OGLRenderer::SetPolygonMode(EPolygonMode::FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        OGLRenderer::SetPolygonMode(EPolygonMode::LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        OGLRenderer::SetPolygonMode(EPolygonMode::POINT);
    }

    if (m_RightMouseFirstClick) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        float rotX =
            m_Sensitivity * (float)(mouseY - (m_Height / 2)) / m_Height;
        float rotY = m_Sensitivity * (float)(mouseX - (m_Width / 2)) / m_Width;
        // Calculates upcoming vertical change in the Orientation
        glm::vec3 newOrientation =
            glm::rotate(m_Orientation, glm::radians(-rotX),
                glm::normalize(glm::cross(m_Orientation, m_UpVector)));

        // Decides whether or not the next vertical Orientation is legal or not
        if (abs(glm::angle(newOrientation, m_UpVector) - glm::radians(90.0f)) <=
            glm::radians(85.0f)) {
            m_Orientation = newOrientation;
        }

        // Rotates the Orientation left and right
        m_Orientation =
            glm::rotate(m_Orientation, glm::radians(-rotY), m_UpVector);

        // Sets mouse cursor to the middle of the screen so that it doesn't end
        // up roaming around
        glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
