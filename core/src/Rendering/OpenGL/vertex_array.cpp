#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "logs.hpp"

#include <glad/glad.h>

namespace waza3d {

	VertexArray::VertexArray()
	{
		/*Генерируем VertexArrayObject*/
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept
	{
		m_id = other.m_id;
		m_elem_count = other.m_elem_count;
		m_indexes_count = other.m_indexes_count;
		other.m_id = 0;
		other.m_elem_count = 0;
		other.m_indexes_count = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		m_id = other.m_id;
		m_elem_count = other.m_elem_count;
		m_indexes_count = other.m_indexes_count;
		other.m_id = 0;
		other.m_elem_count = 0;
		other.m_indexes_count = 0;
		return *this;
	}

	void VertexArray::addVertexBuffer(const VertexBuffer& vertex_buffer)
	{
		/*Назначаем буфер и массив активными*/
		bind();
		vertex_buffer.bind();

		for (const BufferElement& cur_elem : vertex_buffer.getLayout().getElems())
		{
			/*Связываем буфер с позицией в коде куда он пойдет в шейдер
			для этого сначала активируем позицию*/
			glEnableVertexAttribArray(m_elem_count);
			/*Связываем и указываем лейаут
			с параметрами: позиция, число вертексов, тип, нужно ли нормализовать, шаг смещения, смещение начала*/
			glVertexAttribPointer(
				m_elem_count, 
				static_cast<GLint>(cur_elem.components_count), 
				static_cast<GLenum>(cur_elem.component_type), 
				GL_FALSE,
				static_cast<GLsizei>(vertex_buffer.getLayout().getStride()), 
				reinterpret_cast<const void*>(cur_elem.offset));
			++m_elem_count;
		}
	}

	void VertexArray::setIndexBuffer(const IndexBuffer& index_buffer)
	{
		/*Назначаем буфер и массив активными*/
		bind();
		index_buffer.bind();
		m_indexes_count = index_buffer.count();
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}
	size_t VertexArray::indexesCount() const
	{
		return m_indexes_count;
	}
}