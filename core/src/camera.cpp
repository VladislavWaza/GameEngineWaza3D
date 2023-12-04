#include "camera.hpp"

#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace waza3d {

	glm::vec3 Camera::s_world_up = glm::vec3(0.f, 0.f, 1.f );
	glm::vec3 Camera::s_world_right = glm::vec3(0.f, -1.f, 0.f);
	glm::vec3 Camera::s_world_forward = glm::vec3(1.f, 0.f, 0.f);


	Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, const ProjectionMode projection_mode)
		:m_position(position), m_rotation(rotation), m_projection_mode(projection_mode)
	{
		m_update_view_matrix = true;
		updateProjectionMatrix();
	}

	void Camera::setPosition(const glm::vec3& position)
	{
		m_position = position;
		m_update_view_matrix = true;
	}

	void Camera::setRotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		m_update_view_matrix = true;
	}

	void Camera::setPositionRotation(const glm::vec3& position, const glm::vec3& rotation)
	{
		m_position = position;
		m_rotation = rotation;
		m_update_view_matrix = true;
	}

	void Camera::addMovementRotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta)
	{
		m_position += m_direction * movement_delta.x;
		m_position += m_right * movement_delta.y;
		m_position += m_up * movement_delta.z;
		m_rotation += rotation_delta;
		m_update_view_matrix = true;
	}

	void Camera::setProjectionMode(const ProjectionMode projection_mode)
	{
		m_projection_mode = projection_mode;
		updateProjectionMatrix();
	}

	const glm::mat4& Camera::getViewMatrix()
	{
		if (m_update_view_matrix)
		{
			updateViewMatrix();
			m_update_view_matrix = false;
		}
		return m_view_matrix;
	}

	const glm::mat4& Camera::getProjectionMatrix() const
	{
		return m_projection_matrix;
	}

	const glm::vec3& Camera::getPosition() const
	{
		return m_position;
	}

	const glm::vec3& Camera::getRotation() const
	{
		return m_rotation;
	}

	const Camera::ProjectionMode Camera::getProjectionMode() const
	{
		return m_projection_mode;
	}

	void Camera::moveForward(const float delta)
	{
		m_position += m_direction * delta;
		m_update_view_matrix = true;
	}

	void Camera::moveRight(const float delta)
	{
		m_position += m_right * delta;
		m_update_view_matrix = true;
	}

	void Camera::moveUp(const float delta)
	{
		m_position += m_up * delta;
		m_update_view_matrix = true;
	}

	void Camera::moveWorldUp(const float delta)
	{
		m_position += s_world_up * delta;
		m_update_view_matrix = true;
	}

	void Camera::updateViewMatrix()
	{
		remainderRotation();
		float roll_in_radians = glm::radians(m_rotation.x);
		float pitch_in_radians = glm::radians(m_rotation.y);
		float yaw_in_radians = glm::radians(m_rotation.z);

		glm::mat3 rotate_matrixX(
			1, 0, 0,
			0, cos(yaw_in_radians), sin(yaw_in_radians),
			0, -sin(yaw_in_radians), cos(yaw_in_radians)
		);

		glm::mat3 rotate_matrixY(
			cos(pitch_in_radians), 0, -sin(pitch_in_radians),
			0, 1, 0,
			sin(pitch_in_radians), 0, cos(pitch_in_radians)
		);

		glm::mat3 rotate_matrixZ(
			cos(roll_in_radians), sin(roll_in_radians), 0,
			-sin(roll_in_radians), cos(roll_in_radians), 0,
			0, 0, 1
		);

		glm::mat3 euler_matrix = rotate_matrixZ * rotate_matrixX * rotate_matrixY;
		m_direction = glm::normalize(euler_matrix * s_world_forward);
		m_right = glm::normalize(euler_matrix * s_world_right);
		m_up = glm::cross(m_right, m_direction);

		m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);
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
	void Camera::remainderRotation()
	{
		if (m_rotation.x >= 360)
			m_rotation.x = fmod(m_rotation.x, 360);
		if (m_rotation.x < 0)
			m_rotation.x = fmod(m_rotation.x, 360) + 360;

		if (m_rotation.y >= 360)
			m_rotation.y = fmod(m_rotation.y, 360);
		if (m_rotation.y < 0)
			m_rotation.y = fmod(m_rotation.y, 360) + 360;

		if (m_rotation.z >= 360)
			m_rotation.z = fmod(m_rotation.z, 360);
		if (m_rotation.z < 0)
			m_rotation.z = fmod(m_rotation.z, 360) + 360;
	}
}