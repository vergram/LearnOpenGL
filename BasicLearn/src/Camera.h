#pragma once

#include <glm/glm.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float Speed = 4.0f;
const float MouseSensitivity = 0.1f;
const float YAW = -90.0f;
const float PITCH = 0.0f;

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 GetProjectViewMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::vec3 GetPosition() const;
	glm::vec3 GetForward() const;

	void MoveCameraPosition(Camera_Movement direction, float deltaTime);
	void MoveCameraDirection(float xOffset, float yOffset, bool constrainPitch = true);
	void ZoomCameraView(float offset);


private:
	float m_Yaw;
	float m_Pitch;
	float m_Fov;
	glm::vec3 m_Forward;
	glm::vec3 m_Position;
	glm::vec3 m_WorldUp;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::mat4 m_Projection;

	void updateCameraVectors();
};

