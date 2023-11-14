#pragma once

namespace waza3d {

	/* Класс управления буфером для передачи данных о вершинах в видеопамять */
	class VertexBuffer {
		unsigned int m_id = 0;

	public:

		enum class UsageType
		{
			Static, //Один раз записываем данные и много раз используем
			Dynamic, //Часто изменяем данные
			Stream //Единократно изменяем и используем один или много раз
		};

		VertexBuffer(const void* data, const size_t size, const UsageType usage);
		VertexBuffer(VertexBuffer&&) noexcept;
		VertexBuffer& operator=(VertexBuffer&&) noexcept;
		~VertexBuffer();
		

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		void bind() const;
		static void	unbind();
	};

}