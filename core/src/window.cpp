#include "window.hpp"
#include "logs.hpp"
#include "event.hpp"
#include "Rendering/OpenGL/shader_program.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "Rendering/OpenGL/render.hpp"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

namespace waza3d {

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

    float scale[3] = { 1.f, 1.f, 1.f};
    float rotate[3] = { 0.f, 0.f, 0.f };
    float translate[3] = { 0.f, 0.f, 0.f };

    float camera_pos[3] = { 0.f, 0.f, -1.f };
    float camera_rotation[3] = { 180.f, 0.f, 0.f };
    bool perspective_camera = false;
    Camera camera;


	Window::Window(unsigned int width, unsigned int height, const std::string& title)
        :m_data({ width, height, title })
	{
		int resCode = init();

        /*Инициализируем ImGui*/
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();

        /*Подключем обработчик встроенный событий
          Встроенные события будут обрабатывать ПОСЛЕ заданных мной*/
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	}

	Window::~Window()
	{
		shutdown();
	}

	void Window::onUpdate()
	{
        /*Чистим окно заливая своим цветом*/
        Render::setClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        Render::clear();

        /*Получаем хендл ImGui*/
        ImGuiIO& io = ImGui::GetIO();
        /*Задаем размеры*/
        io.DisplaySize.x = static_cast<float>(m_data.m_width);
        io.DisplaySize.y = static_cast<float>(m_data.m_height);

        /*Начинаем новый кадр интерфейса ImGui*/
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Background color window");
        ImGui::ColorEdit4("Background color", m_background_color);
        ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);
        ImGui::SliderFloat3("Rotation", rotate, 0.f, 360.f);
        ImGui::SliderFloat3("Translate", translate, -1.f, 1.f);

        ImGui::SliderFloat3("Camera Position", camera_pos, -3.f, 3.f);
        ImGui::SliderFloat3("Camera Rotation", camera_rotation, 0.f, 360.f);
        ImGui::Checkbox("Perspective camera", &perspective_camera);

        ImGui::End();

        /*Подключаем программу шейдеров и рисуем модель*/
        m_shader_program->bind();
        Render::draw(*m_model);

        /*Совершаем трансформации над объектом и задаем матрицу трансформаций в шейдер*/
        glm::mat4 scale_matrix(
            scale[0], 0, 0, 0,
            0, scale[1], 0, 0,
            0, 0, scale[2], 0,
            0, 0, 0, 1);

        float rir[3] = { glm::radians(rotate[0]), glm::radians(rotate[1]), glm::radians(rotate[2])}; //rotate_in_radians
        glm::mat4 rotate_matrixX(
            1, 0, 0, 0,
            0, cos(rir[0]), -sin(rir[0]), 0,
            0, sin(rir[0]), cos(rir[0]), 0,
            0, 0, 0, 1
        );

        glm::mat4 rotate_matrixY(
            cos(rir[1]), 0, sin(rir[1]), 0,
            0, 1, 0, 0,
            -sin(rir[1]), 0, cos(rir[1]), 0,
            0, 0, 0, 1
        );

        glm::mat4 rotate_matrixZ(
            cos(rir[2]), -sin(rir[2]), 0, 0,
            sin(rir[2]), cos(rir[2]), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );

