#include "vertex_buffer.hpp"
#include "logs.hpp"

#include "glad/glad.h"

namespace waza3d {
    constexpr unsigned int shaderDataTypeToComponentsCount(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Int:
            return 1;

        case ShaderDataType::Float2:
        case ShaderDataType::Int2:
            return 2;

        case ShaderDataType::Float3:
        case ShaderDataType::Int3:
            return 3;

        case ShaderDataType::Float4:
        case ShaderDataType::Int4:
            return 4;
        }

        LOG_ERROR("shaderDataTypeToComponentsCount: unknown ShaderDataType");
        return 0;
    }

    constexpr size_t shaderDataTypeSize(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return sizeof(GLfloat) * shaderDataTypeToComponentsCount(type);
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return sizeof(GLint) * shaderDataTypeToComponentsCount(type);
        }

        LOG_ERROR("shaderDataTypeSize: unknown ShaderDataType");
        return 0;
    }

    constexpr unsigned int shaderDataTypeToComponentsType(const ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        }

        LOG_ERROR("shaderDataTypeToComponentsType: unknown ShaderDataType");
        return GL_FLOAT;
    }

    BufferElement::BufferElement(const ShaderDataType type)
        : type(type)
        , component_type(shaderDataTypeToComponentsType(type))
        , components_count(shaderDataTypeToComponentsCount(type))
        , size(shaderDataTypeSize(type))
        , offset(0)
    {
    }


    BufferLayout::BufferLayout(std::initializer_list<BufferElement> elems)
        : m_elems(std::move(elems))
    {
        size_t offset = 0;
        m_stride = 0;
        for (auto& element : m_elems)
        {
            element.offset = offset;
            offset += element.size;
            m_stride += element.size;
        }
    }

    const std::vector<BufferElement>& BufferLayout::getElems() const
    {
        return m_elems;
    }

    size_t BufferLayout::getStride() const
    {
        return m_stride;
    }
}