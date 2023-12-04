#pragma once
#include <memory>

#include "event.hpp"

namespace waza3d {

	class ShaderProgram;
	class Scene;
	class Camera;

	/* Для использовния класса задать сцену, предоставив указатель на неё*/
	/* Для использовния класса нужно переопределить функцию void onUpdate(), которая циклично вызывается*/
	/* Можно перед запуском основного цикла работы произвести действия в функции beforeStart()*/
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
		virtual int start(unsigned int width, unsigned int height, const char* title, Scene* scene);

		/*Вызывается до цикла работы*/
		virtual void beforeStart() {};
		/*Вызывается в цикле работы*/
		virtual void onUpdate() {};
		virtual void onUIDraw() {};
	
		Scene* m_scene = nullptr;
		std::unique_ptr<Camera> m_camera;
	private:
		std::unique_ptr<class Window> m_window;

		EventDispatcher m_event_dispatcher;
		bool m_close_window = false;

		/*Указатель на программу с шейдерами*/
		std::unique_ptr<ShaderProgram> m_shader_program;
	};
}