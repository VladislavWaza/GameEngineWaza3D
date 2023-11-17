#pragma once
#include <initializer_list>
#include <memory>

#include "Rendering/OpenGL/vertex_buffer.hpp"
#include "Rendering/OpenGL/index_buffer.hpp"
#include "Rendering/OpenGL/vertex_array.hpp"

namespace waza3d {

	class Model
	{
		size_t m_points_count = 0;
		size_t m_indexes_count = 0;
		float* m_points = nullptr;
		unsigned int* m_indexes = nullptr;

		std::unique_ptr<VertexBuffer> m_vertex_buffer;
		std::unique_ptr<IndexBuffer> m_index_buffer;
		std::unique_ptr<VertexArray> m_vertex_array;
	public:
		Model(
			std::initializer_list<float> points, 
			std::initializer_list<unsigned int> indexes,
			BufferLayout layout, 
			const VertexBuffer::UsageType points_usage,
			const VertexBuffer::UsageType index_usage);
		~Model();

		size_t getPointsCount() const;
		size_t getIndexesCount() const;

		void bind() const;
	};
	

}