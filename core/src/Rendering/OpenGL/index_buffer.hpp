#pragma once
#include "vertex_buffer.hpp"


namespace waza3d {

	/*Класс нужен для того, чтобы переиспользывать вертексы смежных полигонов, а не дублировать*/
	class IndexBuffer {
		unsigned int m_id = 0;
		size_t m_count = 0;

	public:
		IndexBuffer(const void* data, const size_t count, const VertexBuffer::UsageType usage);
		~IndexBuffer();
		IndexBuffer(IndexBuffer&&) noexcept;
		IndexBuffer& operator=(IndexBuffer&&) noexcept;

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		void bind() const;
		static void	unbind();
		size_t count() const;
	};
}