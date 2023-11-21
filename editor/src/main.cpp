#include <iostream>

#include <imgui/imgui.h>

#include <application.hpp>

class Editor : public waza3d::Application
{
	int frame = 0;
public:
	virtual void onUpdate() override
	{	
	}

	virtual void onUIDraw() override
	{
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("Camera Position", m_camera_pos, -3.f, 3.f);
		ImGui::SliderFloat3("Camera Rotation", m_camera_rotation, 0.f, 360.f);
		ImGui::Checkbox("Perspective camera", &m_perspective_camera);
		ImGui::End();
	}
};


int main()
{
	auto editor = std::make_unique<Editor>();

	int returnCode = editor->start(1024, 576, "Editor");

	std::cin.get();

	return returnCode;
}