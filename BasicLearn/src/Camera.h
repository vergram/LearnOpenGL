#pragma once

#include <glm/glm.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float Speed = 1.0f;
const float MouseSensitivity = 0.1f;

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 GetViewMatrix() const;

	void MoveCameraPosition(Camera_Movement direction, float deltaTime);
	void MoveCameraDirection(float xOffset, float yOffset, bool constrainPitch = true);
	void ZoomCameraView(float offset);
	void SetYaw(float yaw);
	void SetPitch(float pitch);
private:
	glm::vec3 m_Forward;
	glm::vec3 m_Position;
	glm::vec3 m_WorldUp;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::mat4 m_Projection;

	float m_Yaw;
	float m_Pitch;
	float m_Fov;

	void updateCameraVectors();
};

