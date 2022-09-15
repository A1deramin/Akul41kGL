#include "Shader.h"

#include <fstream>
#include <sstream>
#include <string>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
    std::string GeometrySource;
};

ShaderProgramSource parse_shader(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        std::cout << "Error reading shader source file!" << std::endl;
    }
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2 };
    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;
    while (getline(in, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            } else if (line.find("geometry") != std::string::npos) {
                type = ShaderType::GEOMETRY;
            }
        } else {
            if (type == ShaderType::NONE) {
                std::cout << "Parsing shader source error!" << std::endl;
                return {};
            }
            ss[static_cast<int>(type)] << line << '\n';
        }
    }
    return {ss[0].str(), ss[1].str(), ss[2].str()};
}

// Ctor that build the Shader Program from 2 different shaders
Shader::Shader(const std::string& filePath) {
    // read vertex and fragment shaders source code
    auto shaderSource = parse_shader(filePath);
    const char* vertexSource = shaderSource.VertexSource.c_str();
    const char* fragmentSource = shaderSource.FragmentSource.c_str();
    bool useGeometryShader = !shaderSource.GeometrySource.empty();
    const char* geometrySource;
    if (useGeometryShader) {
        geometrySource = shaderSource.GeometrySource.c_str();
    }

    // Create Vertex Shader Object and get its reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader Source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Compile Vertex Shader into machine code
    glCompileShader(vertexShader);
    CheckForCompilationErrors(vertexShader, "Vertex");

    // Create Fragment Shader Object and get its reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader Source to the Vertex Shader Object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Compile Fragment Shader into machine code
    glCompileShader(fragmentShader);
    CheckForCompilationErrors(fragmentShader, "Fragment");
    GLuint geometryShader;
    if (useGeometryShader) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometrySource, NULL);
        glCompileShader(geometryShader);
        CheckForCompilationErrors(geometryShader, "Geometry");
    }
    // Create Shader Program Object and get its reference
    m_ID = glCreateProgram();

    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);

    if (useGeometryShader) {
        glAttachShader(m_ID, geometryShader);
    }

    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(m_ID);

    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (useGeometryShader) {
        glDeleteShader(geometryShader);
    }
}

// Activates the Shader Program
void Shader::Bind() const {
    glUseProgram(m_ID);
}

// Deletes the Shader Program
void Shader::UnBind() const {
    glUseProgram(0);
}

Shader::~Shader() {
    glDeleteProgram(m_ID);
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    auto location = glGetUniformLocation(m_ID, name.c_str());
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2,
    float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1i(const std::string& name, int v) {
    glUniform1i(GetUniformLocation(name), v);
}

void Shader::SetUniform1f(const std::string& name, float v) {
    glUniform1f(GetUniformLocation(name), v);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& m) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, false, &m[0][0]);
}

void Shader::SetUniform4fv(const std::string& name, const glm::vec4& vec) {
    glUniform4fv(GetUniformLocation(name), 1, &vec[0]);
}

void Shader::SetUniform3fv(const std::string& name, const glm::vec3& vec) {
    glUniform3fv(GetUniformLocation(name), 1, &vec[0]);
}

void Shader::CheckForCompilationErrors(unsigned int shaderID,
    const std::string& context) {
    int isCompiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
        std::cout << "Shader Error!: " << context << std::endl;
        int maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
        // The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);
        for (char i : errorLog) {
            std::cout << i;
        }
        std::cout << std::endl;

        glDeleteShader(shaderID);
    }
}

std::string Shader::GetDefaultShaderPath(EDefaultShader shaderType) {
    switch (shaderType) {
        case EDefaultShader::DEFAULT: return "res/shaders/default.shader";

        case EDefaultShader::SOLID_COLOR: return "res/shaders/solid_color.shader";

        case EDefaultShader::SOLID_COLOR_WIREFRAME: return "res/shaders/solid_color_wireframe.shader";

        case EDefaultShader::COLOR: return "res/shaders/color.shader";

        case EDefaultShader::LIGHTING: return "res/shaders/material.shader";

        case EDefaultShader::VERTEX_LIGHTING: return "res/shaders/vertex_lighting.shader";
        case EDefaultShader::NONE: return "";
    }

    return "invalid_path";
}

ShaderPtr Shader::GetDefaultShader(
    EDefaultShader shaderType = EDefaultShader::DEFAULT) {
    return std::make_shared<Shader>(GetDefaultShaderPath(shaderType));
}
