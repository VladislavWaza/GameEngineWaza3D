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
		/*Движение камеры вдоль её собственных осей и поворот вокруг осей эйлера*/
		void addMovementRotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta);
		/*Движение камеры вдоль её собственных осей*/
		void moveForward(const	float delta);
		void moveRight(const float delta);
		void moveUp(const float delta);
		/*Движение камеры вдоль мировых осей*/
		void moveWorldUp(const float delta);
		/*Устанавливаем режим проекции и обновляем матрицу Projection*/
		void setProjectionMode(const ProjectionMode projection_mode);
		/*Возвращает матрицу View*/
		const glm::mat4& getViewMatrix();
		/*Возвращает матрицу Projection*/
		const glm::mat4& getProjectionMatrix() const;

		const glm::vec3& getPosition() const;
		const glm::vec3& getRotation() const;
		const ProjectionMode getProjectionMode() const;

	private:
		glm::vec3 m_position;
		glm::vec3 m_rotation; //X - roll, Y - pitch, Z - yaw
		ProjectionMode m_projection_mode;
		glm::mat4 m_view_matrix;
		glm::mat4 m_projection_matrix;

		glm::vec3 m_direction;
		glm::vec3 m_right;
		glm::vec3 m_up;

		bool m_update_view_matrix = false;

		static glm::vec3 s_world_up;
		static glm::vec3 s_world_right;
		static glm::vec3 s_world_forward;

		void updateViewMatrix();
		void updateProjectionMatrix();
		/*Возвращает углы в диапазон 0-360*/
		void remainderRotation();
	};

}