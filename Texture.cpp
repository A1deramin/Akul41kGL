#include "Texture.h"

#include "stbimage/stb_image.h"

Texture::Texture(const std::string& path)
    : m_ID(0),
      m_FilePath(path),
      m_LocalBuffer(nullptr),
      m_Width(0),
      m_Height(0),
      m_BPP(0) {
  // stbi_set_flip_vertically_on_load(true);
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
  glGenTextures(1, &m_ID);
  glBindTexture(GL_TEXTURE_2D, m_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, m_LocalBuffer);
  if (!m_RetainLocalBuffer) {
    stbi_image_free(m_LocalBuffer);
  }
}
Texture::~Texture() {
  if (m_RetainLocalBuffer) {
    stbi_image_free(m_LocalBuffer);
  }
  glDeleteTextures(1, &m_ID);
}
void Texture::Bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_ID);
}
void Texture::UnBind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}
