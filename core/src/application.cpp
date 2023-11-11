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

		while (true)
		{
			m_window->on_update(); 
			on_update();
		}

        return 0;
	}

}