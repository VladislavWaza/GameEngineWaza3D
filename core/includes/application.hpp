#pragma once
#include <memory>

#include "event.hpp"
#include "camera.hpp"

namespace waza3d {

	class ShaderProgram;

	/* Для использовния класса нужно переопределить функцию void onUpdate(), которая циклично вызывается*/
	/* Для использования собстенного интерфейса нужно переопределить функцию void onUIDraw(), которая циклично вызывается*/
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

		/*Запуск, в методе содержится основной цикл работы*/
		virtual int start(unsigned int width, unsigned int height, const char* title);

		/*Вызывается в цикле работы*/
		virtual void onUpdate() {};
		virtual void onUIDraw() {};

		/*Камера*/
		float m_camera_pos[3] = { 0.f, 0.f, -1.f };
		float m_camera_rotation[3] = { 180.f, 0.f, 0.f };
		bool m_perspective_camera = false;
		Camera m_camera;
	
	private:
		std::unique_ptr<class Window> m_window;

		EventDispatcher m_event_dispatcher;
		bool m_close_window = false;

		/*Указатель на программу с шейдерами*/
		std::unique_ptr<ShaderProgram> m_shader_program;
	};
}