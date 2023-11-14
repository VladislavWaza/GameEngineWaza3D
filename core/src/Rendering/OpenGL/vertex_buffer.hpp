#pragma once
#include <vector>

namespace waza3d {

	enum class ShaderDataType {
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4
	};

	struct BufferElement
	{
		ShaderDataType type;
		uint32_t component_type;
		size_t components_count;
		size_t size;
		size_t offset;

		BufferElement(const ShaderDataType type);
	};


	class BufferLayout {
		std::vector<BufferElement> m_elems;
		/*шаг смещения*/
		size_t m_stride = 0;
	public:
		BufferLayout(std::initializer_list<BufferElement> elems);
		const std::vector<BufferElement>& getElems() const;
		size_t getStride() const;

	};

	/* Класс управления буфером для передачи данных о вершинах в видеопамять */
	class VertexBuffer {
		unsigned int m_id = 0;
		BufferLayout m_layout;
	public:

		enum class UsageType
		{
			Static, //Один раз записываем данные и много раз используем
			Dynamic, //Часто изменяем данные
			Stream //Единократно изменяем и используем один или много раз
		};

		VertexBuffer(const void* data, const size_t size, BufferLayout layout, const UsageType usage);
		VertexBuffer(VertexBuffer&&) noexcept;
		VertexBuffer& operator=(VertexBuffer&&) noexcept;
		~VertexBuffer();
		

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		void bind() const;
		static void	unbind();

		const BufferLayout& getLayout() const;
	};
}