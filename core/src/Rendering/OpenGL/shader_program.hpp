#pragma once

namespace waza3d {
    
    /* Класс управления программой вертексного и фрагментарного шейдеров */
    class ShaderProgram {

        bool m_isCompiled = false;
        unsigned int m_id = 0;

    public:
        enum class ShaderType {
            Vertex,
            Fragment
        };

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

    private:
        bool createShader(const char* shader_src, const ShaderType shader_type, unsigned int& shader_id);
    };

}