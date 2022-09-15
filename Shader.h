#pragma once
#include <glad/glad.h>

#include <cerrno>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

static struct ShaderProgramSource parse_shader(const std::string& filePath);

enum class EDefaultShader {
    NONE,
    DEFAULT,
    COLOR,
    SOLID_COLOR,
    SOLID_COLOR_WIREFRAME,
    LIGHTING,
    VERTEX_LIGHTING
};

using ShaderPtr = std::shared_ptr<class Shader>;

class Shader {
private:
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    // ID reference of the Shader Program
    GLuint m_ID;

    // Ctor that build the Shader Program from 2 different shaders
    explicit Shader(const std::string& filePath);

    ~Shader();

    // Activates the Shader Program
    void Bind() const;
    // Deletes the Shader Program
    void UnBind() const;

    void SetUniform4f(const std::string& name, float v0, float v1, float v2,
        float v3);
    void SetUniform1i(const std::string& name, int v);
    void SetUniform1f(const std::string& name, float v);
    void SetUniformMat4f(const std::string& name, const glm::mat4& m);
    void SetUniform4fv(const std::string& name, const glm::vec4& vec);
    void SetUniform3fv(const std::string& name, const glm::vec3& vec);

    static std::string GetDefaultShaderPath(EDefaultShader shaderType);
    static ShaderPtr GetDefaultShader(EDefaultShader shaderType);

private:
    int GetUniformLocation(const std::string& name);

    static void CheckForCompilationErrors(unsigned int shaderID,
        const std::string& context = "");
};
