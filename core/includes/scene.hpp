#pragma once
#include <unordered_map>
#include <string>
#include <memory>

#include "object.hpp"

namespace waza3d {
	class Model;
	class ShaderProgram;
	/*Класс хранит объекты, обеспечивает отрисовку объектов*/
	class Scene {
		std::unordered_map<std::string, Object> m_objs;
	public:
		Scene();
		bool addObject(const std::string& name, Model* model);
		Object& getObject(const std::string& name);
		void draw(const ShaderProgram& shader_program) const;
	};
}