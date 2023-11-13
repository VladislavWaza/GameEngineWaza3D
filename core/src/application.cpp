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

		/*Задаем функцию обработки ивента перемещения мыши извне*/
		m_event_dispatcher.addEventListener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{
				//LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.m_x, event.m_y);
			}
		);

		/*Задаем функцию обработки ивента изменения размера окна извне*/
		m_event_dispatcher.addEventListener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				LOG_INFO("[WindowResize] Changed size to {0}x{1}", event.m_width, event.m_height);
			}
		);

		/*Задаем функцию обработки ивента закрытия окна извне*/
		m_event_dispatcher.addEventListener<EventWindowClose>(
			[&](EventWindowClose& e)
			{
				LOG_INFO("[WindowClose]");
				m_close_window = true;
			}
		);

		/*Эта лямбда будет вызываться каждый раз когда происходит какой-либо ивент*/
		m_window->setEventCallback(
			[&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			}
		);


		while (!m_close_window)
		{
			m_window->onUpdate(); 
			onUpdate();
		}
		m_window = nullptr;
        return 0;
	}

}