#include "vertex_buffer.hpp"
#include "logs.hpp"

#include "glad/glad.h"

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


    VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout layout, const UsageType usage)
        :m_layout(std::move(layout))
    {
        /*Генерируем буфер для передачи данных в видеокарту*/
        glGenBuffers(1, &m_id);
        /*Подключаем буфер и назначаем текущим*/
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        /*Отправлем данные в видеопамять и обозначаем что наши данные не будут меняться*/
        glBufferData(GL_ARRAY_BUFFER, size, data, usageTypeToGLenum(usage));
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
        :m_layout(std::move(other.m_layout))
    {
        m_id = other.m_id;
        other.m_id = 0;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
        m_layout = std::move(other.m_layout);
        m_id = other.m_id;
        other.m_id = 0;
        return *this;
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    const BufferLayout& VertexBuffer::getLayout() const
    {
        return m_layout;
    }
}