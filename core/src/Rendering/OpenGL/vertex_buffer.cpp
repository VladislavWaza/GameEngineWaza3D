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


    VertexBuffer::VertexBuffer(const void* data, const size_t size, const UsageType usage)
    {
        /*���������� ����� ��� �������� ������ � ����������*/
        glGenBuffers(1, &m_id);
        /*���������� ����� � ��������� �������*/
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        /*��������� ������ � ����������� � ���������� ��� ���� ������ �� ����� ��������*/
        glBufferData(GL_ARRAY_BUFFER, size, data, usageTypeToGLenum(usage));
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    {
        m_id = other.m_id;
        other.m_id = 0;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
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

}