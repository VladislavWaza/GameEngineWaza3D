#pragma once
#include <memory>

namespace waza3d {

	/* Для использовния класса нужно переопределить функцию void on_update()*/
	class Application 
	{
	public:
		Application();
		virtual ~Application();

		// Класс должен быть в одном экземпляре и не может копироваться
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int start(unsigned int width, unsigned int height, const char* title);

		virtual void on_update() {};
	
	private:
		std::unique_ptr<class Window> m_window;
	};
}