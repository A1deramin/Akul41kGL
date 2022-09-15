#pragma once
#include <glad/glad.h>
#include <vector>

struct VertexBufferElement {
  unsigned int count;
  unsigned int type;
  unsigned char normalized;

  static unsigned int GetSize(unsigned int type) {
    switch (type) {
      case GL_FLOAT:
        return sizeof(GLfloat);
      case GL_UNSIGNED_INT:
        return sizeof(GLuint);
      case GL_UNSIGNED_BYTE:
        return sizeof(GLbyte);
    }

    return 0;
  }
};  // struct VertexBufferElement

class VertexBufferLayout {
 private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;

 public:
  VertexBufferLayout() : m_Stride(0){};

  template <typename T>
  void Push(unsigned int count) {}
  template <>
  void Push<float>(unsigned int count) {
    m_Elements.push_back({count, GL_FLOAT, GL_FALSE});
    m_Stride += VertexBufferElement::GetSize(GL_FLOAT) * count;
  }
  template <>
  void Push<unsigned int>(unsigned int count) {
    m_Elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
    m_Stride += VertexBufferElement::GetSize(GL_UNSIGNED_INT) * count;
  }
  template <>
  void Push<char>(unsigned int count) {
    m_Elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
    m_Stride += VertexBufferElement::GetSize(GL_UNSIGNED_BYTE) * count;
  }
  template <>
  void Push<glm::vec3>(unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
      Push<float>(3);
    }
  }
  template <>
  void Push<glm::vec2>(unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
      Push<float>(2);
    }
  }
  template <>
  void Push<glm::vec4>(unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
      Push<float>(4);
    }
  }

  const std::vector<VertexBufferElement>& GetElements() const {
    return m_Elements;
  }

  unsigned int GetStride() const { return m_Stride; }
};
