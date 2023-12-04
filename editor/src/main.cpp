#include <iostream>

#include <imgui/imgui.h>

#include <application.hpp>
#include <scene.hpp>
#include <camera.hpp>
#include <input.hpp>

class Editor : public waza3d::Application
{
	int frame = 0;
	bool m_perspective_camera = true;
	float m_camera_pos[3];
	float m_camera_rotation[3];
public:
	virtual void beforeStart() override
	{
		m_camera->setPositionRotation(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 90.f, 0.f));
		m_camera->setProjectionMode(m_perspective_camera ? waza3d::Camera::ProjectionMode::Perspective
			: waza3d::Camera::ProjectionMode::Orthographic);
	}


	virtual void onUpdate() override
	{
		bool move_camera = false;
		glm::vec3 movement_delta{ 0, 0, 0 };
		glm::vec3 rotation_delta{ 0, 0, 0 };
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_W))
		{
			movement_delta.z += 0.005f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_S))
		{
			movement_delta.z -= 0.005f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_D))
		{
			movement_delta.y += 0.005f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_A))
		{
			movement_delta.y -= 0.005f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_E))
		{
			movement_delta.x += 0.005f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_Q))
		{
			movement_delta.x -= 0.005f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_UP))
		{
			rotation_delta.y -= 0.5f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_DOWN))
		{
			rotation_delta.y += 0.5f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_RIGHT))
		{
			rotation_delta.z -= 0.5f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_LEFT))
		{
			rotation_delta.z += 0.5f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_Z))
		{
			rotation_delta.x -= 0.5f;
			move_camera = true;
		}
		if (waza3d::Input::isKeyPressed(waza3d::KeyCode::KEY_X))
		{
			rotation_delta.x += 0.5f;
			move_camera = true;
		}
		if (move_camera)
			m_camera->addMovementRotation(movement_delta, rotation_delta);
	}

	virtual void onUIDraw() override
	{
		m_camera_pos[0] = m_camera->getPosition().x;
		m_camera_pos[1] = m_camera->getPosition().y;
		m_camera_pos[2] = m_camera->getPosition().z;

		m_camera_rotation[0] = m_camera->getRotation().x;
		m_camera_rotation[1] = m_camera->getRotation().y;
		m_camera_rotation[2] = m_camera->getRotation().z;

		m_perspective_camera = (m_camera->getProjectionMode() == waza3d::Camera::ProjectionMode::Perspective);

		ImGui::Begin("Editor");
		if (ImGui::SliderFloat3("Camera Position", m_camera_pos, -3.f, 3.f))
		{
			m_camera->setPosition(glm::vec3(m_camera_pos[0], m_camera_pos[1], m_camera_pos[2]));
		}
		if (ImGui::SliderFloat3("Camera Rotation", m_camera_rotation, 0.f, 360.f))
		{
			m_camera->setRotation(glm::vec3(m_camera_rotation[0], m_camera_rotation[1], m_camera_rotation[2]));
		}
		if (ImGui::Checkbox("Perspective camera", &m_perspective_camera))
		{
			m_camera->setProjectionMode(m_perspective_camera ? waza3d::Camera::ProjectionMode::Perspective
				: waza3d::Camera::ProjectionMode::Orthographic);
		}	
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