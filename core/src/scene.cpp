#include "scene.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "Rendering/OpenGL/shader_program.hpp"
#include "logs.hpp"

namespace waza3d {
	Scene::Scene()
	{
		m_camera = std::make_unique<Camera>();
	}

	bool Scene::addObject(const std::string& name, Model* model)
	{
		bool res = m_objs.emplace(name, model).second;
		if (!res)
			LOG_ERROR("Object {0} not added to Scene!", name);
		return res;
	}
	Object& Scene::getObject(const std::string& name)
	{
		return m_objs.at(name);
	}
	void Scene::draw(const ShaderProgram& shader_program) const
	{
		shader_program.bind();

		m_camera->setPositionRotation(
			glm::vec3(m_camera_pos[0], m_camera_pos[1], m_camera_pos[2]),
			glm::vec3(m_camera_rotation[0], m_camera_rotation[1], m_camera_rotation[2])
		);
		m_camera->setProjectionMode(
			m_perspective_camera ? 
			Camera::ProjectionMode::Perspective : 
			Camera::ProjectionMode::Orthographic);

		shader_program.setMatrix4("camera_matrix",
			m_camera->getProjectionMatrix() * m_camera->getViewMatrix());

		for (auto& elem : m_objs)
		{
			const Object& obj = elem.second;
			obj.draw();
			shader_program.setMatrix4("model_matrix", 
				obj.getTranslateMatrix() * obj.getRotateMatrix() * obj.getScaleMatrix());
		}
	}
	void Scene::setCameraPosition(const float x, const float y, const float z)
	{
		m_camera_pos[0] = x;
		m_camera_pos[1] = y;
		m_camera_pos[2] = z;
	}
	void Scene::setCameraRotation(const float x, const float y, const float z)
	{
		m_camera_rotation[0] = x;
		m_camera_rotation[1] = y;
		m_camera_rotation[2] = z;
	}
	void Scene::setCameraPerspective(bool is_perspective)
	{
		m_perspective_camera = is_perspective;
	}
}