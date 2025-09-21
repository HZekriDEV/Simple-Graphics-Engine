#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Camera
{
	public:
		Camera(const glm::vec3& position, float fov, float pitch, float yaw);

		// Setters for transformations
		void SetPosition(const glm::vec3& position);
		void SetPitch(float pitch);
		void SetYaw(float yaw);
		void SetFOV(float fov);
		void SetFrontVector(const glm::vec3& front);

		glm::vec3 Position() const;
		glm::vec3 Front() const;
		glm::vec3 Up() const;
		float Pitch() const;
		float Yaw() const;
		float FOV() const;
		glm::mat4 ViewMatrix() const;

		int screenWidth;
		int screenHeight;

	private:
		glm::vec3 m_cameraPos;
		glm::vec3 m_cameraFront;
		glm::vec3 m_cameraUp;
		float m_fov;
		float m_pitch;
		float m_yaw;
};

#endif