        glm::mat4 translate_matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translate[0], translate[1], translate[2], 1);

        camera.setPositionRotation(
            glm::vec3(camera_pos[0], camera_pos[1], camera_pos[2]), 
            glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2])
        );
        camera.setProjectionMode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
        
        m_shader_program->setMatrix4("model_matrix", translate_matrix * rotate_matrixX * rotate_matrixY * rotate_matrixZ * scale_matrix);
        m_shader_program->setMatrix4("camera_matrix", camera.getProjectionMatrix() * camera.getViewMatrix());


        /*Реденерим*/
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(m_window);
        glfwPollEvents();
	}

	unsigned int Window::width() const
	{
		return m_data.m_width;
	}

	unsigned int Window::height() const
	{
		return m_data.m_height;
	}

    void Window::setEventCallback(const EventCallbackFun& callback)
    {
        m_data.m_event_callback_fun = callback;
    }

	int Window::init()
	{
        LOG_INFO("Creating window '{0}' with size {1}x{2}", m_data.m_title, m_data.m_width, m_data.m_height);

        /*Добавляем вывод лога GLFW при ошибке*/
        glfwSetErrorCallback([](int error_code, const char* description)
            {
                LOG_CRITICAL("GLFW error: {0}", description);
            }
        );


        /* Инициализируем GLFW */
        if (!glfwInit())
        {
            LOG_CRITICAL("Can't initialize GLFW!");
            return -1;
        }

        /* Создаем окно GLFW*/
        m_window = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            LOG_CRITICAL("Can't create window '{0}'!", m_data.m_title);
            return -2;
        }

        if (!Render::init(m_window))
        {
            LOG_CRITICAL("Can't initialize Render!");
            return -3;
        }

        /*Кладем пользовательские данные m_data в GLFW окно m_window*/
        glfwSetWindowUserPointer(m_window, &m_data);

        /*Эта лямбда-функция будет вызываться каждый раз когда происходит встроенное в GLFW изменение размеров окна*/
        glfwSetWindowSizeCallback(m_window, 
            [](GLFWwindow* window, int width, int height)
            {
                /*Получаем ссылку на пользовательские данные m_data из GLFW окна m_window*/
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                
                /*Запоминем новые размеры окна*/
                data.m_width = width;
                data.m_height = height;

                /*Вызываем функцию-обработчик заданную извне*/
                EventWindowResize event(width, height);
                data.m_event_callback_fun(event);
            }
        );

        /*Эта лямбда-функция будет вызываться каждый раз когда происходит встроенное в GLFW событие изменения положение мыши*/
        glfwSetCursorPosCallback(m_window,
            [](GLFWwindow* window, double x, double y)
            {
                /*Позиция курсора относительно верхнего левого края*/

                /*Получаем ссылку на пользовательские данные m_data из GLFW окна m_window*/
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                /*Вызываем функцию-обработчик заданную извне*/
                EventMouseMoved event(x,y);
                data.m_event_callback_fun(event);
            }
        );

        /*Эта лямбда-функция будет вызываться каждый раз когда происходит встроенное в GLFW событие закрытия окна*/
        glfwSetWindowCloseCallback(m_window, 
            [](GLFWwindow* window)
            {
                /*Получаем ссылку на пользовательские данные m_data из GLFW окна m_window*/
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                EventWindowClose event;
                data.m_event_callback_fun(event);
            }
        );

        /*Указыаем границы буфера кадра(в пикселях) для отрисовки с помощью callback
        Эти размеры могу отличатся от размеров окна для дисплея Retina*/
        glfwSetFramebufferSizeCallback(m_window, 
            [](GLFWwindow* window, int width, int height)
            {
                /*Устанавливаем что хотим перерисовывать*/
                Render::setViewport(width, height, 0, 0);
            }
        );

        /*Создаем шейдерную программу*/
        m_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!m_shader_program->isCompiled())
        {
            LOG_CRITICAL("Failed to compile shader program");
            return -1;
        }

        /*Создаем Layout буфера, в котором друг за другом идет позиция и цвет*/
        BufferLayout buffer_layout_2vec3{
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };

        m_model = new Model( {
            0.f, 0.43f, 0.0f,       0.7f, 0.f, 0.f,     //0
            -0.215f, 0.0f, 0.0f,    0.f, 0.7f, 0.f,     //1
            0.215f, 0.0f, 0.0f,     0.f, 0.f, 0.7f,     //2
            0.f, 0.143f, 0.43f,     0.7f, 0.7f, 0.7f,   //3
        }, {
            0, 1, 2, 0, 1, 3, 1, 2, 3, 0, 2, 3 //indexes
        }, buffer_layout_2vec3, VertexBuffer::UsageType::Static, VertexBuffer::UsageType::Static);



        return 0;  
	}

	void Window::shutdown()
	{
        glfwDestroyWindow(m_window);
        ImGui::DestroyContext();
        glfwTerminate();
	}

}