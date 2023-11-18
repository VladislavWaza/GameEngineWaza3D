#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace waza3d {
	
	/*Класс камеры, генерирует матрицы преобразований*/
	class Camera {

	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

		/*Позиция камеры, Поворот в углах, Режим проекции*/
		Camera(
			const glm::vec3& position = { 0, 0, 0 },
			const glm::vec3& rotation = { 0, 0, 0 },
			const ProjectionMode projection_mode = ProjectionMode::Perspective);

		/*Устанавливаем позицию и обновляем матрицу View*/
		void setPosition(const glm::vec3& position);
		/*Устанавливаем углы поворота и обновляем матрицу View*/
		void setRotation(const glm::vec3& rotation);
		/*Устанавливаем позицию и углы поворота и обновляем матрицу View*/
		void setPositionRotation(const glm::vec3& position, const glm::vec3& rotation);
		/*Устанавливаем режим проекции и обновляем матрицу Projection*/
		void setProjectionMode(const ProjectionMode projection_mode);
		/*Возвращает матрицу View*/
		glm::mat4 getViewMatrix() const;
		/*Возвращает матрицу Projection*/
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