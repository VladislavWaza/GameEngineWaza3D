#pragma once
namespace waza3d {
	class VertexBuffer;
	class IndexBuffer;

	/*Класс нужен чтобы связать VertexBuffer с входными атрибутами в коде шейдера и задать layout*/
	class VertexArray {
		unsigned int m_id = 0;
		unsigned int m_elem_count = 0;
		size_t m_indexes_count = 0;
	public:
		VertexArray();
		~VertexArray();

		VertexArray(VertexArray&& other) noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		/*Назначает текущими буфер и массив и добавляет буфер*/
		void addVertexBuffer(const VertexBuffer& vertex_buffer);
		void setIndexBuffer(const IndexBuffer& index_buffer);
		void bind() const;
		static void unbind();
		size_t indexesCount() const;
	};
}