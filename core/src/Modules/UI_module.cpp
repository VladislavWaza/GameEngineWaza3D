#include "UI_module.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <GLFW/glfw3.h>

namespace waza3d {
	void UIModule::init(GLFWwindow* window)
	{
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        /*Получаем хендл ImGui*/
        ImGuiIO& io = ImGui::GetIO();
        /*Активируем докинг*/
        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
        /*Активируем вьюпорты*/
        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;

        /*Инициализируем ImGui*/
        ImGui_ImplOpenGL3_Init();

        /*Подключем обработчик встроенный событий
          Встроенные события будут обрабатывать ПОСЛЕ заданных мной*/
        ImGui_ImplGlfw_InitForOpenGL(window, true);
	}
    void UIModule::shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void UIModule::newFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void UIModule::draw()
    {
        /*Реденерим интерфейс*/
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /**/
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}