#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace waza3d {

	class Camera {

	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

		Camera(
			const glm::vec3& position = { 0, 0, 0 },
			const glm::vec3& rotation = { 0, 0, 0 },
			const ProjectionMode projection_mode = ProjectionMode::Perspective);

		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);
		void setPositionRotation(const glm::vec3& position, const glm::vec3& rotation);
		void setProjectionMode(const ProjectionMode projection_mode);

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

	private:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		ProjectionMode m_projection_mode;
		glm::mat4 m_view_matrix;
		glm::mat4 m_projection_matrix;

		void updateViewMatrix();
		void updateProjectionMatrix();
	};

}