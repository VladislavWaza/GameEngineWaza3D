#pragma once
#include <unordered_map>
#include <string>
#include <memory>

#include "object.hpp"

namespace waza3d {
	class Camera;
	class Model;
	class ShaderProgram;
	/*Класс хранит объекты и камеру, обеспечивает отрисовку объектов*/
	class Scene {
		std::unordered_map<std::string, Object> m_objs;
		std::unique_ptr<Camera> m_camera;
		float m_camera_pos[3] = { 0.f, 0.f, 0.f };
		float m_camera_rotation[3] = { 0.f, 0.f, 0.f };
		bool m_perspective_camera = false;
	public:
		Scene();
		bool addObject(const std::string& name, Model* model);
		Object& getObject(const std::string& name);
		void draw(const ShaderProgram& shader_program) const;

		void setCameraPosition(const float x, const float y, const float z);
		void setCameraRotation(const float x, const float y, const float z);
		void setCameraPerspective(bool mode);
	};
}