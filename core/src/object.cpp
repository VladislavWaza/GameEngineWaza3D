#include "object.hpp"
#include "model.hpp"
#include "Rendering/OpenGL/render.hpp"

#include <glm/trigonometric.hpp>

namespace waza3d {
	Object::Object(const Model* model)
		: m_model(model)
	{
	}

	void Object::setScale(const float scaleX, const float scaleY, const float scaleZ)
	{
		m_scale[0] = scaleX;
		m_scale[1] = scaleY;
		m_scale[2] = scaleZ;
	}

	void Object::setRotation(const float rotationX, const float rotationY, const float rotationZ)
	{
		m_rotation[0] = rotationX;
		m_rotation[1] = rotationY;
		m_rotation[2] = rotationZ;
	}

	void Object::setTranslate(const float translateX, const float translateY, const float translateZ)
	{
		m_translate[0] = translateX;
		m_translate[1] = translateY;
		m_translate[2] = translateZ;
	}

	void Object::draw() const
	{
		Render::draw(*m_model);
	}

	glm::mat4 Object::getTranslateMatrix() const
	{
		glm::mat4 translate_matrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			m_translate[0], m_translate[1], m_translate[2], 1);
		return translate_matrix;
	}

	glm::mat4 Object::getRotateMatrix() const
	{
		float rir[3] = { //rir - rotate_in_radians 
			glm::radians(m_rotation[0]), 
			glm::radians(m_rotation[1]), 
			glm::radians(m_rotation[2]) }; 
		glm::mat4 rotate_matrixX(
			1, 0, 0, 0,
			0, cos(rir[0]), -sin(rir[0]), 0,
			0, sin(rir[0]), cos(rir[0]), 0,
			0, 0, 0, 1
		);

		glm::mat4 rotate_matrixY(
			cos(rir[1]), 0, sin(rir[1]), 0,
			0, 1, 0, 0,
			-sin(rir[1]), 0, cos(rir[1]), 0,
			0, 0, 0, 1
		);

		glm::mat4 rotate_matrixZ(
			cos(rir[2]), -sin(rir[2]), 0, 0,
			sin(rir[2]), cos(rir[2]), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		return rotate_matrixX * rotate_matrixY * rotate_matrixZ;
	}

	glm::mat4 Object::getScaleMatrix() const
	{
		glm::mat4 scale_matrix(
			m_scale[0], 0, 0, 0,
			0, m_scale[1], 0, 0,
			0, 0, m_scale[2], 0,
			0, 0, 0, 1);
		return scale_matrix;
	}
}