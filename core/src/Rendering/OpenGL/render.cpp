#include "render.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "model.hpp"
#include "logs.hpp"

namespace waza3d {
	bool Render::init(GLFWwindow* window)
	{
		/*Делаем контекст окна текущим*/
		glfwMakeContextCurrent(window);

		/*Инициализируем GLAD, для связывания всех функций OpenGL*/
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			LOG_CRITICAL("Failed to initialize GLAD");
			return false;
		}

		LOG_INFO("OpenGL context initialized:");
		LOG_INFO("\tVendor: {0}", getVendorStr());
		LOG_INFO("\tRenderer: {0}", getRendererStr());
		LOG_INFO("\tVersion: {0}", getVersionStr());

		return true;
	}

	void Render::draw(const Model& model)
	{
		model.bind();
		/*Отрисовываем, выбираем тип, число вертексов, тип, указатель на индексы(nullptr так как он уже загружен в видеопамять)*/
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.getIndexesCount()), GL_UNSIGNED_INT, nullptr);
	}

	void Render::setClearColor(const float r, const float g, const float b, const float a)
	{
		glClearColor(r, g, b, a);
	}

	void Render::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Render::setViewport(const unsigned int width, const unsigned int height, const unsigned int left_offset, const unsigned int bottom_offset)
	{
		glViewport(left_offset, bottom_offset, width, height);
	}

	const char* Render::getVendorStr()
	{
		return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	}

	const char* Render::getRendererStr()
	{
		return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	}

	const char* Render::getVersionStr()
	{
		return reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}
}