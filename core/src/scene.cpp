#include "scene.hpp"
#include "model.hpp"
#include "Rendering/OpenGL/shader_program.hpp"
#include "logs.hpp"

namespace waza3d {
	Scene::Scene()
	{
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
		for (auto& elem : m_objs)
		{
			const Object& obj = elem.second;
			obj.draw();
			shader_program.setMatrix4("model_matrix", 
				obj.getTranslateMatrix() * obj.getRotateMatrix() * obj.getScaleMatrix());
		}
	}
}