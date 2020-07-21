#include "shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(const char * VertexPath, const char * FragmentPath) {
    std::string VertexCode;
    std::string FragmentCode;

    std::ifstream VertexShaderFile;
    std::ifstream FragmentShaderFile;

    VertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    FragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        VertexShaderFile.open(VertexPath);
        FragmentShaderFile.open(FragmentPath);
        std::stringstream VertexShaderStream, FragmentShaderStream;

        // Read files
        VertexShaderStream << VertexShaderFile.rdbuf();
        FragmentShaderStream << FragmentShaderFile.rdbuf();
        VertexShaderFile.close();
        FragmentShaderFile.close();

        VertexCode = VertexShaderStream.str();
        FragmentCode = FragmentShaderStream.str();
    } catch (std::ifstream::failure & e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const GLchar * VertexSource = VertexCode.c_str();
    const GLchar * FragmentSource = FragmentCode.c_str();

    GLuint Vertex, Fragment;

    // Compile Vertex Shader
    Vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vertex, 1, &VertexSource, NULL);
    glCompileShader(Vertex);
    CheckShaderErrors(Vertex, "VERTEX");

    // Compile Fragment Shader
    Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Fragment, 1, &FragmentSource, NULL);
    glCompileShader(Fragment);
    CheckShaderErrors(Fragment, "FRAGMENT");

    // Link Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, Vertex);
    glAttachShader(ID, Fragment);
    glLinkProgram(ID);
    CheckShaderErrors(ID, "LINK");

    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::SetBool(const std::string & Name, bool Value) const {
    glUniform1i(glGetUniformLocation(ID, Name.c_str()), Value);
}

void Shader::SetInt(const std::string & Name, int Value) const {
    glUniform1i(glGetUniformLocation(ID, Name.c_str()), Value);
}

void Shader::SetFloat(const std::string & Name, float Value) const {
    glUniform1f(glGetUniformLocation(ID, Name.c_str()), Value);
}

void Shader::SetVec2(const std::string & Name, const glm::vec2 & Vector) const {
    glUniform2fv(glGetUniformLocation(ID, Name.c_str()), 1, &Vector[0]);
}

void Shader::SetVec2(const std::string & Name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, Name.c_str()), x, y);
}

void Shader::SetVec3(const std::string & Name, const glm::vec3 & Vector) const {
    glUniform3fv(glGetUniformLocation(ID, Name.c_str()), 1, &Vector[0]);
}

void Shader::SetVec3(const std::string & Name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, Name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string & Name, const glm::vec4 & Vector) const {
    glUniform4fv(glGetUniformLocation(ID, Name.c_str()), 1, &Vector[0]);
}

void Shader::SetVec4(const std::string & Name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(ID, Name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string & Name, const glm::mat2 & Matrix) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, &Matrix[0][0]);
}

void Shader::SetMat3(const std::string & Name, const glm::mat3 & Matrix) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, &Matrix[0][0]);
}

void Shader::SetMat4(const std::string & Name, const glm::mat4 & Matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, &Matrix[0][0]);
}

void Shader::CheckShaderErrors(GLuint Shader, const std::string Type) {
    GLint Success;
    GLchar InfoLog[1024];

    if (Type != "LINK") {
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
        if (!Success) {
            glGetShaderInfoLog(Shader, 1024, NULL, InfoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR in " << Type << "\n" << InfoLog << "\n---------\n" << std::endl;
        }
    } else {
        glGetProgramiv(Shader, GL_LINK_STATUS, &Success);
        if (!Success) {
            glGetProgramInfoLog(Shader, 1024, NULL, InfoLog);
            std::cout << "ERROR::SHADER_PROGRAM_LINKING_ERROR" << "\n" << InfoLog << "\n---------\n" << std::endl;
        }
    }
}