#pragma once

#include <glm/mat4x4.hpp>
namespace waza3d {
    
    /*Класс управления программой вертексного и фрагментарного шейдеров */
    class ShaderProgram {

        bool m_isCompiled = false;
        unsigned int m_id = 0;

    public:
        enum class ShaderType {
            Vertex,
            Fragment
        };

        /*Код шейдеров: вертексного и фрагментарного*/
        ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
        ShaderProgram(ShaderProgram&&) noexcept;
        ShaderProgram& operator=(ShaderProgram&&) noexcept;
        ~ShaderProgram();

        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        /*Удалось ли скомпилировать и слинковать программу*/
        bool isCompiled();
        /*Связывание*/
        void bind() const;
        /*Отвязывание*/
        static void unbind();
        /*Назначение uniform матрицы*/
        void setMatrix4(const char* name, const glm::mat4& matrix) const;

    private:
        bool createShader(const char* shader_src, const ShaderType shader_type, unsigned int& shader_id);
    };

}