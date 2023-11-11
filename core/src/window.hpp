#pragma once
#include <string>

struct GLFWwindow;

namespace waza3d {

	class Window
	{
	public:
		Window(unsigned int width, unsigned int height, const std::string& title);
		~Window();


		//Класс не может копироваться и перемещаться
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void on_update();
		unsigned int width() const;
		unsigned int height() const;
	private:
		int init();
		void shutdown();


		GLFWwindow* m_window;
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		std::string m_title;
		static inline bool s_GLFW_initialized = false;
	};
}