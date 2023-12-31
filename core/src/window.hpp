#pragma once
#include <string>
#include <functional>

#include <glm/ext/vector_float2.hpp>

struct GLFWwindow;

namespace waza3d {
	struct BaseEvent;

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
		glm::vec2 getCursorPos() const;

		void setEventCallback(const EventCallbackFun& callback);
	private:
		int init();
		void shutdown();

		struct WindowData
		{
			/*Размеры окна*/
			unsigned int m_width = 0;
			unsigned int m_height = 0;
			/*Заголовок окна*/
			std::string m_title;
			/*функция обработки ивента*/
			EventCallbackFun m_event_callback_fun; 

		};
		/*Хендл окна GLFW*/
		GLFWwindow* m_window = nullptr;
		/*Структура с размерами, заголовком и функцией обработки*/
		WindowData m_data;
	};
}