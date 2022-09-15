#pragma once
#include <glad/glad.h>
#include <memory>
#include <vector>
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"

struct VertexBase {
    glm::vec3 position;
    VertexBase() = default;

    VertexBase(glm::vec3 pos)
        : position(pos) {
    }

    static VertexBufferLayout GenerateLayout() {
        VertexBufferLayout layout;
        layout.Push<glm::vec3>(1);
        return layout;
    }
};

struct VertexNormal : public VertexBase {
    glm::vec3 normal;

    static VertexBufferLayout GenerateLayout() {
        VertexBufferLayout layout = VertexBase::GenerateLayout();
        layout.Push<glm::vec3>(1);
        return layout;
    }
};

struct VertexNormalColor : public VertexNormal {
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    static VertexBufferLayout GenerateLayout() {
        VertexBufferLayout layout = VertexNormal::GenerateLayout();
        layout.Push<glm::vec4>(1);
        return layout;
    }
};

struct VertexColor : public VertexBase {
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    VertexColor() = default;

    VertexColor(glm::vec3 position)
        : VertexBase(position) {
    }

    VertexColor(glm::vec3 position, glm::vec4 clr)
        : VertexBase(position),
          color(clr) {
    }

    static VertexBufferLayout GenerateLayout() {
        VertexBufferLayout layout = VertexBase::GenerateLayout();
        layout.Push<glm::vec4>(1);
        return layout;
    }
};

struct VertexColorNormal : VertexColor {
    glm::vec3 normal;

    static VertexBufferLayout GenerateLayout() {
        VertexBufferLayout layout = VertexColor::GenerateLayout();
        layout.Push<glm::vec3>(1);
        return layout;
    }
};

struct VertexNormalTexture : public VertexBase {
    glm::vec3 normal;
    glm::vec2 texUV;

    static VertexBufferLayout GenerateLayout() {
        VertexBufferLayout layout = VertexBase::GenerateLayout();
        layout.Push<glm::vec3>(1);
        layout.Push<glm::vec2>(1);
        return layout;
    }
};

struct VertexColorNormalTexture : public VertexColorNormal {
    glm::vec2 texUV;

    static VertexBufferLayout GenerateLayout() {
        VertexBufferLayout layout = VertexColorNormal::GenerateLayout();
        layout.Push<glm::vec2>(1);
        return layout;
    }
};

template <class Vertex>
class VertexBuffer {
public:
    // Reference ID of the Vertex Buffer Object
    GLuint m_ID;
    // Ctor that generates a Vertex Buffer Object and links it to vertices
    VertexBuffer(GLfloat* vertices, GLsizeiptr size);

    VertexBuffer(const std::vector<Vertex>& vertices);

    VertexBuffer();

    void SetData(const std::vector<Vertex>& vertices);

    void Bind() const;
    void UnBind() const;
    void Delete() const;
};

template <class Vertex>
VertexBuffer<Vertex>::VertexBuffer() {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

template <class Vertex>
void VertexBuffer<Vertex>::SetData(const std::vector<Vertex>& vertices) {
    Bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
        vertices.data(), GL_DYNAMIC_DRAW);
}

// Ctor that generates a Vertex Buffer Object and links it to vertices
template <class Vertex>
VertexBuffer<Vertex>::VertexBuffer(GLfloat* vertices, GLsizeiptr size) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

template <class Vertex>
VertexBuffer<Vertex>::VertexBuffer(const std::vector<Vertex>& vertices) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
        vertices.data(), GL_DYNAMIC_DRAW);
}

template <class Vertex>
void VertexBuffer<Vertex>::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

template <class Vertex>
void VertexBuffer<Vertex>::UnBind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <class Vertex>
void VertexBuffer<Vertex>::Delete() const {
    glDeleteBuffers(1, &m_ID);
}
