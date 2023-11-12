#include "application.hpp"
#include "logs.hpp"
#include "window.hpp"


namespace waza3d {
	Application::Application()
	{
        LOG_INFO("Starting application!");
	}

	Application::~Application()
	{
		LOG_INFO("Closing application!");
	}

	int Application::start(unsigned int width, unsigned int height, const char* title)
	{
		m_window = std::make_unique<Window>(width, height, title);

		/*Задаем функцию обработки ивента извне*/
		m_window->setEventCallback(
			[](Event& event)
			{
				LOG_INFO("[EVENT] Changed size to {0}x{1}", event.m_width, event.m_height);
			}
		);

		while (true)
		{
			m_window->onUpdate(); 
			onUpdate();
		}

        return 0;
	}

}