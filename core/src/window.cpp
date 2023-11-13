#include "window.hpp"
#include "logs.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace waza3d {

    GLfloat points[] = {
        0.5f, 0.5f, 0.0f,
        0.4f, -0.25f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    
    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
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

    GLuint shader_program;
    GLuint vao;


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

        /*Подключаем программу шейдеров и VertexArrayObject*/
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        /*Отрисовываем, выбираем тип, смещение и число вертексов*/
        glDrawArrays(GL_TRIANGLES, 0, 3);

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

        /*Генерируем идентификатор вертексного шейдера*/
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        /*Задаем код шейдера и компилируем*/
        glShaderSource(vs, 1, &vertex_shader, nullptr);
        glCompileShader(vs);

        /*Генерируем идентификатор фрагментного шейдера*/
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        /*Задаем код шейдера и компилируем*/
        glShaderSource(fs, 1, &fragment_shader, nullptr);
        glCompileShader(fs);

        /*Генерируем идентификатор шейдерной программы*/
        shader_program = glCreateProgram();
        /*Линкуем у ней шейдеры*/
        glAttachShader(shader_program, vs);
        glAttachShader(shader_program, fs);
        glLinkProgram(shader_program);

        /*После создания программы шейдеры не нужны, удаляем их*/
        glDeleteShader(vs);
        glDeleteShader(fs);

        /*Генерируем буфер для передачи данных в видеокарту*/
        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);

        /*Подключаем буфер и назначаем текущим*/
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

        /*Отправлем данные в видеопамять и обозначаес что наши данные не будут меняться*/
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        /*Повторяем все вышеописанное для буфера цвета*/
        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        /*Генерируем и назначаем текущим VertexArrayObject*/
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        /*Связываем буферы с позицией в коде куда они пойдут в шейдеры
        для этого сначала активируем позицию*/
        glEnableVertexAttribArray(0);
        /*Сделаем нужный буфер активным*/
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        /*Связываем параметры: позиция, число вертексов, тип, нужно ли нормализовать, шаг смещения, смещение начала*/
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        /*Повторяем связывание для буфера цвета*/
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        return 0;  
	}

	void Window::shutdown()
	{
        glfwDestroyWindow(m_window);
        glfwTerminate();
	}

}