#pragma once
#include <string>
#include <functional>

#include "event.hpp"
#include "Rendering/OpenGL/shader_program.hpp"
#include "Rendering/OpenGL/vertex_buffer.hpp"

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
		/*Фоновый цвет в формате RGBA*/
		float m_background_color[4] = { 0.7f, 0.7f, 0.7f, 0.f };

		/*Проиницализована ли библиотека GLFW*/
		static inline bool s_GLFW_initialized = false;
		
		/*Укалатель на программу с шейдерами*/
		std::unique_ptr<ShaderProgram> m_shader_program;
		/*Указатели на буферы*/
		std::unique_ptr<VertexBuffer> m_points_vb;
		std::unique_ptr<VertexBuffer> m_colors_vb;

		/*Идентификатор VertexArrayObject, который хранит все состояния, необходимые для предоставления данных о вершинах*/
		unsigned int m_vao;

	};
}