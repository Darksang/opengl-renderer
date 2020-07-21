#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <iostream>

class Shader {
    public:
        Shader(const char * VertexPath, const char * FragmentPath);

        void Activate();
        void SetBool(const std::string & Name, bool Value) const;
        void SetInt(const std::string & Name, int Value) const;
        void SetFloat(const std::string & Name, float Value) const;
        void SetVec2(const std::string & Name, const glm::vec2 & Vector) const;
        void SetVec2(const std::string & Name, float x, float y) const;
        void SetVec3(const std::string & Name, const glm::vec3 & Vector) const;
        void SetVec3(const std::string & Name, float x, float y, float z) const;
        void SetVec4(const std::string & Name, const glm::vec4 & Vector) const;
        void SetVec4(const std::string & Name, float x, float y, float z, float w) const;
        void SetMat2(const std::string & Name, const glm::mat2 & Matrix) const;
        void SetMat3(const std::string & Name, const glm::mat3 & Matrix) const;
        void SetMat4(const std::string & Name, const glm::mat4 & Matrix) const;

    private:
        GLuint ID;

        void CheckShaderErrors(GLuint Shader, const std::string Type);
};

#endif