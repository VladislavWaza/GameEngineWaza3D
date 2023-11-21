#include <iostream>

#include <imgui/imgui.h>

#include <application.hpp>
#include <scene.hpp>
#include <camera.hpp>

class Editor : public waza3d::Application
{
	int frame = 0;
	float m_camera_pos[3] = { 0.f, 0.f, -1.f };
	float m_camera_rotation[3] = { 180.f, 0.f, 0.f };
	bool m_perspective_camera = false;
public:
	virtual void onUpdate() override
	{
		scene()->setCameraPerspective(m_perspective_camera);
		scene()->setCameraPosition(m_camera_pos[0], m_camera_pos[1], m_camera_pos[2]);
		scene()->setCameraRotation(m_camera_rotation[0], m_camera_rotation[1], m_camera_rotation[2]);
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
	waza3d::Scene firstScene;
	auto editor = std::make_unique<Editor>();

	int returnCode = editor->start(1024, 576, "Editor", &firstScene);

	std::cin.get();

	return returnCode;
}