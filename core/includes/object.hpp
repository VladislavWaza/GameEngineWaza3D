#pragma once

#include <glm/mat4x4.hpp>

namespace waza3d {
	
	class Model;

	class Object {
		const Model* m_model = nullptr;
		float m_scale[3] = { 1.f, 1.f, 1.f };
		float m_rotation[3] = { 0.f, 0.f, 0.f };
		float m_translate[3] = { 0.f, 0.f, 0.f };
	public:
		Object(const Model* model);

		void setScale(const float scaleX, const float scaleY, const float scaleZ);
		void setRotation(const float rotationX, const float rotationY, const float rotationZ);
		void setTranslate(const float translateX, const float translateY, const float translateZ);

		glm::mat4 getTranslateMatrix();
		glm::mat4 getRotateMatrix();
		glm::mat4 getScaleMatrix();
		const Model* getModel();
	};


}