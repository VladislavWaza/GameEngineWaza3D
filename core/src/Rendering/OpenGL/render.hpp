#pragma once

struct GLFWwindow;

namespace waza3d {

	class Model;

	/*Класс имплементирует в себя функции отрисовки OpenGL*/
	class Render {
	public:
		static bool init(GLFWwindow* window);

		static void draw(const Model& model);
		static void setClearColor(const float r, const float g, const float b, const float a);
		static void clear();
		static void setViewport(const unsigned int width, const unsigned int height,
			const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);

		static const char* getVendorStr();
		static const char* getRendererStr();
		static const char* getVersionStr();

	};

}