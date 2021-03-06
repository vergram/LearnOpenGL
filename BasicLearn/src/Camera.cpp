#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera::Camera()
	:m_Forward(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_Projection(glm::perspective(45.0f, 1.3f, 0.1f, 100.0f)),
	m_Yaw(YAW),
	m_Pitch(PITCH),
	m_Fov(45.0f)
{
	updateCameraVectors();
}

Camera::~Camera()
{}

glm::mat4 Camera::GetProjectViewMatrix() const
{
	return m_Projection * glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_Projection;
}

glm::vec3 Camera::GetPosition() const
{
	return m_Position;
}

glm::vec3 Camera::GetForward() const
{
	return m_Forward;
}

void Camera::MoveCameraPosition(Camera_Movement direction, float deltaTime)
{
	float movement = Speed * deltaTime;
	switch (direction)
	{
	case FORWARD:
		m_Position += m_Forward * movement;
		break;
	case BACKWARD:
		m_Position -= m_Forward * movement;
		break;
	case LEFT:
		m_Position -= m_Right * movement;
		break;
	case RIGHT:
		m_Position += m_Right * movement;
		break;
	default:
		break;
	}
}

void Camera::MoveCameraDirection(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::ZoomCameraView(float offset)
{
	if(m_Fov > 1.0f && m_Fov < 90.0f)
	{
		m_Fov -= offset;
	}
	if (m_Fov < 1.0f)
	{
		m_Fov = 1.0f;
	}
	if (m_Fov > 90.0f)
	{
		m_Fov = 90.0f;
	}
	m_Projection = glm::perspective(m_Fov, 1.3f, 0.1f, 100.0f);
}

void Camera::updateCameraVectors()
{
	m_Forward.x = glm::cos(glm::radians(m_Pitch)) * glm::cos(glm::radians(m_Yaw));
	m_Forward.y = glm::sin(glm::radians(m_Pitch));
	m_Forward.z = glm::cos(glm::radians(m_Pitch)) * glm::sin(glm::radians(m_Yaw));

	// need normalize here because we need a unit direction vector to caculate the position movement
	m_Forward = glm::normalize(m_Forward);

	m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}
