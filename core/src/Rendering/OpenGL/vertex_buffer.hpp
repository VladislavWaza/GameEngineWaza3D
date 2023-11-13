#pragma once

namespace waza3d {

	class VertexBuffer {
		unsigned int m_id = 0;

	public:

		enum class UsageType
		{
			Static, //Один раз записываем данны и много раз используем
			Dynamic, //Часто изменяем данные
			Stream //Однократно изменяем и используем много или нет
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