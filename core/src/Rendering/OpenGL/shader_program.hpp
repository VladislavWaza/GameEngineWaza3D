#pragma once

#include <glad/glad.h>

namespace waza3d {

    class ShaderProgram {
        bool m_isCompiled = false;
        GLuint m_id = 0;

        bool createShader(const char* shader_src, const GLenum shader_type, GLuint& shader_id);
    public:
        ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
        ShaderProgram(ShaderProgram&&) noexcept;
        ShaderProgram& operator=(ShaderProgram&&) noexcept;
        ~ShaderProgram();

        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        bool isCompiled();
        void bind() const;
        static void unbind();
    };

}