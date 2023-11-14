#include "index_buffer.hpp"
#include "logs.hpp"

#include <glad/glad.h>

namespace waza3d {
    constexpr GLenum usageTypeToGLenum(const VertexBuffer::UsageType usage_type)
    {
        switch (usage_type)
        {
        case VertexBuffer::UsageType::Dynamic: return GL_DYNAMIC_DRAW;
        case VertexBuffer::UsageType::Static: return GL_STATIC_DRAW;
        case VertexBuffer::UsageType::Stream: return GL_STREAM_DRAW;
        }
        LOG_CRITICAL("Unknown VertexBuffer usage");
        return GL_STREAM_DRAW;
    }

    IndexBuffer::IndexBuffer(const void* data, const size_t count, const VertexBuffer::UsageType usage)
        :m_count(count)
    {
        /*Генерируем буфер для передачи данных в видеокарту*/
        glGenBuffers(1, &m_id);
        /*Подключаем буфер и назначаем текущим*/
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        /*Отправлем данные в видеопамять и обозначаем что наши данные не будут меняться*/
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), data, usageTypeToGLenum(usage));
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
        :m_count(other.m_count), m_id(other.m_id)
    {
        other.m_count = 0;
        other.m_id = 0;
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
    {
        m_count = other.m_count;
        m_id = other.m_id;
        other.m_count = 0;
        other.m_id = 0;
        return *this;
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    size_t IndexBuffer::count() const
    {
        return m_count;
    }

}
