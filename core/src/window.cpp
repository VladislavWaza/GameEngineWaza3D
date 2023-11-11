#include "window.hpp"
#include "logs.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace waza3d {

	Window::Window(unsigned int width, unsigned int height, const std::string& title)
		:m_width(width), m_height(height), m_title(title)
	{
		init();
	}

	Window::~Window()
	{
		shutdown();
	}

	void Window::on_update()
	{
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
	}

	unsigned int Window::width() const
	{
		return m_width;
	}

	unsigned int Window::height() const
	{
		return m_height;
	}

	int Window::init()
	{
        LOG_INFO("Creating window '{0}' with size {1}x{2}", m_title, m_width, m_height);

        /* Initialize the library */
        if (!s_GLFW_initialized)
        {
            if (!glfwInit())
            {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            LOG_CRITICAL("Can't create window '{0}'!", m_title);
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        /*Инициализируем GLAD, для связывания всех функций OpenGL*/
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -1;
        }

        return 0;  
	}

	void Window::shutdown()
	{
        glfwDestroyWindow(m_window);
        glfwTerminate();
	}

}