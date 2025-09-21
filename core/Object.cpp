#include "Object.h"

Object::Object(const std::string name) : name(name)
{
	ID = 1;
}

void Object::SetMesh(const Mesh& mesh)
{
	m_mesh = mesh;
}
void Object::SetMesh(const Model& model)
{
	m_model = model;
}
void Object::SetShader(const Shader& shader)
{
	m_shader = shader;
	if (isPrimitive)
	{
		m_mesh.SetShader(shader);
	}
	else
	{
		m_model.SetShader(shader);
	}
}
Mesh Object::GetMesh()
{
	return m_mesh;
}
void Object::UpdatePosition(const glm::vec3& position)
{
	m_mesh.SetPosition(position);
}
void Object::UpdateRotation(const glm::vec3& angles)
{
	// Represent each rotation as a quaternion	
	float theta_x = glm::radians(angles[0]);
	float theta_y = glm::radians(angles[1]);
	float theta_z = glm::radians(angles[2]);

	glm::quat q_x = glm::angleAxis(theta_x, glm::vec3(1.0f, 0.0f, 0.0f)); // x unit quaternion
	glm::quat q_y = glm::angleAxis(theta_y, glm::vec3(0.0f, 1.0f, 0.0f)); // y unit quaternion
	glm::quat q_z = glm::angleAxis(theta_z, glm::vec3(0.0f, 0.0f, 1.0f)); // z unit quaternion

	// Multiply quaternions q_total = q_z * q_y * q_x (non-commutative)
	glm::quat q_total = q_z * q_y * q_x;

	glm::vec3 axis;
	float s = sqrt(q_total.x * q_total.x + q_total.y * q_total.y + q_total.z * q_total.z);
	if (s < 1e-6)
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
	else
		axis = glm::vec3(q_total.x, q_total.y, q_total.z) / s;

	float theta = 2.0f * acos(q_total.w);

	m_mesh.SetRotation(glm::degrees(theta), axis);
}
void Object::UpdateScale(const glm::vec3& scale)
{
	m_mesh.SetScale(scale);
}

void Object::Render(const Camera& camera)
{
	if (isPrimitive)
	{
		m_mesh.Draw(camera);
	}
	else
	{
		m_model.Draw(camera);
	}
}