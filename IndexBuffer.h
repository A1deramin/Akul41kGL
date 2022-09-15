#pragma once
#include <glad/glad.h>

#include <vector>
class IndexBuffer {
 public:
  // ID reference of the Element Buffer Object
  unsigned int m_ID;

 private:
  unsigned int m_Count;

 public:
  // Ctor that generates a Element Buffer Object and links it to indices
  IndexBuffer(unsigned int* indices, unsigned int count);

  IndexBuffer(const std::vector<unsigned int>& indices);

  IndexBuffer();

  void SetData(const std::vector<unsigned int>& indices);

  unsigned int GetCount() const;

  void Bind() const;
  void UnBind() const;
  void Delete() const;
};
