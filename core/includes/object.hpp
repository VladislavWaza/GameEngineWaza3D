#pragma once

#include <glm/mat4x4.hpp>

namespace waza3d {
	
	class Model;

	/*Класс хранит данные о трансформации и указатель на модель, также обеспечивает отрисовку модели*/
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

		glm::mat4 getTranslateMatrix() const;
		glm::mat4 getRotateMatrix() const;
		glm::mat4 getScaleMatrix() const;
		void draw() const;
	};


}