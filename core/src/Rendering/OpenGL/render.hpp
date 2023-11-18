#pragma once

struct GLFWwindow;

namespace waza3d {

	class Model;

	/*Класс имплементирует в себя функции отрисовки OpenGL*/
	class Render {
	public:
		/*Инициализация контекста окна, инициализация GLAD*/
		static bool init(GLFWwindow* window);

		/*Отрисовка модели*/
		static void draw(const Model& model);
		/*Выбор цвета заливки*/
		static void setClearColor(const float r, const float g, const float b, const float a);
		/*Очищение путем заливки*/
		static void clear();
		/*Выбор перерисовываемой области*/
		static void setViewport(const unsigned int width, const unsigned int height,
			const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);

		static const char* getVendorStr();
		static const char* getRendererStr();
		static const char* getVersionStr();

	};

}