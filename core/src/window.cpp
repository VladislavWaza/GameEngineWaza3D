#include "window.hpp"
#include "logs.hpp"
#include "event.hpp"
#include "Rendering/OpenGL/render.hpp"
#include "Modules/UI_module.hpp"

#include <GLFW/glfw3.h>

namespace waza3d {

	Window::Window(unsigned int width, unsigned int height, const std::string& title)
        :m_data({ width, height, title })
	{
        /*Инициализируем окно*/
		int resCode = init();
	}

	Window::~Window()
	{
		shutdown();
	}

	void Window::onUpdate()
	{
        glfwSwapBuffers(m_window);
        glfwPollEvents();
	}

	unsigned int Window::width() const
	{
		return m_data.m_width;
	}

	unsigned int Window::height() const
	{
		return m_data.m_height;
	}

    void Window::setEventCallback(const EventCallbackFun& callback)
    {
        m_data.m_event_callback_fun = callback;
    }

	int Window::init()
	{
        LOG_INFO("Creating window '{0}' with size {1}x{2}", m_data.m_title, m_data.m_width, m_data.m_height);

        /*Добавляем вывод лога GLFW при ошибке*/
        glfwSetErrorCallback([](int error_code, const char* description)
            {
                LOG_CRITICAL("GLFW error: {0}", description);
            }
        );

        /* Инициализируем GLFW */
        if (!glfwInit())
        {
            LOG_CRITICAL("Can't initialize GLFW!");
            return -1;
        }

        /* Создаем окно GLFW*/
        m_window = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            LOG_CRITICAL("Can't create window '{0}'!", m_data.m_title);
            return -2;
        }

        /*Инициализируем рендер*/
        if (!Render::init(m_window))
        {
            LOG_CRITICAL("Can't initialize Render!");
            return -3;
        }

        /*Кладем пользовательские данные m_data в GLFW окно m_window*/
        glfwSetWindowUserPointer(m_window, &m_data);


        glfwSetKeyCallback(m_window,
            [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                switch (action)
                {
                case GLFW_PRESS:
                {
                    EventKeyPressed event(static_cast<KeyCode>(key), false);
                    data.m_event_callback_fun(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    EventKeyReleased event(static_cast<KeyCode>(key));
                    data.m_event_callback_fun(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    EventKeyPressed event(static_cast<KeyCode>(key), true);
                    data.m_event_callback_fun(event);
                    break;
                }
                default:
                    break;
                }
            }
        );

        /*Эта лямбда-функция будет вызываться каждый раз когда происходит встроенное в GLFW изменение размеров окна*/
        glfwSetWindowSizeCallback(m_window, 
            [](GLFWwindow* window, int width, int height)
            {
                /*Получаем ссылку на пользовательские данные m_data из GLFW окна m_window*/
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                /*Запоминем новые размеры окна*/
                data.m_width = width;
                data.m_height = height;

                /*Вызываем функцию-обработчик заданную извне*/
                EventWindowResize event(width, height);
                data.m_event_callback_fun(event);
            }
        );

        /*Эта лямбда-функция будет вызываться каждый раз когда происходит встроенное в GLFW событие изменения положение мыши*/
        glfwSetCursorPosCallback(m_window,
            [](GLFWwindow* window, double x, double y)
            {
                /*Позиция курсора относительно верхнего левого края*/

                /*Получаем ссылку на пользовательские данные m_data из GLFW окна m_window*/
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                /*Вызываем функцию-обработчик заданную извне*/
                EventMouseMoved event(x,y);
                data.m_event_callback_fun(event);
            }
        );

        /*Эта лямбда-функция будет вызываться каждый раз когда происходит встроенное в GLFW событие закрытия окна*/
        glfwSetWindowCloseCallback(m_window, 
            [](GLFWwindow* window)
            {
                /*Получаем ссылку на пользовательские данные m_data из GLFW окна m_window*/
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                EventWindowClose event;
                data.m_event_callback_fun(event);
            }
        );

        /*Указыаем границы буфера кадра(в пикселях) для отрисовки с помощью callback
        Эти размеры могу отличатся от размеров окна для дисплея Retina*/
        glfwSetFramebufferSizeCallback(m_window, 
            [](GLFWwindow* window, int width, int height)
            {
                /*Устанавливаем что хотим перерисовывать*/
                Render::setViewport(width, height, 0, 0);
            }
        );

        /*Инициализируем UI*/
        UIModule::init(m_window);

        return 0;  
	}

	void Window::shutdown()
	{
        UIModule::shutdown();
        glfwDestroyWindow(m_window);
        glfwTerminate();
	}

}