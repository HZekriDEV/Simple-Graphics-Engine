#include "config.h"

Camera::Camera(const glm::vec3& position, float fov, float pitch, float yaw)
{
	m_cameraPos = position;
	m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_fov = fov;
	m_pitch = pitch;
	m_yaw = yaw;
}

void Camera::SetPosition(const glm::vec3& position)
{
	m_cameraPos = position;
}

void Camera::SetPitch(float pitch)
{
	m_pitch = pitch;
}

void Camera::SetYaw(float yaw)
{
	m_yaw = yaw;
}

void Camera::SetFOV(float fov)
{
	m_fov = fov;
}

void Camera::SetFrontVector(const glm::vec3& front)
{
	m_cameraFront = front;
}

glm::vec3 Camera::Position() const
{
	return m_cameraPos;
}

glm::vec3 Camera::Front() const
{
	return m_cameraFront;
}

glm::vec3 Camera::Up() const
{
	return m_cameraUp;
}

glm::mat4 Camera::ViewMatrix() const
{
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

float Camera::FOV() const
{
	return m_fov;
}

float Camera::Pitch() const
{
	return m_pitch;
}

float Camera::Yaw() const
{
	return m_yaw;
}