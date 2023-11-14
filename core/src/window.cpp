#include "window.hpp"
#include "logs.hpp"
#include "event.hpp"
#include "Rendering/OpenGL/shader_program.hpp"
#include "Rendering/OpenGL/vertex_buffer.hpp"
#include "Rendering/OpenGL/vertex_array.hpp"
#include "Rendering/OpenGL/index_buffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace waza3d {


    GLfloat points_colors_for_index_buf[] = {
        0.4f, -0.25f, 0.0f,     0.3f, 0.75f, 0.63f,
        0.5f, 0.5f, 0.0f,       0.7f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.5f,
        -0.4f, 0.25f, 0.0f,     0.6f, 0.0f, 0.6f,
    };

    GLuint indexes[] = {
        0, 1, 2, 3, 2, 1
    };

    /*Код шейдеров на языке GLSL*/
    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(color, 1.0);"
        "}";

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
        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        /*Получаем хендл*/
        ImGuiIO& io = ImGui::GetIO();
        /*Задаем размеры*/
        io.DisplaySize.x = static_cast<float>(m_data.m_width);
        io.DisplaySize.y = static_cast<float>(m_data.m_height);

        /*Начинаем новый кадр*/
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        /*Рисуем демо окно*/
        //ImGui::ShowDemoWindow();

        ImGui::Begin("Background color window");
        ImGui::ColorEdit4("Background color", m_background_color);

        ImGui::End();

        /*Подключаем программу шейдеров и VertexArray*/
        m_shader_program->bind();
        m_points_colors_va->bind();
        /*Отрисовываем, выбираем тип, число вертексов, тип, указатель на индексы(nullptr так как он уже загружен в видеопамять)*/
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_points_colors_va->indexesCount()), GL_UNSIGNED_INT, nullptr);

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

        /* Initialize the library */
        if (!s_GLFW_initialized)
        {
            if (!glfwInit())
            {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            LOG_CRITICAL("Can't create window '{0}'!", m_data.m_title);
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        /*Инициализируем GLAD, для связывания всех функций OpenGL*/
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -1;
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
                glViewport(0, 0, width, height);
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

        /*Генерируем буфер для передачи данных в видеокарту*/
        m_points_colors_vb = std::make_unique<VertexBuffer>(
            points_colors_for_index_buf, sizeof(points_colors_for_index_buf), buffer_layout_2vec3, VertexBuffer::UsageType::Static
            );
        /*Генерируем индексный буфер*/
        m_points_colors_ib = std::make_unique<IndexBuffer>(
            indexes, sizeof(indexes)/sizeof(GLuint), VertexBuffer::UsageType::Static
            );


        /*Генерируем и назначаем текущим VertexArray*/
        m_points_colors_va = std::make_unique<VertexArray>();

        /*Связываем буферы c массивом*/
        m_points_colors_va->addVertexBuffer(*m_points_colors_vb);
        m_points_colors_va->setIndexBuffer(*m_points_colors_ib);
        return 0;  
	}

	void Window::shutdown()
	{
        glfwDestroyWindow(m_window);
        glfwTerminate();
	}

}