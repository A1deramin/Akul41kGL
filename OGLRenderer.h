#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "glad/glad.h"

enum class EPolygonMode { FILL, LINE, POINT };


namespace {
GLFWwindow* GWindow = nullptr;
}


class OGLRenderer {
    GLFWwindow* m_Window{};
    int m_Width;
    int m_Height;
public:
    OGLRenderer() = default;

    explicit OGLRenderer(const int width = 800, const int height = 800)
        : m_Width(width),
          m_Height(height) { Init(width, height); }

    static void Finalize() {
    };

    ~OGLRenderer() {
        Finalize();
        glfwTerminate();
    }

    static void Draw(unsigned int indices) {
        glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
    }

    template <class Vertex>
    static void Draw(const VertexArray<Vertex>& va, const IndexBuffer& ib, const Shader& shader) {
        shader.Bind();
        va.Bind();
        ib.Bind();
        glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    GLFWwindow* GetWindow() const { return m_Window; }

    static void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

    static void SetPolygonMode(EPolygonMode mode) {
        switch (mode) {
            case EPolygonMode::FILL: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            case EPolygonMode::LINE: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case EPolygonMode::POINT: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                break;
        }
    };

    static void GetWindowSize(int& outWidth, int& outHeight) {
        if (GWindow) {
            glfwGetWindowSize(GWindow, &outWidth, &outHeight);
        }
    }

    GLFWwindow* Init(int width, int height) {
        // Initialize GLFW
        glfwInit();

        // Tell GLFW which version of OpenGL we are using
        // In this case we are using OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        // Tell GLFW we are using the CORE profile
        // We don't use deprecated functions.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window object
        m_Window = glfwCreateWindow(width, height, "win", NULL, NULL);
        GWindow = m_Window;
        if (!m_Window) {
            std::cout << "Failed to create window" << std::endl;
            glfwTerminate();
            return nullptr;
        }
        // Introduce the window into the current context
        glfwMakeContextCurrent(m_Window);

        // GLAD configures OpenGL
        gladLoadGL();

        // Specify the viewport of OpenGL in the Window
        glViewport(0, 0, m_Width, m_Height);

        ///////////////////////////////////////////////////////////////
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        return m_Window;
    }
};
