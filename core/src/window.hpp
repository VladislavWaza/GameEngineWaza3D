#pragma once
#include <string>
#include <functional>

#include "event.hpp"

struct GLFWwindow;

namespace waza3d {

	class Window
	{
	public:
		using EventCallbackFun = std::function<void(BaseEvent&)>;

		Window(unsigned int width, unsigned int height, const std::string& title);
		~Window();


		//Класс не может копироваться и перемещаться
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		/*onUpdate циклично вызывается в Application*/
		void onUpdate();
		unsigned int width() const;
		unsigned int height() const;
		void setEventCallback(const EventCallbackFun& callback);
	private:
		int init();
		void shutdown();

		struct WindowData
		{
			unsigned int m_width = 0;
			unsigned int m_height = 0;
			std::string m_title;
			/*функция обработки ивента*/
			EventCallbackFun m_event_callback_fun; 

		};

		GLFWwindow* m_window = nullptr;
		static inline bool s_GLFW_initialized = false;
		WindowData m_data;
	};
}