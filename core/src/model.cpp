#include "model.hpp"

#include <algorithm>

namespace waza3d {
	Model::Model(std::initializer_list<float> points, std::initializer_list<unsigned int> indexes,
		BufferLayout layout, const VertexBuffer::UsageType points_usage, const VertexBuffer::UsageType index_usage)
	{
		m_points_count = points.size();
		m_indexes_count = indexes.size();

		m_points = new float[m_points_count];
		m_indexes = new unsigned int[m_indexes_count];

		std::copy(points.begin(), points.end(), m_points);
		std::copy(indexes.begin(), indexes.end(), m_indexes);


		/*Генерируем буфер для передачи данных в видеокарту*/
		m_vertex_buffer = std::make_unique<VertexBuffer>(m_points, sizeof(float) * m_points_count, layout, points_usage);
		/*Генерируем индексный буфер*/
		m_index_buffer = std::make_unique<IndexBuffer>(m_indexes, m_indexes_count, index_usage);

		/*Генерируем и назначаем текущим VertexArray*/
		m_vertex_array = std::make_unique<VertexArray>();

		/*Связываем буферы c массивом*/
		m_vertex_array->addVertexBuffer(*m_vertex_buffer);
		m_vertex_array->setIndexBuffer(*m_index_buffer);
	}

	Model::~Model()
	{
		delete[] m_points;
		delete[] m_indexes;
	}
	void Model::bind() const
	{
		m_vertex_array->bind();
	}

	size_t Model::getPointsCount() const
	{
		return m_points_count;
	}
	size_t Model::getIndexesCount() const
	{
		return m_indexes_count;
	}
}