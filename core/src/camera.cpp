#include "camera.hpp"

#include <glm/trigonometric.hpp>

namespace waza3d {
	Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, const ProjectionMode projection_mode)
		:m_position(position), m_rotation(rotation), m_projection_mode(projection_mode)
	{
		updateViewMatrix();
		updateProjectionMatrix();
	}

	void Camera::setPosition(const glm::vec3& position)
	{
		m_position = position;
		updateViewMatrix();
	}

	void Camera::setRotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		updateViewMatrix();
	}

	void Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation)
	{
		m_position = position;
		m_rotation = rotation;
		updateViewMatrix();
	}

	void Camera::setProjectionMode(const ProjectionMode projection_mode)
	{
		m_projection_mode = projection_mode;
		updateProjectionMatrix();
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return m_view_matrix;
	}

	glm::mat4 Camera::getProjectionMatrix() const
	{
		return m_projection_matrix;
	}

	void Camera::updateViewMatrix()
	{
		glm::mat4 translate_matrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-m_position[0], -m_position[1], -m_position[2], 1);

		float rir[3] = { glm::radians(-m_rotation.x), glm::radians(-m_rotation.y), glm::radians(-m_rotation.z) }; //rotate_in_radians

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

		m_view_matrix = rotate_matrixZ * rotate_matrixY * rotate_matrixX * translate_matrix;
	}
		
	void Camera::updateProjectionMatrix()
	{
		if (m_projection_mode == ProjectionMode::Perspective)
		{
			/*Параметры фруструма*/
			float r = 0.1f; //расстояние до правой и левой грани
			float t = 0.1f;  //расстояние до верхней и нижней грани
			float f = 10;  //-f это расстояние от камеры до задней грани
			float n = 0.1f;  //-n это расстояние от камеры до переденей грани
			m_projection_matrix = glm::mat4(
				n / r, 0, 0, 0,
				0, n / t, 0, 0,
				0, 0, (-f - n) / (f - n), -1,
				0, 0, -2 * f * n / (f - n), 0
			);
		}
		else
		{
			/*Параметры фруструма*/
			float r = 2; //расстояние до правой и левой грани
			float t = 2; //расстояние до верхней и нижней грани
			float f = 100; //-f это расстояние от камеры до задней грани
			float n = 0.1f; //-n это расстояние от камеры до переденей грани
			m_projection_matrix = glm::mat4(
				1 / r, 0, 0, 0,
				0, 1 / t, 0, 0,
				0, 0, -2 / (f - n), 0,
				0, 0, (-f - n) / (f - n), 1
			);
		}
	}
}