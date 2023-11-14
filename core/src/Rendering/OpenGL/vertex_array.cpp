#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
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
		other.m_id = 0;
		other.m_elem_count = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		m_id = other.m_id;
		m_elem_count = other.m_elem_count;
		other.m_id = 0;
		other.m_elem_count = 0;
		return *this;
	}

	void VertexArray::addBuffer(const VertexBuffer& vertex_buffer)
	{
		/*Назначаем буфер и массив активными*/
		bind();
		vertex_buffer.bind();

		/*Связываем буфер с позицией в коде куда он пойдет в шейдер
		для этого сначала активируем позицию*/
		glEnableVertexAttribArray(m_elem_count);
		/*Связываем и указываем лейаут
	    с параметрами: позиция, число вертексов, тип, нужно ли нормализовать, шаг смещения, смещение начала*/
		glVertexAttribPointer(m_elem_count, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		++m_elem_count;
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}
}