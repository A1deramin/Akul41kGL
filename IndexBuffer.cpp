#include "IndexBuffer.h"
// Ctor that generates a Element Buffer Object and links it to indices
IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count) {
    glGenBuffers(1, &m_ID);
    m_Count = count;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int),
                 indices, GL_DYNAMIC_DRAW);
}

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices) {
    glGenBuffers(1, &m_ID);
    m_Count = indices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int),
                 indices.data(), GL_DYNAMIC_DRAW);
}

IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::SetData(const std::vector<unsigned int>& indices) {
    Bind();
    m_Count = indices.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int),
                 indices.data(), GL_DYNAMIC_DRAW);
}

unsigned int IndexBuffer::GetCount() const { return m_Count; }

void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID); }
void IndexBuffer::UnBind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
void IndexBuffer::Delete() const { glDeleteBuffers(1, &m_ID); }
