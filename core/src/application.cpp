#include "application.hpp"
#include "logs.hpp"
#include "window.hpp"
#include "Rendering/OpenGL/shader_program.hpp"
#include "Rendering/OpenGL/vertex_buffer.hpp"
#include "Rendering/OpenGL/render.hpp"
#include "Modules/UI_module.hpp"
#include "model.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "input.hpp"
#include "camera.hpp"

#include <imgui/imgui.h>

namespace waza3d {
	Application::Application()
	{
        LOG_INFO("Starting application!");
	}

	Application::~Application()
	{
		LOG_INFO("Closing application!");
	}

	int Application::start(unsigned int width, unsigned int height, const char* title, Scene* scene)
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

		m_event_dispatcher.addEventListener<EventKeyPressed>(
			[&](EventKeyPressed& e)
			{
				if (static_cast<char>(e.m_key_code) <= 'Z' && static_cast<char>(e.m_key_code) >= 'A')
					LOG_INFO("[KeyPressed] {0} with code {1}", static_cast<char>(e.m_key_code), static_cast<int>(e.m_key_code));
				else
					LOG_INFO("[KeyPressed] with code {0}", static_cast<int>(e.m_key_code));
				Input::PressKey(e.m_key_code);
			}
		);

		m_event_dispatcher.addEventListener<EventKeyReleased>(
			[&](EventKeyReleased& e)
			{
				if (static_cast<char>(e.m_key_code) <= 'Z' && static_cast<char>(e.m_key_code) >= 'A')
					LOG_INFO("[KeyReleased] {0} with code {1}", static_cast<char>(e.m_key_code), static_cast<int>(e.m_key_code));
				else
					LOG_INFO("[KeyReleased] with code {0}", static_cast<int>(e.m_key_code));
				Input::ReleaseKey(e.m_key_code);
			}
		);

		/*Эта лямбда будет вызываться каждый раз когда происходит какой-либо ивент*/
		m_window->setEventCallback(
			[&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			}
		);

		/*Код шейдеров на языке GLSL*/
		const char* vertex_shader =
			"#version 460\n"
			"layout(location = 0) in vec3 vertex_position;"
			"layout(location = 1) in vec3 vertex_color;"
			"uniform mat4 model_matrix;" //uniform одинаков для всех вызовов шейдера
			"uniform mat4 camera_matrix;" //uniform одинаков для всех вызовов шейдера 
			"out vec3 color;"
			"void main() {"
			"   color = vertex_color;"
			"   gl_Position = camera_matrix * model_matrix * vec4(vertex_position, 1.0);"
			"}";

		const char* fragment_shader =
			"#version 460\n"
			"in vec3 color;"
			"out vec4 frag_color;"
			"void main() {"
			"   frag_color = vec4(color, 1.0);"
			"}";

		/*Создаем шейдерную программу*/
		m_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
		if (!m_shader_program->isCompiled())
		{
			LOG_CRITICAL("Failed to compile shader program");
			return -1;
		}


		m_camera = std::make_unique<Camera>();

		m_scene = scene;

		/*Создаем Layout буфера, в котором друг за другом идет позиция и цвет*/
		BufferLayout buffer_layout_2vec3{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};

		Model* model = new Model({
			0.f,	0.43f, 0.0f, 	0.7f, 0.f,	 0.f,     //0
			-0.215f, 0.0f, 0.0f,	0.f, 0.7f,	 0.f,     //1
			0.215f, 0.0f,  0.0f, 	 0.f, 0.f,	 0.7f,	//2
			 0.f, 0.143f,  0.43f,	 0.7f, 0.7f, 0.7f,	//3
			}, {
				0, 1, 2, 0, 1, 3, 1, 2, 3, 0, 2, 3 //indexes
			}, buffer_layout_2vec3, VertexBuffer::UsageType::Static, VertexBuffer::UsageType::Static);

		m_scene->addObject("piramid", model);

		/*Фоновый цвет в формате RGBA*/
		float background_color[4] = { 0.7f, 0.7f, 0.7f, 0.f };
		float scale[3] = {1.f, 1.f, 1.f};
		float rotate[3] = { 0.f, 0.f, 0.f };
		float translate[3] = { 0.f, 0.f, 0.f };

		beforeStart();
		while (!m_close_window)
		{
			/*Чистим окно заливая своим цветом*/
			Render::setClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
			Render::clear();

			/*Подключаем программу шейдеров и рисуем cцену*/
			m_shader_program->bind();
			m_shader_program->setMatrix4("camera_matrix",
				m_camera->getProjectionMatrix() * m_camera->getViewMatrix());
			m_scene->draw(*m_shader_program);

			m_scene->getObject("piramid").setScale(scale[0], scale[1], scale[2]);
			m_scene->getObject("piramid").setRotation(rotate[0], rotate[1], rotate[2]);
			m_scene->getObject("piramid").setTranslate(translate[0], translate[1], translate[2]);
			/************************************************************************************************************/
			/*                                              Интерфейс                                                   */
			/************************************************************************************************************/
			/*Начинаем новый кадр интерфейса*/
			UIModule::newFrame();

			ImGui::Begin("Background color window");
			ImGui::ColorEdit4("Background color", background_color);
			ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);
			ImGui::SliderFloat3("Rotation", rotate, 0.f, 360.f);
			ImGui::SliderFloat3("Translate", translate, -1.f, 1.f);
			ImGui::End();

			onUIDraw();
			/*Реденерим интерфейс*/
			UIModule::draw();


			m_window->onUpdate(); 
			onUpdate();
		}
		m_window = nullptr;
        return 0;
	}
}