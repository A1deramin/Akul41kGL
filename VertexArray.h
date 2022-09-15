#pragma once
#include <glad/glad.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
template <class Vertex>
class VertexArray {
 public:
  // ID reference of the Vertex Array Object
  GLuint m_ID;
  // Ctor that generates a VAO ID
  VertexArray();

  void AddBuffer(const VertexBuffer<Vertex>& vbo,
                 const VertexBufferLayout& layout);
  void Bind() const;
  void UnBind() const;
  void Delete() const;
};

// Ctor that generates a VertexArray ID
template <class Vertex>
VertexArray<Vertex>::VertexArray() {
  glGenVertexArrays(1, &m_ID);
}

template <class Vertex>
void VertexArray<Vertex>::AddBuffer(const VertexBuffer<Vertex>& vb,
                                    const VertexBufferLayout& layout) {
  Bind();
  vb.Bind();
  const auto& elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, element.normalized,
                          layout.GetStride(), (const void*)offset);
    offset += element.count * VertexBufferElement::GetSize(element.type);
  }
}
template <class Vertex>
void VertexArray<Vertex>::Bind() const {
  glBindVertexArray(m_ID);
}
template <class Vertex>
void VertexArray<Vertex>::UnBind() const {
  glBindVertexArray(0);
}
template <class Vertex>
void VertexArray<Vertex>::Delete() const {
  glDeleteVertexArrays(1, &m_ID);
}
