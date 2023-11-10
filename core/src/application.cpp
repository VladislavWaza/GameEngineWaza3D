#include "application.hpp"
#include "logs.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace waza3d {
	Application::Application()
	{
        LOG_INFO("Info log!");
        LOG_ERROR("Some error message");
        LOG_WARNING("Warning message");
        LOG_CRITICAL("Critical message");
	}

	Application::~Application()
	{

	}

	int Application::start(unsigned int width, unsigned int height, const char* title)
	{
        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /*Инициализируем GLAD, для связывания всех функций OpenGL*/
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -1;
        }

        /*Красный цвет фона*/
        glClearColor(1, 0, 0, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            on_update();
        }

        glfwTerminate();
        return 0;
	}

}