#include "shader_program.hpp"
#include "logs.hpp"

#include "GLFW/glfw3.h"

namespace waza3d {
    bool ShaderProgram::createShader(const char* shader_src, const GLenum shader_type, GLuint& shader_id)
    {
        /*Генерируем идентификатор шейдера*/
        shader_id = glCreateShader(shader_type);
        /*Задаем код шейдера и компилируем*/
        glShaderSource(shader_id, 1, &shader_src, nullptr);
        glCompileShader(shader_id);

        /*Проверяем корректность компиляции*/
        GLint success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            /*Выдаем ошибку компиляции*/
            char info_log[1024];
            glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);

            LOG_CRITICAL("Shader compilation error:\n{}", info_log);
            return false;
        }
        return true;
    }


    ShaderProgram::ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src)
	{
        /*Создаем шейдеры*/
        GLuint vertex_shader_id = 0;
        if (!createShader(vertex_shader_src, GL_VERTEX_SHADER, vertex_shader_id))
        {
            LOG_CRITICAL("Vertex shader compilation error");
            glDeleteShader(vertex_shader_id);
            return;
        }
        GLuint fragment_shader_id = 0;
        if (!createShader(fragment_shader_src, GL_FRAGMENT_SHADER, fragment_shader_id))
        {
            LOG_CRITICAL("Fragment shader compilation error");
            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);
            return;
        }
        
        /*Генерируем идентификатор шейдерной программы*/
        m_id = glCreateProgram();
        /*Линкуем к ней шейдеры*/
        glAttachShader(m_id, vertex_shader_id);
        glAttachShader(m_id, fragment_shader_id);
        glLinkProgram(m_id);

        /*Проверяем корректность линковки*/
        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLchar log_info[1024];
            glGetProgramInfoLog(m_id, 1024, nullptr, log_info);
            LOG_CRITICAL("Shader program link error:\n{}", log_info);
            glDeleteProgram(m_id);
            m_id = 0;
            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);
            return;
        }
        else
        {
            m_isCompiled = true;
        }

        /*Шейдеры больше не нужны, удаляем*/
        glDetachShader(m_id, vertex_shader_id);
        glDetachShader(m_id, fragment_shader_id);
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
	}

    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    {
        m_id = other.m_id;
        m_isCompiled = other.m_isCompiled;

        other.m_id = 0;
        other.m_isCompiled = false;
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
    {
        glDeleteProgram(m_id);
        m_id = other.m_id;
        m_isCompiled = other.m_isCompiled;

        other.m_id = 0;
        other.m_isCompiled = false;
        return *this;
    }


    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_id);
    }

    bool ShaderProgram::isCompiled()
    {
        return m_isCompiled;
    }

    void ShaderProgram::bind()
    {
        glUseProgram(m_id);
    }
    void ShaderProgram::unbind()
    {
        glUseProgram(0);
    }
}
